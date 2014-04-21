#include <Python.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/python.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"

using namespace boost::python;

struct MeshTraits : public OpenMesh::DefaultTraits
{
	/// Use double precision points
	typedef OpenMesh::Vec3d Point;

	/// Use double precision Normals
	typedef OpenMesh::Vec3d Normal;

	/// Use RGBA Color
	typedef OpenMesh::Vec4f Color;
};

typedef OpenMesh::TriMesh_ArrayKernelT<MeshTraits> TriMesh;
typedef OpenMesh::PolyMesh_ArrayKernelT<MeshTraits> PolyMesh;

typedef OpenMesh::BaseHandle BaseHandle;
typedef OpenMesh::VertexHandle VertexHandle;
typedef OpenMesh::HalfedgeHandle HalfedgeHandle;
typedef OpenMesh::EdgeHandle EdgeHandle;
typedef OpenMesh::FaceHandle FaceHandle;

template <class Vector, class Scalar>
void set_item(Vector& _vec, int _index, Scalar _value) {
	if (_index < 0) {
		_index += _vec.size();
	}

	if ((size_t)_index < _vec.size()) {
		_vec[_index] = _value;
	}
	else {
		PyErr_SetString(PyExc_IndexError, "index out of range");
		throw_error_already_set();
	}
}

template <class Vector, class Scalar>
Scalar get_item(Vector& _vec, int _index) {
	if (_index < 0) {
		_index += _vec.size();
	}

	if ((size_t)_index < _vec.size()) {
		return _vec[_index];
	}
	else {
		PyErr_SetString(PyExc_IndexError, "index out of range");
		throw_error_already_set();
	}

	return 0.0;
}

template<class Iterator, size_t (OpenMesh::ArrayKernel::*n_items)() const>
class IteratorWrapperT {
	public:
		IteratorWrapperT(OpenMesh::PolyConnectivity& mesh) :
			iterator(mesh, typename Iterator::value_type(0)), iterator_end(mesh, typename Iterator::value_type(int((mesh.*n_items)()))) {
		}

		IteratorWrapperT iter() {
			return *this;
		}

		typename Iterator::value_type next() {
			if (iterator != iterator_end) {
				typename Iterator::value_type res = *iterator;
				++iterator;
				return res;
			}
			else {
				PyErr_SetString(PyExc_StopIteration, "No more data.");
				throw_error_already_set();
			}
			return typename Iterator::value_type();
		}

		unsigned int len() {
			return std::distance(iterator, iterator_end);
		}

	private:
		Iterator iterator;
		Iterator iterator_end;
};

template<class Circulator>
class CirculatorWrapperT {
	public:
		CirculatorWrapperT(typename Circulator::mesh_type& mesh, typename Circulator::center_type center) :
			circulator(mesh, center) {
		}

		CirculatorWrapperT iter() {
			return *this;
		}

		typename Circulator::value_type next() {
			if (circulator.is_valid()) {
				typename Circulator::value_type res = *circulator;
				++circulator;
				return res;
			}
			else {
				PyErr_SetString(PyExc_StopIteration, "No more data.");
				throw_error_already_set();
			}
			return typename Circulator::value_type();
		}

	private:
		Circulator circulator;
};

template<class Mesh>
class MeshWrapperT : public Mesh {
	public:
		IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices> vertices() {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices>(*this);
		}

		IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges> halfedges() {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges>(*this);
		}

		IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges> edges() {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges>(*this);
		}

		IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces> faces() {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces>(*this);
		}

		CirculatorWrapperT<typename Mesh::VertexVertexIter> vv(VertexHandle handle) {
			return CirculatorWrapperT<typename Mesh::VertexVertexIter>(*this, handle);
		}

		CirculatorWrapperT<typename Mesh::VertexFaceIter> vf(VertexHandle handle) {
			return CirculatorWrapperT<typename Mesh::VertexFaceIter>(*this, handle);
		}

		CirculatorWrapperT<typename Mesh::FaceVertexIter> fv(FaceHandle handle) {
			return CirculatorWrapperT<typename Mesh::FaceVertexIter>(*this, handle);
		}
};

void expose_vec3d() {
	struct VecFn {
		static OpenMesh::Vec3d *makeNullPoint() { return new OpenMesh::Vec3d(0, 0, 0); }
	};

	double (OpenMesh::Vec3d::*max1)() const = &OpenMesh::Vec3d::max;
	OpenMesh::Vec3d (OpenMesh::Vec3d::*max2)(const OpenMesh::Vec3d&) const = &OpenMesh::Vec3d::max;
	double (OpenMesh::Vec3d::*min1)() const = &OpenMesh::Vec3d::min;
	OpenMesh::Vec3d (OpenMesh::Vec3d::*min2)(const OpenMesh::Vec3d&) const = &OpenMesh::Vec3d::min;

	OpenMesh::Vec3d& (OpenMesh::Vec3d::*maximize)(const OpenMesh::Vec3d&) = &OpenMesh::Vec3d::maximize;
	OpenMesh::Vec3d& (OpenMesh::Vec3d::*minimize)(const OpenMesh::Vec3d&) = &OpenMesh::Vec3d::minimize;
	OpenMesh::Vec3d& (OpenMesh::Vec3d::*normalize)() = &OpenMesh::Vec3d::normalize;
	OpenMesh::Vec3d& (OpenMesh::Vec3d::*normalize_cond)() = &OpenMesh::Vec3d::normalize_cond;

	class_<OpenMesh::Vec3d>("Vec3d", init<double, double, double>())
		.def("__init__", make_constructor(&VecFn::makeNullPoint))
		.def(self_ns::str(self))
		.def(self == self)
		.def(self != self)
		.def(self < self)
		.def(self * self)
		.def(self / self)
		.def(self + self)
		.def(self - self)
		.def(self *= self)
		.def(self /= self)
		.def(self += self)
		.def(self -= self)
		.def(double() * self)
		.def(self * double())
		.def(self / double())
		.def(self *= double())
		.def(self /= double())
		.def("dot", &OpenMesh::Vec3d::operator|)
		.def("cross", &OpenMesh::Vec3d::operator%)
		.def("length", &OpenMesh::Vec3d::length)
		.def("norm", &OpenMesh::Vec3d::norm)
		.def("sqrnorm", &OpenMesh::Vec3d::sqrnorm)
		.def("l1_norm", &OpenMesh::Vec3d::l1_norm)
		.def("l8_norm", &OpenMesh::Vec3d::l8_norm)
		.def("max", max1)
		.def("max", max2)
		.def("min", min1)
		.def("min", min2)
		.def("maximize", maximize, return_internal_reference<>())
		.def("minimize", minimize, return_internal_reference<>())
		.def("mean", &OpenMesh::Vec3d::mean)
		.def("mean_abs", &OpenMesh::Vec3d::mean_abs)
		.def("max_abs", &OpenMesh::Vec3d::max_abs)
		.def("min_abs", &OpenMesh::Vec3d::min_abs)
		.def("maximized", &OpenMesh::Vec3d::maximized)
		.def("minimized", &OpenMesh::Vec3d::minimized)
		.def("normalize", normalize, return_internal_reference<>())
		.def("normalize_cond", normalize_cond, return_internal_reference<>())
		.def("normalized", &OpenMesh::Vec3d::normalized)
		.def("__setitem__", &set_item<OpenMesh::Vec3d, double>)
		.def("__getitem__", &get_item<OpenMesh::Vec3d, double>)
		;
}

void expose_handles() {
	class_<BaseHandle>("BaseHandle")
		.def("idx", &BaseHandle::idx)
		.def("is_valid", &BaseHandle::is_valid)
		.def("reset", &BaseHandle::reset)
		.def("invalidate", &BaseHandle::invalidate)
		.def(self == self)
		.def(self != self)
		.def(self < self)
		;
	class_<VertexHandle, bases<BaseHandle> >("VertexHandle");
	class_<HalfedgeHandle, bases<BaseHandle> >("HalfedgeHandle");
	class_<EdgeHandle, bases<BaseHandle> >("EdgeHandle");
	class_<FaceHandle, bases<BaseHandle> >("FaceHandle");
}

template<typename Mesh>
void expose_openmesh_type(const char *typeName) {
	EdgeHandle (Mesh::*edge_handle_uint)(unsigned int) const = &Mesh::edge_handle;
	FaceHandle (Mesh::*face_handle_uint)(unsigned int) const = &Mesh::face_handle;

	EdgeHandle (Mesh::*edge_handle_heh)(HalfedgeHandle) const = &Mesh::edge_handle;
	FaceHandle (Mesh::*face_handle_heh)(HalfedgeHandle) const = &Mesh::face_handle;

	typename Mesh::Point& (Mesh::*point)(VertexHandle) = &Mesh::point;

	FaceHandle (Mesh::*add_face)(VertexHandle, VertexHandle, VertexHandle) = &Mesh::add_face;

	class_<Mesh> classMeshT = class_<Mesh>(typeName);

	/*
	 * It is important that we enter the scope before we add
	 * the definitions because in some of the builders classes
	 * which are supposed to be inside the scope are defined.
	 */
	scope scope_MeshT = classMeshT;

	classMeshT
		.def("vertices", &Mesh::vertices)
		.def("halfedges", &Mesh::halfedges)
		.def("edges", &Mesh::edges)
		.def("faces", &Mesh::faces)
		.def("n_vertices", &Mesh::n_vertices)
		.def("n_halfedges", &Mesh::n_halfedges)
		.def("n_edges", &Mesh::n_edges)
		.def("n_faces", &Mesh::n_faces)
		.def("add_vertex", &Mesh::add_vertex)
		.def("point", point, return_value_policy<copy_non_const_reference>())
		.def("add_face", add_face)
		.def("vertex_handle", &Mesh::vertex_handle)
		.def("edge_handle", edge_handle_uint)
		.def("edge_handle", edge_handle_heh)
		.def("face_handle", face_handle_uint)
		.def("face_handle", face_handle_heh)
		.def("to_vertex_handle", &Mesh::to_vertex_handle)
		.def("vv", &Mesh::vv)
		.def("vf", &Mesh::vf)
		.def("fv", &Mesh::fv)
		;
}

template<class Iterator, size_t (OpenMesh::ArrayKernel::*n_items)() const>
void expose_iterator(const char *typeName) {
	class_<IteratorWrapperT<Iterator, n_items> >(typeName, init<MeshWrapperT<TriMesh>&>())
		.def(init<MeshWrapperT<PolyMesh>&>())
		.def("__iter__", &IteratorWrapperT<Iterator, n_items>::iter)
		.def("__next__", &IteratorWrapperT<Iterator, n_items>::next)
		.def("__len__", &IteratorWrapperT<Iterator, n_items>::len)
		.def("next", &IteratorWrapperT<Iterator, n_items>::next)
		;
}

template<class Circulator>
void expose_circulator(const char *typeName) {
	class_<CirculatorWrapperT<Circulator> >(typeName, init<MeshWrapperT<TriMesh>&, typename Circulator::center_type>())
		.def(init<MeshWrapperT<PolyMesh>&, typename Circulator::center_type>())
		.def("__iter__", &CirculatorWrapperT<Circulator>::iter)
		.def("__next__", &CirculatorWrapperT<Circulator>::next)
		.def("next", &CirculatorWrapperT<Circulator>::next)
		;
}

BOOST_PYTHON_MODULE(openmesh) {
	expose_vec3d();
	expose_handles();

	expose_openmesh_type<MeshWrapperT<TriMesh> >("TriMesh");
	expose_openmesh_type<MeshWrapperT<PolyMesh> >("PolyMesh");

	expose_iterator<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices>("VertexIter");
	expose_iterator<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges>("HalfedgeIter");
	expose_iterator<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges>("EdgeIter");
	expose_iterator<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces>("FaceIter");

	expose_circulator<OpenMesh::PolyConnectivity::VertexVertexIter>("VertexVertexIter");
	expose_circulator<OpenMesh::PolyConnectivity::VertexIHalfedgeIter>("VertexIHalfedgeIter");
	expose_circulator<OpenMesh::PolyConnectivity::VertexOHalfedgeIter>("VertexOHalfedgeIter");
	expose_circulator<OpenMesh::PolyConnectivity::VertexEdgeIter>("VertexEdgeIter");
	expose_circulator<OpenMesh::PolyConnectivity::VertexFaceIter>("VertexFaceIter");

	expose_circulator<OpenMesh::PolyConnectivity::FaceVertexIter>("FaceVertexIter");
	expose_circulator<OpenMesh::PolyConnectivity::FaceHalfedgeIter>("FaceHalfedgeIter");
	expose_circulator<OpenMesh::PolyConnectivity::FaceEdgeIter>("FaceEdgeIter");
	expose_circulator<OpenMesh::PolyConnectivity::FaceFaceIter>("FaceFaceIter");

	// TODO expose_circulator<OpenMesh::PolyConnectivity::HalfedgeLoopIter>("HalfedgeLoopIter");
}

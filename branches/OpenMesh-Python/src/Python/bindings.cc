/** @file */

/**
 * Defines the return value policy for functions that return references to
 * objects that are managed by %OpenMesh.
 */
#define OPENMESH_PYTHON_DEFAULT_POLICY return_value_policy<copy_const_reference>()

#include <Python.h>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/python.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/copy_const_reference.hpp>
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Utils/PropertyManager.hh"

namespace OpenMesh {

/**
 * Contains everything that is needed to expose %OpenMesh to %Python.
 */
namespace Python {

using namespace boost::python;

using OpenMesh::BaseHandle;
using OpenMesh::VertexHandle;
using OpenMesh::HalfedgeHandle;
using OpenMesh::EdgeHandle;
using OpenMesh::FaceHandle;


struct MeshTraits : public OpenMesh::DefaultTraits {
	/// Use double precision points
	typedef OpenMesh::Vec3d Point;

	/// Use double precision Normals
	typedef OpenMesh::Vec3d Normal;

	/// Use RGBA Color
	typedef OpenMesh::Vec4f Color;
};

typedef OpenMesh::TriMesh_ArrayKernelT<MeshTraits> TriMesh;
typedef OpenMesh::PolyMesh_ArrayKernelT<MeshTraits> PolyMesh;

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

/**
 * Wrapper for mesh item iterators.
 *
 * This class template is used to wrap mesh item iterators for %Python. It
 * implements %Python's iterator protocol (the magic methods \_\_iter\_\_ and
 * \_\_next\_\_).
 *
 * @tparam Iterator An iterator type.
 * @tparam n_items A member function pointer that points to the mesh function
 * that returns the number of items to iterate over (e.g. n_vertices).
 */
template<class Iterator, size_t (OpenMesh::ArrayKernel::*n_items)() const>
class IteratorWrapperT {
	public:

		/**
		 * Constructor
		 *
		 * @param _mesh The mesh that contains the items to iterate over.
		 */
		IteratorWrapperT(const OpenMesh::PolyConnectivity& _mesh) :
			mesh_(_mesh), n_items_(n_items),
			iterator_(_mesh, typename Iterator::value_type(0)),
			iterator_end_(_mesh, typename Iterator::value_type(int((_mesh.*n_items)()))) {
		}

		/**
		 * Implementation of %Python's \_\_iter\_\_ magic method.
		 *
		 * @return This iterator.
		 */
		IteratorWrapperT iter() const {
			return *this;
		}

		/**
		 * Implementation of %Python's \_\_next\_\_ magic method.
		 *
		 * @return The next item. Raises a %Python StopIteration exception if
		 * there are no more items.
		 */
		typename Iterator::value_type next() {
			if (iterator_ != iterator_end_) {
				typename Iterator::value_type res = *iterator_;
				++iterator_;
				return res;
			}
			else {
				PyErr_SetString(PyExc_StopIteration, "No more data.");
				throw_error_already_set();
			}
			return typename Iterator::value_type();
		}

		/**
		 * Implementation of %Python's \_\_len\_\_ magic method.
		 *
		 * @return The number of items in the mesh.
		 */
		unsigned int len() const {
			return (mesh_.*n_items_)();
		}

	private:
		const OpenMesh::PolyConnectivity& mesh_;
		size_t (OpenMesh::ArrayKernel::*n_items_)() const;
		Iterator iterator_;
		Iterator iterator_end_;
};

/**
 * Wrapper for circulators.
 *
 * This class template is used to wrap circulators for %Python. It implements
 * %Python's iterator protocol (the magic methods \_\_iter\_\_ and
 * \_\_next\_\_).
 *
 * @tparam Circulator A circulator type.
 */
template<class Circulator>
class CirculatorWrapperT {
	public:

		/**
		 * Constructor
		 *
		 * @param _mesh The mesh that contains the items to iterate over.
		 * @param _center The handle to the center item.
		 */
		CirculatorWrapperT(const typename Circulator::mesh_type& _mesh, typename Circulator::center_type _center) :
			circulator_(_mesh, _center) {
		}

		/**
		 * Implementation of %Python's \_\_iter\_\_ magic method.
		 *
		 * @return This circulator.
		 */
		CirculatorWrapperT iter() const {
			return *this;
		}

		/**
		 * Implementation of %Python's \_\_next\_\_ magic method.
		 *
		 * @return The next item. Raises a %Python StopIteration exception if
		 * there are no more items.
		 */
		typename Circulator::value_type next() {
			if (circulator_.is_valid()) {
				typename Circulator::value_type res = *circulator_;
				++circulator_;
				return res;
			}
			else {
				PyErr_SetString(PyExc_StopIteration, "No more data.");
				throw_error_already_set();
			}
			return typename Circulator::value_type();
		}

	private:
		Circulator circulator_;
};

/**
 * Wrapper for meshes.
 *
 * This class template is used to wrap meshes for %Python. It inherits from it's
 * template parameter (a mesh type) and implements functions that are required
 * by %Python but not provided by mesh types.
 *
 * @tparam Mesh A mesh type.
 */
template<class Mesh>
class MeshWrapperT : public Mesh {
	public:

		/**
		 * Thin wrapper for Mesh::garbage_collection.
		 *
		 * This wrapper function is required because Mesh::garbage_collection
		 * has default arguments and therefore cannot be exposed directly.
		 */
		void garbage_collection() {
			Mesh::garbage_collection();
		}

		/**
		 * Set the status of a vertex.
		 *
		 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::status may
		 * return by value instead of reference. This function ensures that the
		 * status of an item can be changed nonetheless.
		 */
		void set_status(VertexHandle _vh, const OpenMesh::Attributes::StatusInfo &_info) {
			Mesh::status(_vh) = _info;
		}

		/**
		 * Set the status of a halfedge.
		 *
		 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::status may
		 * return by value instead of reference. This function ensures that the
		 * status of an item can be changed nonetheless.
		 */
		void set_status(HalfedgeHandle _heh, const OpenMesh::Attributes::StatusInfo &_info) {
			Mesh::status(_heh) = _info;
		}

		/**
		 * Set the status of an edge.
		 *
		 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::status may
		 * return by value instead of reference. This function ensures that the
		 * status of an item can be changed nonetheless.
		 */
		void set_status(EdgeHandle _eh, const OpenMesh::Attributes::StatusInfo &_info) {
			Mesh::status(_eh) = _info;
		}

		/**
		 * Set the status of a face.
		 *
		 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::status may
		 * return by value instead of reference. This function ensures that the
		 * status of an item can be changed nonetheless.
		 */
		void set_status(FaceHandle _fh, const OpenMesh::Attributes::StatusInfo &_info) {
			Mesh::status(_fh) = _info;
		}

		/**
		 * Get a vertex iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices> vertices() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices>(*this);
		}

		/**
		 * Get a halfedge iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges> halfedges() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges>(*this);
		}

		/**
		 * Get an edge iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges> edges() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges>(*this);
		}

		/**
		 * Get a face iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces> faces() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces>(*this);
		}

		/**
		 * Get a vertex-vertex circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexVertexIter> vv(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexVertexIter>(*this, _handle);
		}

		/**
		 * Get a vertex-(incoming)halfedge circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexIHalfedgeIter> vih(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexIHalfedgeIter>(*this, _handle);
		}

		/**
		 * Get a vertex-(outgoing)halfedge circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexOHalfedgeIter> voh(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexOHalfedgeIter>(*this, _handle);
		}

		/**
		 * Get a vertex-edge circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexEdgeIter> ve(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexEdgeIter>(*this, _handle);
		}

		/**
		 * Get a vertex-face circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexFaceIter> vf(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexFaceIter>(*this, _handle);
		}

		/**
		 * Get a face-vertex circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceVertexIter> fv(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceVertexIter>(*this, _handle);
		}

		/**
		 * Get a face-halfedge circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceHalfedgeIter> fh(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceHalfedgeIter>(*this, _handle);
		}

		/**
		 * Get a face-edge circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceEdgeIter> fe(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceEdgeIter>(*this, _handle);
		}

		/**
		 * Get a face-face circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceFaceIter> ff(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceFaceIter>(*this, _handle);
		}
};

/**
 * Wrapper for property managers.
 *
 * This class template is used to wrap property managers for %Python. It
 * inherits from it's first template parameter (a property manager) and
 * implements functions that are required by %Python but not provided by
 * property manager types.
 *
 * @tparam Manager A property manager type.
 * @tparam IndexHandle The appropriate index handle type.
 */
template<class Manager, class IndexHandle>
class PropertyManagerWrapperT : public Manager {
	public:

		/**
		 * Constructor
		 */
		PropertyManagerWrapperT() : Manager() { }

		/**
		 * Constructor
		 *
		 * TODO: reference constructor
		 */
		PropertyManagerWrapperT(OpenMesh::PolyConnectivity &_mesh, const char *_propname, bool _existing = false) :
			Manager(_mesh, _propname, _existing) {
		}

		/**
		 * Thin wrapper for Manager::retain.
		 *
		 * This wrapper function is required because Manager::retain has default
		 * arguments and therefore cannot be exposed directly.
		 */
		void retain_void() {
			Manager::retain();
		}

		/**
		 * Thin wrapper for Manager::retain.
		 *
		 * This wrapper function is required because Manager::retain has default
		 * arguments and therefore cannot be exposed directly.
		 */
		void retain_bool(bool _retain) {
			Manager::retain(_retain);
		}

		/**
		 * Implementation of %Python's \_\_getitem\_\_ magic method.
		 *
		 * @param _handle A handle of the appropriate handle type.
		 * @return The requested property value.
		 */
		object getitem(IndexHandle _handle) const {
			return (*this)[_handle];
		}

		/**
		 * Implementation of %Python's \_\_setitem\_\_ magic method.
		 *
		 * @param _handle A handle of the appropriate handle type.
		 * @param _value The property value to be set.
		 */
		void setitem(IndexHandle _handle, object _value) {
			(*this)[_handle] = _value;
		}

		/**
		 * Thin wrapper for Manager::propertyExists.
		 *
		 * This wrapper function is required because %Python does not know that
		 * MeshWrapperT is derived from PolyConnectivity.
		 */
		static bool property_exists(MeshWrapperT<TriMesh> &_mesh, const char *_propname) {
			return Manager::propertyExists(_mesh, _propname);
		}

		/**
		 * Thin wrapper for Manager::propertyExists.
		 *
		 * This wrapper function is required because %Python does not know that
		 * MeshWrapperT is derived from PolyConnectivity.
		 */
		static bool property_exists(MeshWrapperT<PolyMesh> &_mesh, const char *_propname) {
			return Manager::propertyExists(_mesh, _propname);
		}

		/**
		 * Thin wrapper for Manager::createIfNotExists.
		 *
		 * This wrapper function is required because %Python does not know that
		 * MeshWrapperT is derived from PolyConnectivity.
		 */
		static typename Manager::Proxy create_if_not_exists(MeshWrapperT<TriMesh> &_mesh, const char *_propname) {
			return Manager::createIfNotExists(_mesh, _propname);
		}

		/**
		 * Thin wrapper for Manager::createIfNotExists.
		 *
		 * This wrapper function is required because %Python does not know that
		 * MeshWrapperT is derived from PolyConnectivity.
		 */
		static typename Manager::Proxy create_if_not_exists(MeshWrapperT<PolyMesh> &_mesh, const char *_propname) {
			return Manager::createIfNotExists(_mesh, _propname);
		}
};

/**
 * Expose a vector type to %Python.
 *
 * This function template is used to expose vectors to %Python. The template
 * parameters are used to instantiate the appropriate vector type.
 *
 * @tparam Scalar A scalar type.
 * @tparam N The dimension of the vector.
 *
 * @param _name The name of the vector type to be exposed.
 *
 * @note N must be either 2, 3 or 4.
 */
template<class Scalar, int N>
void expose_vec(const char *_name) {
	typedef OpenMesh::VectorT<Scalar, N> Vector;

	Scalar (Vector::*min_void)() const = &Vector::min;
	Scalar (Vector::*max_void)() const = &Vector::max;

	Vector (Vector::*max_vector)(const Vector&) const = &Vector::max;
	Vector (Vector::*min_vector)(const Vector&) const = &Vector::min;

	class_<Vector> classVector = class_<Vector>(_name);

	classVector
		.def("__setitem__", &set_item<Vector, Scalar>)
		.def("__getitem__", &get_item<Vector, Scalar>)
		.def(self == self)
		.def(self != self)
		.def(self *= Scalar())
		.def(self /= Scalar())
		.def(self * Scalar())
		.def(Scalar() * self)
		.def(self / Scalar())
		.def(self *= self)
		.def(self /= self)
		.def(self -= self)
		.def(self += self)
		.def(self * self)
		.def(self / self)
		.def(self + self)
		.def(self - self)
		.def(-self)
		.def("dot", &Vector::operator|)
		.def("vectorize", &Vector::vectorize, return_internal_reference<>())
		.def(self < self)

		.def("norm", &Vector::norm)
		.def("length", &Vector::length)
		.def("sqrnorm", &Vector::sqrnorm)
		.def("normalize", &Vector::normalize, return_internal_reference<>())
		.def("normalized", &Vector::normalized)
		.def("normalize_cond", &Vector::normalize_cond, return_internal_reference<>())

		.def("l1_norm", &Vector::l1_norm)
		.def("l8_norm", &Vector::l8_norm)

		.def("max", max_void)
		.def("max_abs", &Vector::max_abs)
		.def("min", min_void)
		.def("min_abs", &Vector::min_abs)
		.def("mean", &Vector::mean)
		.def("mean_abs", &Vector::mean_abs)
		.def("minimize", &Vector::minimize, return_internal_reference<>())
		.def("minimized", &Vector::minimized)
		.def("maximize", &Vector::maximize, return_internal_reference<>())
		.def("maximized", &Vector::maximized)
		.def("min", min_vector)
		.def("max", max_vector)

		.def("size", &Vector::size)
		.staticmethod("size")
		.def("vectorized", &Vector::vectorized)
		.staticmethod("vectorized")
		;

	typedef OpenMesh::VectorT<Scalar, 2> Vector2;
	typedef OpenMesh::VectorT<Scalar, 3> Vector3;
	typedef OpenMesh::VectorT<Scalar, 4> Vector4;

	struct Factory {
		static Vector2 *vec2_default() {
			return new Vector2(Scalar(), Scalar());
		}
		static Vector2 *vec2_user_defined(const Scalar& _v0, const Scalar& _v1) {
			return new Vector2(_v0, _v1);
		}
		static Vector3 *vec3_default() {
			return new Vector3(Scalar(), Scalar(), Scalar());
		}
		static Vector3 *vec3_user_defined(const Scalar& _v0, const Scalar& _v1, const Scalar& _v2) {
			return new Vector3(_v0, _v1, _v2);
		}
		static Vector4 *vec4_default() {
			return new Vector4(Scalar(), Scalar(), Scalar(), Scalar());
		}
		static Vector4 *vec4_user_defined(const Scalar& _v0, const Scalar& _v1, const Scalar& _v2, const Scalar& _v3) {
			return new Vector4(_v0, _v1, _v2, _v3);
		}
	};

	Vector3 (Vector3::*cross)(const Vector3&) const = &Vector3::operator%;

	if (N == 2) {
		classVector
			.def("__init__", make_constructor(&Factory::vec2_default))
			.def("__init__", make_constructor(&Factory::vec2_user_defined))
			;
	}

	if (N == 3) {
		classVector
			.def("__init__", make_constructor(&Factory::vec3_default))
			.def("__init__", make_constructor(&Factory::vec3_user_defined))
			.def("cross", cross)
			;
	}

	if (N == 4) {
		classVector
			.def("__init__", make_constructor(&Factory::vec4_default))
			.def("__init__", make_constructor(&Factory::vec4_user_defined))
			;
	}
}

/**
 * Expose mesh item handles to %Python.
 */
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


/**
 * Expose the StatusBits enum and StatusInfo class to %Python.
 */
void expose_status_bits_and_info() {
	using OpenMesh::Attributes::StatusBits;
	using OpenMesh::Attributes::StatusInfo;

	enum_<StatusBits>("StatusBits")
		.value("DELETED", OpenMesh::Attributes::DELETED)
		.value("LOCKED", OpenMesh::Attributes::LOCKED)
		.value("SELECTED", OpenMesh::Attributes::SELECTED)
		.value("HIDDEN", OpenMesh::Attributes::HIDDEN)
		.value("FEATURE", OpenMesh::Attributes::FEATURE)
		.value("TAGGED", OpenMesh::Attributes::TAGGED)
		.value("TAGGED2", OpenMesh::Attributes::TAGGED2)
		.value("FIXEDNONMANIFOLD", OpenMesh::Attributes::FIXEDNONMANIFOLD)
		.value("UNUSED", OpenMesh::Attributes::UNUSED)
		;

	class_<StatusInfo>("StatusInfo")
		.def("deleted", &StatusInfo::deleted)
		.def("set_deleted", &StatusInfo::set_deleted)
		.def("locked", &StatusInfo::locked)
		.def("set_locked", &StatusInfo::set_locked)
		.def("selected", &StatusInfo::selected)
		.def("set_selected", &StatusInfo::set_selected)
		.def("hidden", &StatusInfo::hidden)
		.def("set_hidden", &StatusInfo::set_hidden)
		.def("feature", &StatusInfo::feature)
		.def("set_feature", &StatusInfo::set_feature)
		.def("tagged", &StatusInfo::tagged)
		.def("set_tagged", &StatusInfo::set_tagged)
		.def("tagged2", &StatusInfo::tagged2)
		.def("set_tagged2", &StatusInfo::set_tagged2)
		.def("fixed_nonmanifold", &StatusInfo::fixed_nonmanifold)
		.def("set_fixed_nonmanifold", &StatusInfo::set_fixed_nonmanifold)
		.def("bits", &StatusInfo::bits)
		.def("set_bits", &StatusInfo::set_bits)
		.def("is_bit_set", &StatusInfo::is_bit_set)
		.def("set_bit", &StatusInfo::set_bit)
		.def("unset_bit", &StatusInfo::unset_bit)
		.def("change_bit", &StatusInfo::change_bit)
		;
}

/**
 * Expose a mesh type to %Python.
 *
 * @tparam Mesh A mesh type.
 *
 * @param _name The name of the mesh type to be exposed.
 *
 * @note Meshes are wrapped by MeshWrapperT before they are exposed to %Python,
 * i.e. they are not exposed directly.
 */
template<class Mesh>
void expose_mesh(const char *_name) {
	using OpenMesh::Attributes::StatusInfo;

	VertexHandle   (Mesh::*vertex_handle_uint  )(unsigned int  ) const = &Mesh::vertex_handle;
	HalfedgeHandle (Mesh::*halfedge_handle_uint)(unsigned int  ) const = &Mesh::halfedge_handle;
	EdgeHandle     (Mesh::*edge_handle_uint    )(unsigned int  ) const = &Mesh::edge_handle;
	FaceHandle     (Mesh::*face_handle_uint    )(unsigned int  ) const = &Mesh::face_handle;
	HalfedgeHandle (Mesh::*halfedge_handle_vh  )(VertexHandle  ) const = &Mesh::halfedge_handle;
	HalfedgeHandle (Mesh::*halfedge_handle_fh  )(FaceHandle    ) const = &Mesh::halfedge_handle;

	EdgeHandle     (Mesh::*edge_handle_heh     )(HalfedgeHandle) const = &Mesh::edge_handle;
	FaceHandle     (Mesh::*face_handle_heh     )(HalfedgeHandle) const = &Mesh::face_handle;

	HalfedgeHandle (Mesh::*halfedge_handle_eh_uint)(EdgeHandle, unsigned int) const = &Mesh::halfedge_handle;
	HalfedgeHandle (Mesh::*prev_halfedge_handle_heh)(HalfedgeHandle) const = &Mesh::prev_halfedge_handle;

	void (Mesh::*set_halfedge_handle_vh_heh)(VertexHandle, HalfedgeHandle) = &Mesh::set_halfedge_handle;
	void (Mesh::*set_halfedge_handle_fh_heh)(FaceHandle, HalfedgeHandle  ) = &Mesh::set_halfedge_handle;

	FaceHandle (Mesh::*add_face)(VertexHandle, VertexHandle, VertexHandle) = &Mesh::add_face;

	// Get value of a standard property (point, normal, color)
	const typename Mesh::Point&  (Mesh::*point_vh )(VertexHandle  ) const = &Mesh::point;
	const typename Mesh::Normal& (Mesh::*normal_vh)(VertexHandle  ) const = &Mesh::normal;
	const typename Mesh::Normal& (Mesh::*normal_hh)(HalfedgeHandle) const = &Mesh::normal;
	const typename Mesh::Normal& (Mesh::*normal_fh)(FaceHandle    ) const = &Mesh::normal;
	const typename Mesh::Color&  (Mesh::*color_vh )(VertexHandle  ) const = &Mesh::color;
	const typename Mesh::Color&  (Mesh::*color_hh )(HalfedgeHandle) const = &Mesh::color;
	const typename Mesh::Color&  (Mesh::*color_eh )(EdgeHandle    ) const = &Mesh::color;
	const typename Mesh::Color&  (Mesh::*color_fh )(FaceHandle    ) const = &Mesh::color;

	// Get value of a standard property (texture coordinate)
	const typename Mesh::TexCoord1D& (Mesh::*texcoord1D_vh)(VertexHandle  ) const = &Mesh::texcoord1D;
	const typename Mesh::TexCoord1D& (Mesh::*texcoord1D_hh)(HalfedgeHandle) const = &Mesh::texcoord1D;
	const typename Mesh::TexCoord2D& (Mesh::*texcoord2D_vh)(VertexHandle  ) const = &Mesh::texcoord2D;
	const typename Mesh::TexCoord2D& (Mesh::*texcoord2D_hh)(HalfedgeHandle) const = &Mesh::texcoord2D;
	const typename Mesh::TexCoord3D& (Mesh::*texcoord3D_vh)(VertexHandle  ) const = &Mesh::texcoord3D;
	const typename Mesh::TexCoord3D& (Mesh::*texcoord3D_hh)(HalfedgeHandle) const = &Mesh::texcoord3D;

	// Get value of a standard property (status)
	const StatusInfo& (Mesh::*status_vh)(VertexHandle  ) const = &Mesh::status;
	const StatusInfo& (Mesh::*status_hh)(HalfedgeHandle) const = &Mesh::status;
	const StatusInfo& (Mesh::*status_eh)(EdgeHandle    ) const = &Mesh::status;
	const StatusInfo& (Mesh::*status_fh)(FaceHandle    ) const = &Mesh::status;

	// Set value of a standard property (point, normal, color)
	void (Mesh::*set_normal_vh)(VertexHandle,   const typename Mesh::Normal&) = &Mesh::set_normal;
	void (Mesh::*set_normal_hh)(HalfedgeHandle, const typename Mesh::Normal&) = &Mesh::set_normal;
	void (Mesh::*set_normal_fh)(FaceHandle,     const typename Mesh::Normal&) = &Mesh::set_normal;
	void (Mesh::*set_color_vh )(VertexHandle,   const typename Mesh::Color& ) = &Mesh::set_color;
	void (Mesh::*set_color_hh )(HalfedgeHandle, const typename Mesh::Color& ) = &Mesh::set_color;
	void (Mesh::*set_color_eh )(EdgeHandle,     const typename Mesh::Color& ) = &Mesh::set_color;
	void (Mesh::*set_color_fh )(FaceHandle,     const typename Mesh::Color& ) = &Mesh::set_color;

	// Set value of a standard property (texture coordinate)
	void (Mesh::*set_texcoord1D_vh)(VertexHandle,   const typename Mesh::TexCoord1D&) = &Mesh::set_texcoord1D;
	void (Mesh::*set_texcoord1D_hh)(HalfedgeHandle, const typename Mesh::TexCoord1D&) = &Mesh::set_texcoord1D;
	void (Mesh::*set_texcoord2D_vh)(VertexHandle,   const typename Mesh::TexCoord2D&) = &Mesh::set_texcoord2D;
	void (Mesh::*set_texcoord2D_hh)(HalfedgeHandle, const typename Mesh::TexCoord2D&) = &Mesh::set_texcoord2D;
	void (Mesh::*set_texcoord3D_vh)(VertexHandle,   const typename Mesh::TexCoord3D&) = &Mesh::set_texcoord3D;
	void (Mesh::*set_texcoord3D_hh)(HalfedgeHandle, const typename Mesh::TexCoord3D&) = &Mesh::set_texcoord3D;

	// Set value of a standard property (status)
	void (Mesh::*set_status_vh)(VertexHandle,   const StatusInfo&) = &Mesh::set_status;
	void (Mesh::*set_status_hh)(HalfedgeHandle, const StatusInfo&) = &Mesh::set_status;
	void (Mesh::*set_status_eh)(EdgeHandle,     const StatusInfo&) = &Mesh::set_status;
	void (Mesh::*set_status_fh)(FaceHandle,     const StatusInfo&) = &Mesh::set_status;

	class_<Mesh> classMesh = class_<Mesh>(_name);

	/*
	 * It is important that we enter the scope before we add
	 * the definitions because in some of the builders classes
	 * which are supposed to be inside the scope are defined.
	 */
	scope scope_Mesh = classMesh;

	classMesh
		.def("vertex_handle", vertex_handle_uint)
		.def("halfedge_handle", halfedge_handle_uint)
		.def("edge_handle", edge_handle_uint)
		.def("face_handle", face_handle_uint)

		.def("clear", &Mesh::clear)
		.def("clean", &Mesh::clean)
		.def("garbage_collection", &Mesh::garbage_collection)

		.def("n_vertices", &Mesh::n_vertices)
		.def("n_halfedges", &Mesh::n_halfedges)
		.def("n_edges", &Mesh::n_edges)
		.def("n_faces", &Mesh::n_faces)
		.def("vertices_empty", &Mesh::vertices_empty)
		.def("halfedges_empty", &Mesh::halfedges_empty)
		.def("edges_empty", &Mesh::edges_empty)
		.def("faces_empty", &Mesh::faces_empty)

		.def("halfedge_handle", halfedge_handle_vh)
		.def("set_halfedge_handle", set_halfedge_handle_vh_heh)

		.def("to_vertex_handle", &Mesh::to_vertex_handle)
		.def("from_vertex_handle", &Mesh::from_vertex_handle)
		.def("set_vertex_handle", &Mesh::set_vertex_handle)
		.def("face_handle", face_handle_heh)
		.def("set_face_handle", &Mesh::set_face_handle)
		.def("next_halfedge_handle", &Mesh::next_halfedge_handle)
		.def("set_next_halfedge_handle", &Mesh::set_next_halfedge_handle)
		.def("prev_halfedge_handle", prev_halfedge_handle_heh)
		.def("opposite_halfedge_handle", &Mesh::opposite_halfedge_handle)
		.def("ccw_rotated_halfedge_handle", &Mesh::ccw_rotated_halfedge_handle)
		.def("cw_rotated_halfedge_handle", &Mesh::cw_rotated_halfedge_handle)
		.def("edge_handle", edge_handle_heh)

		.def("halfedge_handle", halfedge_handle_eh_uint)

		.def("halfedge_handle", halfedge_handle_fh)
		.def("set_halfedge_handle", set_halfedge_handle_fh_heh)

		.def("point", point_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_point", &Mesh::set_point)
		.def("normal", normal_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_normal", set_normal_vh)
		.def("normal", normal_hh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_normal", set_normal_hh)
		.def("color", color_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_color", set_color_vh)
		.def("texcoord1D", texcoord1D_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_texcoord1D", set_texcoord1D_vh)
		.def("texcoord2D", texcoord2D_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_texcoord2D", set_texcoord2D_vh)
		.def("texcoord3D", texcoord3D_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_texcoord3D", set_texcoord3D_vh)
		.def("texcoord1D", texcoord1D_hh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_texcoord1D", set_texcoord1D_hh)
		.def("texcoord2D", texcoord2D_hh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_texcoord2D", set_texcoord2D_hh)
		.def("texcoord3D", texcoord3D_hh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_texcoord3D", set_texcoord3D_hh)
		.def("status", status_vh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_status", set_status_vh)
		.def("status", status_hh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_status", set_status_hh)
		.def("color", color_hh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_color", set_color_hh)
		.def("color", color_eh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_color", set_color_eh)
		.def("status", status_eh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_status", set_status_eh)
		.def("normal", normal_fh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_normal", set_normal_fh)
		.def("color", color_fh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_color", set_color_fh)
		.def("status", status_fh, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("set_status", set_status_fh)

		.def("request_vertex_normals", &Mesh::request_vertex_normals)
		.def("request_vertex_colors", &Mesh::request_vertex_colors)
		.def("request_vertex_texcoords1D", &Mesh::request_vertex_texcoords1D)
		.def("request_vertex_texcoords2D", &Mesh::request_vertex_texcoords2D)
		.def("request_vertex_texcoords3D", &Mesh::request_vertex_texcoords3D)
		.def("request_vertex_status", &Mesh::request_vertex_status)
		.def("request_halfedge_status", &Mesh::request_halfedge_status)
		.def("request_halfedge_normals", &Mesh::request_halfedge_normals)
		.def("request_halfedge_colors", &Mesh::request_halfedge_colors)
		.def("request_halfedge_texcoords1D", &Mesh::request_halfedge_texcoords1D)
		.def("request_halfedge_texcoords2D", &Mesh::request_halfedge_texcoords2D)
		.def("request_halfedge_texcoords3D", &Mesh::request_halfedge_texcoords3D)
		.def("request_edge_status", &Mesh::request_edge_status)
		.def("request_edge_colors", &Mesh::request_edge_colors)
		.def("request_face_normals", &Mesh::request_face_normals)
		.def("request_face_colors", &Mesh::request_face_colors)
		.def("request_face_status", &Mesh::request_face_status)
		.def("request_face_texture_index", &Mesh::request_face_texture_index)

		.def("release_vertex_normals", &Mesh::release_vertex_normals)
		.def("release_vertex_colors", &Mesh::release_vertex_colors)
		.def("release_vertex_texcoords1D", &Mesh::release_vertex_texcoords1D)
		.def("release_vertex_texcoords2D", &Mesh::release_vertex_texcoords2D)
		.def("release_vertex_texcoords3D", &Mesh::release_vertex_texcoords3D)
		.def("release_vertex_status", &Mesh::release_vertex_status)
		.def("release_halfedge_status", &Mesh::release_halfedge_status)
		.def("release_halfedge_normals", &Mesh::release_halfedge_normals)
		.def("release_halfedge_colors", &Mesh::release_halfedge_colors)
		.def("release_halfedge_texcoords1D", &Mesh::release_halfedge_texcoords1D)
		.def("release_halfedge_texcoords2D", &Mesh::release_halfedge_texcoords2D)
		.def("release_halfedge_texcoords3D", &Mesh::release_halfedge_texcoords3D)
		.def("release_edge_status", &Mesh::release_edge_status)
		.def("release_edge_colors", &Mesh::release_edge_colors)
		.def("release_face_normals", &Mesh::release_face_normals)
		.def("release_face_colors", &Mesh::release_face_colors)
		.def("release_face_status", &Mesh::release_face_status)
		.def("release_face_texture_index", &Mesh::release_face_texture_index)

		.def("has_vertex_normals", &Mesh::has_vertex_normals)
		.def("has_vertex_colors", &Mesh::has_vertex_colors)
		.def("has_vertex_texcoords1D", &Mesh::has_vertex_texcoords1D)
		.def("has_vertex_texcoords2D", &Mesh::has_vertex_texcoords2D)
		.def("has_vertex_texcoords3D", &Mesh::has_vertex_texcoords3D)
		.def("has_vertex_status", &Mesh::has_vertex_status)
		.def("has_halfedge_status", &Mesh::has_halfedge_status)
		.def("has_halfedge_normals", &Mesh::has_halfedge_normals)
		.def("has_halfedge_colors", &Mesh::has_halfedge_colors)
		.def("has_halfedge_texcoords1D", &Mesh::has_halfedge_texcoords1D)
		.def("has_halfedge_texcoords2D", &Mesh::has_halfedge_texcoords2D)
		.def("has_halfedge_texcoords3D", &Mesh::has_halfedge_texcoords3D)
		.def("has_edge_status", &Mesh::has_edge_status)
		.def("has_edge_colors", &Mesh::has_edge_colors)
		.def("has_face_normals", &Mesh::has_face_normals)
		.def("has_face_colors", &Mesh::has_face_colors)
		.def("has_face_status", &Mesh::has_face_status)
		.def("has_face_texture_index", &Mesh::has_face_texture_index)

		.def("vertices", &Mesh::vertices)
		.def("halfedges", &Mesh::halfedges)
		.def("edges", &Mesh::edges)
		.def("faces", &Mesh::faces)

		.def("vv", &Mesh::vv)
		.def("vih", &Mesh::vih)
		.def("voh", &Mesh::voh)
		.def("ve", &Mesh::ve)
		.def("vf", &Mesh::vf)

		.def("fv", &Mesh::fv)
		.def("fh", &Mesh::fh)
		.def("fe", &Mesh::fe)
		.def("ff", &Mesh::ff)

		.def("add_vertex", &Mesh::add_vertex)
		.def("add_face", add_face)
		.def("vertex_handle", &Mesh::vertex_handle)
		;
}

/**
 * Expose an iterator type to %Python.
 *
 * @tparam Iterator An iterator type.
 * @tparam n_items A member function pointer that points to the mesh function
 * that returns the number of items to iterate over (e.g. n_vertices).
 *
 * @param _name The name of the iterator type to be exposed.
 *
 * @note %Iterators are wrapped by IteratorWrapperT before they are exposed to
 * %Python, i.e. they are not exposed directly.
 */
template<class Iterator, size_t (OpenMesh::ArrayKernel::*n_items)() const>
void expose_iterator(const char *_name) {
	class_<IteratorWrapperT<Iterator, n_items> >(_name, init<MeshWrapperT<TriMesh>&>())
		.def(init<MeshWrapperT<PolyMesh>&>())
		.def("__iter__", &IteratorWrapperT<Iterator, n_items>::iter)
		.def("__next__", &IteratorWrapperT<Iterator, n_items>::next)
		.def("__len__", &IteratorWrapperT<Iterator, n_items>::len)
		.def("next", &IteratorWrapperT<Iterator, n_items>::next)
		;
}

/**
 * Expose a circulator type to %Python.
 *
 * @tparam Circulator A circulator type.
 *
 * @param _name The name of the circulator type to be exposed.
 *
 * @note Circulators are wrapped by CirculatorWrapperT before they are exposed
 * to %Python, i.e. they are not exposed directly.
 */
template<class Circulator>
void expose_circulator(const char *_name) {
	class_<CirculatorWrapperT<Circulator> >(_name, init<MeshWrapperT<TriMesh>&, typename Circulator::center_type>())
		.def(init<MeshWrapperT<PolyMesh>&, typename Circulator::center_type>())
		.def("__iter__", &CirculatorWrapperT<Circulator>::iter)
		.def("__next__", &CirculatorWrapperT<Circulator>::next)
		.def("next", &CirculatorWrapperT<Circulator>::next)
		;
}

/**
 * Expose a property manager type to %Python.
 *
 * This function template is used to expose property managers to %Python. The
 * template parameters are used to instantiate the appropriate property manager
 * type.
 *
 * @tparam PropHandle A property handle type (e.g. %VPropHandle\<object\>).
 * @tparam IndexHandle The appropriate index handle type (e.g. %VertexHandle for
 * %VPropHandle\<object\>).
 *
 * @param _name The name of the property manager type to be exposed.
 *
 * @note Property managers are wrapped by PropertyManagerWrapperT before they
 * are exposed to %Python, i.e. they are not exposed directly.
 */
template<class PropHandle, class IndexHandle>
void expose_property_manager(const char *_name) {
	typedef OpenMesh::PropertyManager<PropHandle, OpenMesh::PolyConnectivity> PropertyManager;
	typedef PropertyManagerWrapperT<PropertyManager, IndexHandle> PropertyManagerWrapper;

	bool (*property_exists_tri )(MeshWrapperT<TriMesh>&,  const char*) = &PropertyManagerWrapper::property_exists;
	bool (*property_exists_poly)(MeshWrapperT<PolyMesh>&, const char*) = &PropertyManagerWrapper::property_exists;

	typename PropertyManager::Proxy (*create_if_not_exists_tri )(MeshWrapperT<TriMesh>&,  const char*) = &PropertyManagerWrapper::create_if_not_exists;
	typename PropertyManager::Proxy (*create_if_not_exists_poly)(MeshWrapperT<PolyMesh>&, const char*) = &PropertyManagerWrapper::create_if_not_exists;

	class_<typename PropertyManager::Proxy>("Proxy", no_init);

	class_<PropertyManagerWrapper, boost::noncopyable>(_name)
		.def(init<MeshWrapperT<TriMesh>&, const char *, optional<bool> >())
		.def(init<MeshWrapperT<PolyMesh>&, const char *, optional<bool> >())

		.def("swap", &PropertyManagerWrapper::swap)
		.def("is_valid", &PropertyManagerWrapper::isValid)

		.def("__bool__", &PropertyManagerWrapper::operator bool)
		.def("__nonzero__", &PropertyManagerWrapper::operator bool)

		.def("get_name", &PropertyManagerWrapper::getName, return_internal_reference<>())
		.def("get_mesh", &PropertyManagerWrapper::getMesh, return_value_policy<reference_existing_object>())
		.def("retain", &PropertyManagerWrapper::retain_void)
		.def("retain", &PropertyManagerWrapper::retain_bool)

		.def("__getitem__", &PropertyManagerWrapper::getitem)
		.def("__setitem__", &PropertyManagerWrapper::setitem)

		.def("property_exists", property_exists_tri)
		.def("property_exists", property_exists_poly)
		.staticmethod("property_exists")

		.def("create_if_not_exists", create_if_not_exists_tri)
		.def("create_if_not_exists", create_if_not_exists_poly)
		.staticmethod("create_if_not_exists")
		;
}

BOOST_PYTHON_MODULE(openmesh) {
	expose_vec<float,  2>("Vec2f");
	expose_vec<float,  3>("Vec3f");
	expose_vec<float,  4>("Vec4f");
	expose_vec<double, 2>("Vec2d");
	expose_vec<double, 3>("Vec3d");
	expose_vec<double, 4>("Vec4d");

	expose_handles();
	expose_status_bits_and_info();

	expose_mesh<MeshWrapperT<TriMesh> >("TriMesh");
	expose_mesh<MeshWrapperT<PolyMesh> >("PolyMesh");

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

	expose_property_manager<OpenMesh::VPropHandleT<object>, VertexHandle>("VPropertyManager");
}

} // namespace Python
} // namespace OpenMesh

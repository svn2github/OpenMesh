/*
 * bindings.cc
 *
 *  Created on: May 30, 2012
 *      Author: ebke
 */

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/python.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/return_internal_reference.hpp>
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"

using namespace boost::python;

struct TriTraits : public OpenMesh::DefaultTraits
{
  /// Use double precision points
  typedef OpenMesh::Vec3d Point;
  /// Use double precision Normals
  typedef OpenMesh::Vec3d Normal;

  /// Use RGBA Color
  typedef OpenMesh::Vec4f Color;

};

namespace {
    inline object pass_through(object const& o) { return o; }
}

template<typename IterType, typename Type>
class IteratorWrapper {
    public:
        typedef IteratorWrapper<IterType, Type> This;

        IteratorWrapper(IterType begin, IterType end) :
            it(begin), end(end) {

        }

        Type next() {
            IterType result = it;
            if (++it == end) {
                PyErr_SetString(PyExc_StopIteration, "No more data.");
                boost::python::throw_error_already_set();
            }
            return *result;
        }

        static void wrap(const char *name) {
            class_<This>(name, no_init)
                    .def("next", &This::next, with_custodian_and_ward_postcall<0, 1>())
                    .def("__iter__", pass_through)
                    ;
        }

    private:
        IterType it, end;
};

template<class SurroundingType, class ParamType, class IterType, class Type>
class IteratorWrapperBuilder {
    public:
        typedef IteratorWrapper<IterType, Type> *result_type;
        typedef IteratorWrapper<IterType, Type> result_deref_type;

        typedef IterType (SurroundingType::*IterBuilder)(ParamType);
        IteratorWrapperBuilder(IterBuilder beginFn, IterBuilder endFn)
            : beginFn(beginFn), endFn(endFn) {
        }

        IteratorWrapper<IterType, Type> *operator() (SurroundingType &collection, ParamType param) {
            return new IteratorWrapper<IterType, Type>((collection.*beginFn)(param), (collection.*endFn)(param));
        }

    private:
        IterBuilder beginFn, endFn;
};

template<class SurroundingType, class ParamType, class IterType>
object buildIteratorWrapperBuilder(
        IterType (SurroundingType::*beginFn)(ParamType), IterType (SurroundingType::*endFn)(ParamType), const char *wrappedTypeName) {

    typedef IteratorWrapperBuilder<SurroundingType, ParamType, IterType, typename IterType::value_type> IWB;

    static bool wrapped = false;
    if (!wrapped) {
        wrapped = true;
        IWB::result_deref_type::wrap(wrappedTypeName);
    }

    return make_function(
            IWB(beginFn, endFn),
            return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >(),
            boost::mpl::vector<typename IWB::result_type, SurroundingType&, ParamType>());
}

template <class VectorT, class Scalar>
void set_item(VectorT& _vec, int _index, Scalar _value) {
  if (_index < 0)
    _index += _vec.size();

  if ((size_t)_index < _vec.size())
    _vec[_index] = _value;
  else {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    throw_error_already_set();
  }
}

template <class VectorT, class Scalar>
Scalar get_item(VectorT& _vec, int _index) {
  if (_index < 0)
    _index += _vec.size();

  if ((size_t)_index < _vec.size())
    return _vec[_index];
  else {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    throw_error_already_set();
  }

  return 0.0;
}

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
                boost::python::throw_error_already_set();
            }
            return typename Circulator::value_type();
        }

    private:
        Circulator circulator;
};

template<class Mesh>
class MeshWrapperT : public Mesh {
    public:
        typedef typename Mesh::VertexHandle VertexHandle;
        typedef typename Mesh::FaceHandle FaceHandle;

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

template<typename MeshT>
void expose_openmesh_type(const char *typeName) {
    {
        class_<MeshT> classMeshT = class_<MeshT>(typeName);

        /*
         * It is important that we enter the scope before we add
         * the definitions because in some of the builders classes
         * which are supposed to be inside the scope are defined.
         */
        scope scope_MeshT = classMeshT;

        classMeshT
            /*
             * vertices should return an iterable object with __len__
             * defined. Then we could get rid of n_vertices.
             */
            .add_property("vertices", range((typename MeshT::VertexIter (MeshT::*)())&MeshT::vertices_begin, (typename MeshT::VertexIter (MeshT::*)())&MeshT::vertices_end))
            .def("n_vertices", &MeshT::n_vertices)
            .def("n_halfedges", &MeshT::n_halfedges)
            .def("n_edges", &MeshT::n_edges)
            .def("n_faces", &MeshT::n_faces)
            .def("add_vertex", &MeshT::add_vertex)
            .def("point", (typename MeshT::Point &(MeshT::*)(typename MeshT::VertexHandle))&MeshT::point, return_internal_reference<>())
            .def("add_face", (typename MeshT::FaceHandle (MeshT::*)(typename MeshT::VertexHandle, typename MeshT::VertexHandle, typename MeshT::VertexHandle))&MeshT::add_face)
            .def("vertex_handle", &MeshT::vertex_handle)
            .def("vv", &MeshT::vv)
            .def("vf", &MeshT::vf)
            .def("fv", &MeshT::fv)
            ;

        class_<OpenMesh::BaseHandle>("BaseHandle")
            .def("idx", &OpenMesh::BaseHandle::idx)
            .def("is_valid", &OpenMesh::BaseHandle::is_valid)
            .def("reset", &OpenMesh::BaseHandle::reset)
            .def("invalidate", &OpenMesh::BaseHandle::invalidate)
            .def(self == self)
            .def(self != self)
            .def(self < self)
            ;
        class_<typename MeshT::VertexHandle, bases<OpenMesh::BaseHandle> >("VertexHandle");
        class_<typename MeshT::FaceHandle, bases<OpenMesh::BaseHandle> >("FaceHandle");
    }
}

BOOST_PYTHON_MODULE(openmesh) {
    typedef OpenMesh::TriMesh_ArrayKernelT<TriTraits> TriMesh;
    expose_vec3d();
    expose_openmesh_type<MeshWrapperT<TriMesh> >("TriMesh");

    class_<CirculatorWrapperT<typename TriMesh::VertexVertexIter> >("VertexVertexIter", no_init)
            .def("__iter__", &CirculatorWrapperT<typename TriMesh::VertexVertexIter>::iter)
            .def("__next__", &CirculatorWrapperT<typename TriMesh::VertexVertexIter>::next)
            ;
    class_<CirculatorWrapperT<typename TriMesh::VertexFaceIter> >("VertexFaceIter", no_init)
            .def("__iter__", &CirculatorWrapperT<typename TriMesh::VertexFaceIter>::iter)
            .def("__next__", &CirculatorWrapperT<typename TriMesh::VertexFaceIter>::next)
            ;
    class_<CirculatorWrapperT<typename TriMesh::FaceVertexIter> >("FaceVertexIter", no_init)
            .def("__iter__", &CirculatorWrapperT<typename TriMesh::FaceVertexIter>::iter)
            .def("__next__", &CirculatorWrapperT<typename TriMesh::FaceVertexIter>::next)
            ;
}

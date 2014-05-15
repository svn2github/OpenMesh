#include "Python/Bindings.hh"
#include "Python/Vector.hh"
#include "Python/Mesh.hh"
#include "Python/Iterator.hh"
#include "Python/Circulator.hh"
#include "Python/PropertyManager.hh"

namespace OpenMesh {
namespace Python {

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

BOOST_PYTHON_MODULE(openmesh) {
	expose_handles();
	expose_status_bits_and_info();

	expose_vec<float,  2>("Vec2f");
	expose_vec<float,  3>("Vec3f");
	expose_vec<float,  4>("Vec4f");
	expose_vec<double, 2>("Vec2d");
	expose_vec<double, 3>("Vec3d");
	expose_vec<double, 4>("Vec4d");

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

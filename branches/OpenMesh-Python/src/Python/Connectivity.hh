#ifndef OPENMESH_PYTHON_CONNECTIVITY_HH
#define OPENMESH_PYTHON_CONNECTIVITY_HH

#include "Python/Bindings.hh"
#include <boost/python/stl_iterator.hpp>

namespace OpenMesh {
namespace Python {

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(delete_vertex_overloads, delete_vertex, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(delete_edge_overloads, delete_edge, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(delete_face_overloads, delete_face, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(is_boundary_overloads, is_boundary, 1, 2)

/**
 * Create a new face from a %Python list of vertex handles.
 *
 * This function template is used to generate %Python member functions for the
 * connectivity types.
 *
 * @tparam Connectivity A connectivity type (e.g. PolyConnectivity).
 *
 * @param _self The instance for which the function should be called.
 * @param _vhandles The list of vertex handles.
 */
template<class Connectivity>
FaceHandle add_face(Connectivity& _self, const list& _vhandles) {
	stl_input_iterator<VertexHandle> begin(_vhandles);
	stl_input_iterator<VertexHandle> end;

	std::vector<VertexHandle> vector;
	vector.insert(vector.end(), begin, end);

	return _self.add_face(vector);
}

/**
 * Exposes the PolyConnectivity type to %Python.
 */
void expose_poly_connectivity() {
	// Vertex and face valence
	unsigned int (PolyConnectivity::*valence_vh)(VertexHandle) const = &PolyConnectivity::valence;
	unsigned int (PolyConnectivity::*valence_fh)(FaceHandle  ) const = &PolyConnectivity::valence;

	// Triangulate face or mesh
	void (PolyConnectivity::*triangulate_fh  )(FaceHandle) = &PolyConnectivity::triangulate;
	void (PolyConnectivity::*triangulate_void)(          ) = &PolyConnectivity::triangulate;

	// Adding items to a mesh
	FaceHandle (PolyConnectivity::*add_face_3_vh)(VertexHandle, VertexHandle, VertexHandle) = &PolyConnectivity::add_face;
	FaceHandle (PolyConnectivity::*add_face_4_vh)(VertexHandle, VertexHandle, VertexHandle, VertexHandle) = &PolyConnectivity::add_face;
	FaceHandle (*add_face_list)(PolyConnectivity&, const list&) = &add_face;

	// Boundary and manifold tests
	bool (PolyConnectivity::*is_boundary_hh)(HalfedgeHandle  ) const = &PolyConnectivity::is_boundary;
	bool (PolyConnectivity::*is_boundary_eh)(EdgeHandle      ) const = &PolyConnectivity::is_boundary;
	bool (PolyConnectivity::*is_boundary_vh)(VertexHandle    ) const = &PolyConnectivity::is_boundary;
	bool (PolyConnectivity::*is_boundary_fh)(FaceHandle, bool) const = &PolyConnectivity::is_boundary;

	// Generic handle derefertiation
	const PolyConnectivity::Vertex&   (PolyConnectivity::*deref_vh)(VertexHandle  ) const = &PolyConnectivity::deref;
	const PolyConnectivity::Halfedge& (PolyConnectivity::*deref_hh)(HalfedgeHandle) const = &PolyConnectivity::deref;
	const PolyConnectivity::Edge&     (PolyConnectivity::*deref_eh)(EdgeHandle    ) const = &PolyConnectivity::deref;
	const PolyConnectivity::Face&     (PolyConnectivity::*deref_fh)(FaceHandle    ) const = &PolyConnectivity::deref;

	class_<PolyConnectivity>("PolyConnectivity")
		.def("assign_connectivity", &PolyConnectivity::assign_connectivity)
		.def("opposite_face_handle", &PolyConnectivity::opposite_face_handle)
		.def("adjust_outgoing_halfedge", &PolyConnectivity::adjust_outgoing_halfedge)
		.def("find_halfedge", &PolyConnectivity::find_halfedge)
		.def("valence", valence_vh)
		.def("valence", valence_fh)
		.def("collapse", &PolyConnectivity::collapse)
		.def("is_simple_link", &PolyConnectivity::is_simple_link)
		.def("is_simply_connected", &PolyConnectivity::is_simply_connected)
		.def("remove_edge", &PolyConnectivity::remove_edge)
		.def("reinsert_edge", &PolyConnectivity::reinsert_edge)
		.def("insert_edge", &PolyConnectivity::insert_edge)
		.def("split", &PolyConnectivity::split)
		.def("split_copy", &PolyConnectivity::split_copy)
		.def("triangulate", triangulate_fh)
		.def("triangulate", triangulate_void)
		.def("split_edge", &PolyConnectivity::split_edge)
		.def("split_edge_copy", &PolyConnectivity::split_edge_copy)

		.def("add_vertex", &PolyConnectivity::add_vertex)
		.def("add_face", add_face_3_vh)
		.def("add_face", add_face_4_vh)
		.def("add_face", add_face_list)

		.def("is_collapse_ok",  &PolyConnectivity::is_collapse_ok)
		.def("delete_vertex", &PolyConnectivity::delete_vertex, delete_vertex_overloads())
		.def("delete_edge", &PolyConnectivity::delete_edge, delete_edge_overloads())
		.def("delete_face", &PolyConnectivity::delete_face, delete_face_overloads())

		.def("is_boundary", is_boundary_hh)
		.def("is_boundary", is_boundary_eh)
		.def("is_boundary", is_boundary_vh)
		.def("is_boundary", is_boundary_fh, is_boundary_overloads())
		.def("is_manifold", &PolyConnectivity::is_manifold)

		.def("deref", deref_vh, return_value_policy<reference_existing_object>())
		.def("deref", deref_hh, return_value_policy<reference_existing_object>())
		.def("deref", deref_eh, return_value_policy<reference_existing_object>())
		.def("deref", deref_fh, return_value_policy<reference_existing_object>())

		.def("is_triangles", &PolyConnectivity::is_triangles)
		.staticmethod("is_triangles")

		.def_readonly("InvalidVertexHandle", &PolyConnectivity::InvalidVertexHandle)
		.def_readonly("InvalidHalfedgeHandle", &PolyConnectivity::InvalidHalfedgeHandle)
		.def_readonly("InvalidEdgeHandle", &PolyConnectivity::InvalidEdgeHandle)
		.def_readonly("InvalidFaceHandle", &PolyConnectivity::InvalidFaceHandle)
		;
}

/**
 * Exposes the TriConnectivity type to %Python.
 */
void expose_tri_connectivity() {
	// Assign connectivity
	void (TriConnectivity::*assign_connectivity_tri )(const TriConnectivity& ) = &TriConnectivity::assign_connectivity;
	void (TriConnectivity::*assign_connectivity_poly)(const PolyConnectivity&) = &TriConnectivity::assign_connectivity;

	// Adding items to a mesh
	FaceHandle (TriConnectivity::*add_face_3_vh)(VertexHandle, VertexHandle, VertexHandle) = &TriConnectivity::add_face;
	FaceHandle (*add_face_list)(TriConnectivity&, const list&) = &add_face;

	// Topology modifying operators
	void (TriConnectivity::*split_eh_vh)(EdgeHandle, VertexHandle) = &TriConnectivity::split;
	void (TriConnectivity::*split_fh_vh)(FaceHandle, VertexHandle) = &TriConnectivity::split;
	void (TriConnectivity::*split_copy_eh_vh)(EdgeHandle, VertexHandle) = &TriConnectivity::split_copy;
	void (TriConnectivity::*split_copy_fh_vh)(FaceHandle, VertexHandle) = &TriConnectivity::split_copy;

	class_<TriConnectivity, bases<PolyConnectivity> >("TriConnectivity")
		.def("assign_connectivity", assign_connectivity_tri)
		.def("assign_connectivity", assign_connectivity_poly)
		.def("opposite_vh", &TriConnectivity::opposite_vh)
		.def("opposite_he_opposite_vh", &TriConnectivity::opposite_he_opposite_vh)

		.def("add_face", add_face_3_vh)
		.def("add_face", add_face_list)

		.def("is_collapse_ok", &TriConnectivity::is_collapse_ok)
		.def("vertex_split", &TriConnectivity::vertex_split)
		.def("is_flip_ok", &TriConnectivity::is_flip_ok)
		.def("flip", &TriConnectivity::flip)
		.def("split", split_eh_vh)
		.def("split_copy", split_copy_eh_vh)
		.def("split", split_fh_vh)
		.def("split_copy", split_copy_fh_vh)

		.def("is_triangles", &TriConnectivity::is_triangles)
		.staticmethod("is_triangles")
		;
}

} // namespace OpenMesh
} // namespace Python

#endif

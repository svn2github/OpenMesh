#ifndef OPENMESH_PYTHON_MESH_HH
#define OPENMESH_PYTHON_MESH_HH

#include "Python/Bindings.hh"
#include "Python/Iterator.hh"
#include "Python/Circulator.hh"

namespace OpenMesh {
namespace Python {

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(garbage_collection_overloads, garbage_collection, 0, 3)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(find_feature_edges_overloads, find_feature_edges, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(update_normal_overloads, update_normal, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(update_halfedge_normals_overloads, update_halfedge_normals, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(calc_halfedge_normal_overloads, calc_halfedge_normal, 1, 2)

/**
 * Set the status of an item.
 *
 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::status may
 * return by value instead of reference. This function ensures that the
 * status of an item can be changed nonetheless.
 */
template <class Mesh, class IndexHandle>
void set_status(Mesh& _mesh, IndexHandle _h, const OpenMesh::Attributes::StatusInfo& _info) {
	_mesh.status(_h) = _info;
}

/**
 * Thin wrapper for Mesh::add_property.
 *
 * @tparam PropHandle A property handle type.
 *
 * This wrapper function is required because Mesh::add_property has
 * default arguments and therefore cannot be exposed directly.
 */
template <class Mesh, class PropHandle>
void add_property_ph(Mesh& _mesh, PropHandle& _ph) {
	_mesh.add_property(_ph);
}

/**
 * Thin wrapper for Mesh::add_property.
 *
 * @tparam PropHandle A property handle type.
 *
 * This wrapper function is required because Mesh::add_property has
 * default arguments and therefore cannot be exposed directly.
 */
template <class Mesh, class PropHandle>
void add_property_ph_str(Mesh& _mesh, PropHandle& _ph, const std::string& _name) {
	_mesh.add_property(_ph, _name);
}

/**
 * Set the property value for the item represented by the handle.
 *
 * @tparam PropHandle A property handle type.
 * @tparam IndexHandle The appropriate index handle type.
 *
 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::property may
 * return by value instead of reference. This function ensures that the
 * property value of an item can be changed nonetheless.
 */
template <class Mesh, class PropHandle, class IndexHandle>
void set_property(Mesh& _mesh, PropHandle _ph, IndexHandle _h, const object& _value) {
	_mesh.property(_ph, _h) = _value;
}

/**
 * Thin wrapper for Mesh::copy_all_properties.
 *
 * @tparam IndexHandle A mesh item handle type.
 *
 * This wrapper function is required because Mesh::copy_all_properties
 * has default arguments and therefore cannot be exposed directly.
 */
template <class Mesh, class IndexHandle>
void copy_all_properties_ih_ih(Mesh& _mesh, IndexHandle _from, IndexHandle _to) {
	_mesh.copy_all_properties(_from, _to);
}

/**
 * Thin wrapper for Mesh::copy_all_properties.
 *
 * @tparam IndexHandle A mesh item handle type.
 *
 * This wrapper function is required because Mesh::copy_all_properties
 * has default arguments and therefore cannot be exposed directly.
 */
template <class Mesh, class IndexHandle>
void copy_all_properties_ih_ih_bool(Mesh& _mesh, IndexHandle _from, IndexHandle _to, bool _copyBuildIn) {
	_mesh.copy_all_properties(_from, _to, _copyBuildIn);
}

/**
 * Get an iterator.
 */
template <class Mesh, class Iterator, size_t (ArrayKernel::*n_items)() const>
IteratorWrapperT<Iterator, n_items> get_iterator(Mesh& _mesh) {
	return IteratorWrapperT<Iterator, n_items>(_mesh, typename Iterator::value_type(0));
}

/**
 * Get a skipping iterator.
 */
template <class Mesh, class Iterator, size_t (ArrayKernel::*n_items)() const>
IteratorWrapperT<Iterator, n_items> get_skipping_iterator(Mesh& _mesh) {
	return IteratorWrapperT<Iterator, n_items>(_mesh, typename Iterator::value_type(0), true);
}

/**
 * Get a circulator.
 */
template <class Mesh, class Circulator, class CenterEntityHandle>
CirculatorWrapperT<Circulator, CenterEntityHandle> get_circulator(Mesh& _mesh, CenterEntityHandle _handle) {
	return CirculatorWrapperT<Circulator, CenterEntityHandle>(_mesh, _handle);
}

/**
 * Expose a mesh type to %Python.
 *
 * @tparam Mesh A mesh type.
 * @tparam Connectivity The appropriate connectivity type (e.g. TriConnectivity
 * for triangle meshes).
 *
 * @param _name The name of the mesh type to be exposed.
 *
 */
template <class Mesh, class Connectivity>
void expose_mesh(const char *_name) {
	using OpenMesh::Attributes::StatusInfo;

	typedef typename Mesh::Scalar Scalar;
	typedef typename Mesh::Point  Point;
	typedef typename Mesh::Normal Normal;
	typedef typename Mesh::Color  Color;

	//======================================================================
	//  KernelT Function Pointers
	//======================================================================

	// Get the i'th item
	VertexHandle   (Mesh::*vertex_handle_uint  )(unsigned int) const = &Mesh::vertex_handle;
	HalfedgeHandle (Mesh::*halfedge_handle_uint)(unsigned int) const = &Mesh::halfedge_handle;
	EdgeHandle     (Mesh::*edge_handle_uint    )(unsigned int) const = &Mesh::edge_handle;
	FaceHandle     (Mesh::*face_handle_uint    )(unsigned int) const = &Mesh::face_handle;

	// Delete items
	void (Mesh::*garbage_collection)(bool, bool, bool) = &Mesh::garbage_collection;

	// Vertex connectivity
	HalfedgeHandle (Mesh::*halfedge_handle_vh)(VertexHandle) const = &Mesh::halfedge_handle;
	HalfedgeHandle (Mesh::*halfedge_handle_fh)(FaceHandle  ) const = &Mesh::halfedge_handle;

	// Halfedge connectivity
	FaceHandle     (Mesh::*face_handle_hh         )(HalfedgeHandle) const = &Mesh::face_handle;
	HalfedgeHandle (Mesh::*prev_halfedge_handle_hh)(HalfedgeHandle) const = &Mesh::prev_halfedge_handle;
	EdgeHandle     (Mesh::*edge_handle_hh         )(HalfedgeHandle) const = &Mesh::edge_handle;

	// Edge connectivity
	HalfedgeHandle (Mesh::*halfedge_handle_eh_uint)(EdgeHandle, unsigned int) const = &Mesh::halfedge_handle;

	// Set halfedge
	void (Mesh::*set_halfedge_handle_vh_hh)(VertexHandle, HalfedgeHandle) = &Mesh::set_halfedge_handle;
	void (Mesh::*set_halfedge_handle_fh_hh)(FaceHandle, HalfedgeHandle  ) = &Mesh::set_halfedge_handle;

	// Handle -> Item
	const typename Mesh::Vertex&   (Mesh::*vertex  )(VertexHandle  ) const = &Mesh::vertex;
	const typename Mesh::Halfedge& (Mesh::*halfedge)(HalfedgeHandle) const = &Mesh::halfedge;
	const typename Mesh::Edge&     (Mesh::*edge    )(EdgeHandle    ) const = &Mesh::edge;
	const typename Mesh::Face&     (Mesh::*face    )(FaceHandle    ) const = &Mesh::face;

	// Item -> Handle
	VertexHandle   (Mesh::*handle_v)(const typename Mesh::Vertex&  ) const = &Mesh::handle;
	HalfedgeHandle (Mesh::*handle_h)(const typename Mesh::Halfedge&) const = &Mesh::handle;
	EdgeHandle     (Mesh::*handle_e)(const typename Mesh::Edge&    ) const = &Mesh::handle;
	FaceHandle     (Mesh::*handle_f)(const typename Mesh::Face&    ) const = &Mesh::handle;

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
	void (*set_status_vh)(Mesh&, VertexHandle,   const StatusInfo&) = &set_status;
	void (*set_status_hh)(Mesh&, HalfedgeHandle, const StatusInfo&) = &set_status;
	void (*set_status_eh)(Mesh&, EdgeHandle,     const StatusInfo&) = &set_status;
	void (*set_status_fh)(Mesh&, FaceHandle,     const StatusInfo&) = &set_status;

	// Property management - add property (1/2)
	void (*add_property_vph)(Mesh&, VPropHandleT<object>&) = &add_property_ph;
	void (*add_property_eph)(Mesh&, EPropHandleT<object>&) = &add_property_ph;
	void (*add_property_hph)(Mesh&, HPropHandleT<object>&) = &add_property_ph;
	void (*add_property_fph)(Mesh&, FPropHandleT<object>&) = &add_property_ph;

	// Property management - add property (2/2)
	void (*add_property_vph_str)(Mesh&, VPropHandleT<object>&, const std::string&) = &add_property_ph_str;
	void (*add_property_eph_str)(Mesh&, EPropHandleT<object>&, const std::string&) = &add_property_ph_str;
	void (*add_property_hph_str)(Mesh&, HPropHandleT<object>&, const std::string&) = &add_property_ph_str;
	void (*add_property_fph_str)(Mesh&, FPropHandleT<object>&, const std::string&) = &add_property_ph_str;

	// Property management - remove property
	void (Mesh::*remove_property_vph)(VPropHandleT<object>&) = &Mesh::remove_property;
	void (Mesh::*remove_property_eph)(EPropHandleT<object>&) = &Mesh::remove_property;
	void (Mesh::*remove_property_hph)(HPropHandleT<object>&) = &Mesh::remove_property;
	void (Mesh::*remove_property_fph)(FPropHandleT<object>&) = &Mesh::remove_property;

	// Property management - get property by name
	bool (Mesh::*get_property_handle_vph)(VPropHandleT<object>&, const std::string&) const = &Mesh::get_property_handle;
	bool (Mesh::*get_property_handle_eph)(EPropHandleT<object>&, const std::string&) const = &Mesh::get_property_handle;
	bool (Mesh::*get_property_handle_hph)(HPropHandleT<object>&, const std::string&) const = &Mesh::get_property_handle;
	bool (Mesh::*get_property_handle_fph)(FPropHandleT<object>&, const std::string&) const = &Mesh::get_property_handle;

	// Property management - get property value for an item
	const object& (Mesh::*property_vertex  )(VPropHandleT<object>, VertexHandle  ) const = &Mesh::property;
	const object& (Mesh::*property_edge    )(EPropHandleT<object>, EdgeHandle    ) const = &Mesh::property;
	const object& (Mesh::*property_halfedge)(HPropHandleT<object>, HalfedgeHandle) const = &Mesh::property;
	const object& (Mesh::*property_face    )(FPropHandleT<object>, FaceHandle    ) const = &Mesh::property;

	// Property management - set property value for an item
	void (*set_property_vertex  )(Mesh&, VPropHandleT<object>, VertexHandle,   const object&) = &set_property;
	void (*set_property_edge    )(Mesh&, EPropHandleT<object>, EdgeHandle,     const object&) = &set_property;
	void (*set_property_halfedge)(Mesh&, HPropHandleT<object>, HalfedgeHandle, const object&) = &set_property;
	void (*set_property_face    )(Mesh&, FPropHandleT<object>, FaceHandle,     const object&) = &set_property;

	// Low-level adding new items
	VertexHandle (Mesh::*new_vertex_void )(void                        ) = &Mesh::new_vertex;
	VertexHandle (Mesh::*new_vertex_point)(const typename Mesh::Point& ) = &Mesh::new_vertex;
	FaceHandle   (Mesh::*new_face_void   )(void                        ) = &Mesh::new_face;
	FaceHandle   (Mesh::*new_face_face   )(const typename Mesh::Face&  ) = &Mesh::new_face;

	// Kernel item iterators
	IteratorWrapperT<typename Mesh::VertexIter,   &Mesh::n_vertices > (*vertices )(Mesh&) = &get_iterator;
	IteratorWrapperT<typename Mesh::HalfedgeIter, &Mesh::n_halfedges> (*halfedges)(Mesh&) = &get_iterator;
	IteratorWrapperT<typename Mesh::EdgeIter,     &Mesh::n_edges    > (*edges    )(Mesh&) = &get_iterator;
	IteratorWrapperT<typename Mesh::FaceIter,     &Mesh::n_faces    > (*faces    )(Mesh&) = &get_iterator;

	IteratorWrapperT<typename Mesh::VertexIter,   &Mesh::n_vertices > (*svertices )(Mesh&) = &get_skipping_iterator;
	IteratorWrapperT<typename Mesh::HalfedgeIter, &Mesh::n_halfedges> (*shalfedges)(Mesh&) = &get_skipping_iterator;
	IteratorWrapperT<typename Mesh::EdgeIter,     &Mesh::n_edges    > (*sedges    )(Mesh&) = &get_skipping_iterator;
	IteratorWrapperT<typename Mesh::FaceIter,     &Mesh::n_faces    > (*sfaces    )(Mesh&) = &get_skipping_iterator;

	//======================================================================
	//  BaseKernel Function Pointers
	//======================================================================

	// Copy all properties (1/2)
	void (*copy_all_properties_vh_vh)(Mesh&, VertexHandle,   VertexHandle  ) = &copy_all_properties_ih_ih;
	void (*copy_all_properties_eh_eh)(Mesh&, EdgeHandle,     EdgeHandle    ) = &copy_all_properties_ih_ih;
	void (*copy_all_properties_hh_hh)(Mesh&, HalfedgeHandle, HalfedgeHandle) = &copy_all_properties_ih_ih;
	void (*copy_all_properties_fh_fh)(Mesh&, FaceHandle,     FaceHandle    ) = &copy_all_properties_ih_ih;

	// Copy all properties (2/2)
	void (*copy_all_properties_vh_vh_bool)(Mesh&, VertexHandle,   VertexHandle,   bool) = &copy_all_properties_ih_ih_bool;
	void (*copy_all_properties_eh_eh_bool)(Mesh&, EdgeHandle,     EdgeHandle,     bool) = &copy_all_properties_ih_ih_bool;
	void (*copy_all_properties_hh_hh_bool)(Mesh&, HalfedgeHandle, HalfedgeHandle, bool) = &copy_all_properties_ih_ih_bool;
	void (*copy_all_properties_fh_fh_bool)(Mesh&, FaceHandle,     FaceHandle,     bool) = &copy_all_properties_ih_ih_bool;

	//======================================================================
	//  PolyConnectivity Function Pointers
	//======================================================================

	CirculatorWrapperT<typename Mesh::VertexVertexIter,    VertexHandle  > (*vv )(Mesh&, VertexHandle  ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::VertexIHalfedgeIter, VertexHandle  > (*vih)(Mesh&, VertexHandle  ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::VertexOHalfedgeIter, VertexHandle  > (*voh)(Mesh&, VertexHandle  ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::VertexEdgeIter,      VertexHandle  > (*ve )(Mesh&, VertexHandle  ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::VertexFaceIter,      VertexHandle  > (*vf )(Mesh&, VertexHandle  ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::FaceVertexIter,      FaceHandle    > (*fv )(Mesh&, FaceHandle    ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::FaceHalfedgeIter,    FaceHandle    > (*fh )(Mesh&, FaceHandle    ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::FaceEdgeIter,        FaceHandle    > (*fe )(Mesh&, FaceHandle    ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::FaceFaceIter,        FaceHandle    > (*ff )(Mesh&, FaceHandle    ) = &get_circulator;
	CirculatorWrapperT<typename Mesh::HalfedgeLoopIter,    HalfedgeHandle> (*hl )(Mesh&, HalfedgeHandle) = &get_circulator;

	//======================================================================
	//  PolyMeshT Function Pointers
	//======================================================================

	void (Mesh::*calc_edge_vector_eh_normal)(EdgeHandle,     Normal&) const = &Mesh::calc_edge_vector;
	void (Mesh::*calc_edge_vector_hh_normal)(HalfedgeHandle, Normal&) const = &Mesh::calc_edge_vector;

	Normal (Mesh::*calc_edge_vector_eh)(EdgeHandle    ) const = &Mesh::calc_edge_vector;
	Normal (Mesh::*calc_edge_vector_hh)(HalfedgeHandle) const = &Mesh::calc_edge_vector;

	Scalar (Mesh::*calc_edge_length_eh)(EdgeHandle    ) const = &Mesh::calc_edge_length;
	Scalar (Mesh::*calc_edge_length_hh)(HalfedgeHandle) const = &Mesh::calc_edge_length;

	Scalar (Mesh::*calc_edge_sqr_length_eh)(EdgeHandle    ) const = &Mesh::calc_edge_sqr_length;
	Scalar (Mesh::*calc_edge_sqr_length_hh)(HalfedgeHandle) const = &Mesh::calc_edge_sqr_length;

	Scalar (Mesh::*calc_dihedral_angle_fast_hh)(HalfedgeHandle) const = &Mesh::calc_dihedral_angle_fast;
	Scalar (Mesh::*calc_dihedral_angle_fast_eh)(EdgeHandle    ) const = &Mesh::calc_dihedral_angle_fast;

	Scalar (Mesh::*calc_dihedral_angle_hh)(HalfedgeHandle) const = &Mesh::calc_dihedral_angle;
	Scalar (Mesh::*calc_dihedral_angle_eh)(EdgeHandle    ) const = &Mesh::calc_dihedral_angle;

//	void (Mesh::*split_fh_point)(FaceHandle, const Point&) = &Mesh::split;
//	void (Mesh::*split_eh_point)(EdgeHandle, const Point&) = &Mesh::split;

	void (Mesh::*split_fh_vh)(FaceHandle, VertexHandle) = &Mesh::split;
	void (Mesh::*split_eh_vh)(EdgeHandle, VertexHandle) = &Mesh::split;

	void (Mesh::*update_normal_fh)(FaceHandle  ) = &Mesh::update_normal;
	void (Mesh::*update_normal_vh)(VertexHandle) = &Mesh::update_normal;

	Normal (Mesh::*calc_face_normal_fh)(FaceHandle) const = &Mesh::calc_face_normal;
//	Normal (Mesh::*calc_face_normal_pt)(const Point&, const Point&, const Point&) const = &Mesh::calc_face_normal;

	void  (Mesh::*calc_face_centroid_fh_point)(FaceHandle, Point&) const = &Mesh::calc_face_centroid;
	Point (Mesh::*calc_face_centroid_fh      )(FaceHandle        ) const = &Mesh::calc_face_centroid;

	//======================================================================
	//  Mesh Type
	//======================================================================

	class_<Mesh, bases<Connectivity> > class_mesh(_name);

	class_mesh

		//======================================================================
		//  KernelT
		//======================================================================

		.def("reserve", &Mesh::reserve)

		.def("vertex", vertex, return_value_policy<reference_existing_object>())
		.def("halfedge", halfedge, return_value_policy<reference_existing_object>())
		.def("edge", edge, return_value_policy<reference_existing_object>())
		.def("face", face, return_value_policy<reference_existing_object>())

		.def("handle", handle_v)
		.def("handle", handle_h)
		.def("handle", handle_e)
		.def("handle", handle_f)

		.def("vertex_handle", vertex_handle_uint)
		.def("halfedge_handle", halfedge_handle_uint)
		.def("edge_handle", edge_handle_uint)
		.def("face_handle", face_handle_uint)

		.def("clear", &Mesh::clear)
		.def("clean", &Mesh::clean)
		.def("garbage_collection", garbage_collection, garbage_collection_overloads())

		.def("n_vertices", &Mesh::n_vertices)
		.def("n_halfedges", &Mesh::n_halfedges)
		.def("n_edges", &Mesh::n_edges)
		.def("n_faces", &Mesh::n_faces)
		.def("vertices_empty", &Mesh::vertices_empty)
		.def("halfedges_empty", &Mesh::halfedges_empty)
		.def("edges_empty", &Mesh::edges_empty)
		.def("faces_empty", &Mesh::faces_empty)

		.def("halfedge_handle", halfedge_handle_vh)
		.def("set_halfedge_handle", set_halfedge_handle_vh_hh)

		.def("to_vertex_handle", &Mesh::to_vertex_handle)
		.def("from_vertex_handle", &Mesh::from_vertex_handle)
		.def("set_vertex_handle", &Mesh::set_vertex_handle)
		.def("face_handle", face_handle_hh)
		.def("set_face_handle", &Mesh::set_face_handle)
		.def("next_halfedge_handle", &Mesh::next_halfedge_handle)
		.def("set_next_halfedge_handle", &Mesh::set_next_halfedge_handle)
		.def("prev_halfedge_handle", prev_halfedge_handle_hh)
		.def("opposite_halfedge_handle", &Mesh::opposite_halfedge_handle)
		.def("ccw_rotated_halfedge_handle", &Mesh::ccw_rotated_halfedge_handle)
		.def("cw_rotated_halfedge_handle", &Mesh::cw_rotated_halfedge_handle)
		.def("edge_handle", edge_handle_hh)

		.def("halfedge_handle", halfedge_handle_eh_uint)

		.def("halfedge_handle", halfedge_handle_fh)
		.def("set_halfedge_handle", set_halfedge_handle_fh_hh)

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

		.def("add_property", add_property_vph)
		.def("add_property", add_property_vph_str)
		.def("add_property", add_property_eph)
		.def("add_property", add_property_eph_str)
		.def("add_property", add_property_hph)
		.def("add_property", add_property_hph_str)
		.def("add_property", add_property_fph)
		.def("add_property", add_property_fph_str)

		.def("remove_property", remove_property_vph)
		.def("remove_property", remove_property_eph)
		.def("remove_property", remove_property_hph)
		.def("remove_property", remove_property_fph)

		.def("get_property_handle", get_property_handle_vph)
		.def("get_property_handle", get_property_handle_eph)
		.def("get_property_handle", get_property_handle_hph)
		.def("get_property_handle", get_property_handle_fph)

		.def("property", property_vertex, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("property", property_edge, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("property", property_halfedge, OPENMESH_PYTHON_DEFAULT_POLICY)
		.def("property", property_face, OPENMESH_PYTHON_DEFAULT_POLICY)

		.def("set_property", set_property_vertex)
		.def("set_property", set_property_edge)
		.def("set_property", set_property_halfedge)
		.def("set_property", set_property_face)

		.def("new_vertex", new_vertex_void)
		.def("new_vertex", new_vertex_point)
		.def("new_edge", &Mesh::new_edge)
		.def("new_face", new_face_void)
		.def("new_face", new_face_face)

		.def("vertices", vertices)
		.def("halfedges", halfedges)
		.def("edges", edges)
		.def("faces", faces)

		.def("svertices", svertices)
		.def("shalfedges", shalfedges)
		.def("sedges", sedges)
		.def("sfaces", sfaces)

		//======================================================================
		//  BaseKernel
		//======================================================================

		.def("copy_all_properties", copy_all_properties_vh_vh)
		.def("copy_all_properties", copy_all_properties_eh_eh)
		.def("copy_all_properties", copy_all_properties_hh_hh)
		.def("copy_all_properties", copy_all_properties_fh_fh)

		.def("copy_all_properties", copy_all_properties_vh_vh_bool)
		.def("copy_all_properties", copy_all_properties_eh_eh_bool)
		.def("copy_all_properties", copy_all_properties_hh_hh_bool)
		.def("copy_all_properties", copy_all_properties_fh_fh_bool)

		//======================================================================
		//  PolyConnectivity
		//======================================================================

		.def("vv", vv)
		.def("vih", vih)
		.def("voh", voh)
		.def("ve", ve)
		.def("vf", vf)

		.def("fv", fv)
		.def("fh", fh)
		.def("fe", fe)
		.def("ff", ff)

		.def("hl", hl)

		//======================================================================
		//  PolyMeshT
		//======================================================================

		.def("add_vertex", &Mesh::add_vertex)

		.def("calc_edge_vector", calc_edge_vector_eh_normal)
		.def("calc_edge_vector", calc_edge_vector_eh)
		.def("calc_edge_vector", calc_edge_vector_hh_normal)
		.def("calc_edge_vector", calc_edge_vector_hh)

		.def("calc_edge_length", calc_edge_length_eh)
		.def("calc_edge_length", calc_edge_length_hh)
		.def("calc_edge_sqr_length", calc_edge_sqr_length_eh)
		.def("calc_edge_sqr_length", calc_edge_sqr_length_hh)

		.def("calc_sector_vectors", &Mesh::calc_sector_vectors)
		.def("calc_sector_angle", &Mesh::calc_sector_angle)
		.def("calc_sector_normal", &Mesh::calc_sector_normal)
		.def("calc_sector_area", &Mesh::calc_sector_area)

		.def("calc_dihedral_angle_fast", calc_dihedral_angle_fast_hh)
		.def("calc_dihedral_angle_fast", calc_dihedral_angle_fast_eh)
		.def("calc_dihedral_angle", calc_dihedral_angle_hh)
		.def("calc_dihedral_angle", calc_dihedral_angle_eh)

//		.def("find_feature_edges", &Mesh::find_feature_edges, find_feature_edges_overloads())

//		.def("split", split_fh_point)
		.def("split", split_fh_vh)
//		.def("split", split_eh_point)
		.def("split", split_eh_vh)

		.def("update_normals", &Mesh::update_normals)
		.def("update_normal", update_normal_fh)
		.def("update_face_normals", &Mesh::update_face_normals)

		.def("calc_face_normal", calc_face_normal_fh)
//		.def("calc_face_normal", calc_face_normal_pt)

		.def("calc_face_centroid", calc_face_centroid_fh_point)
		.def("calc_face_centroid", calc_face_centroid_fh)

//		.def("update_normal", &Mesh::update_normal, update_normal_overloads())
		.def("update_halfedge_normals", &Mesh::update_halfedge_normals, update_halfedge_normals_overloads())

		.def("calc_halfedge_normal", &Mesh::calc_halfedge_normal, calc_halfedge_normal_overloads())

		.def("is_estimated_feature_edge", &Mesh::is_estimated_feature_edge)

		.def("update_normal", update_normal_vh)
		.def("update_vertex_normals", &Mesh::update_vertex_normals)

//		.def("calc_vertex_normal", &Mesh::calc_vertex_normal)
//		.def("calc_vertex_normal_fast", &Mesh::calc_vertex_normal_fast)
//		.def("calc_vertex_normal_correct", &Mesh::calc_vertex_normal_correct)
//		.def("calc_vertex_normal_loop", &Mesh::calc_vertex_normal_loop)

		.def("is_polymesh", &Mesh::is_polymesh)
		.staticmethod("is_polymesh")

		.def("is_trimesh", &Mesh::is_trimesh)
		.staticmethod("is_trimesh")
		;

	//======================================================================
	//  Nested Types
	//======================================================================

	// Enter mesh scope
	scope scope_mesh = class_mesh;

	// Point
	const type_info point_info = type_id<typename Mesh::Point>();
	const converter::registration * point_registration = converter::registry::query(point_info);
	scope_mesh.attr("Point") = handle<>(point_registration->m_class_object);

	// Normal
	const type_info normal_info = type_id<typename Mesh::Normal>();
	const converter::registration * normal_registration = converter::registry::query(normal_info);
	scope_mesh.attr("Normal") = handle<>(normal_registration->m_class_object);

	// Color
	const type_info color_info = type_id<typename Mesh::Color>();
	const converter::registration * color_registration = converter::registry::query(color_info);
	scope_mesh.attr("Color") = handle<>(color_registration->m_class_object);

	// TexCoord2D
	const type_info texcoord2d_info = type_id<typename Mesh::TexCoord2D>();
	const converter::registration * texcoord2d_registration = converter::registry::query(texcoord2d_info);
	scope_mesh.attr("TexCoord2D") = handle<>(texcoord2d_registration->m_class_object);

	// TexCoord3D
	const type_info texcoord3d_info = type_id<typename Mesh::TexCoord3D>();
	const converter::registration * texcoord3d_registration = converter::registry::query(texcoord3d_info);
	scope_mesh.attr("TexCoord3D") = handle<>(texcoord3d_registration->m_class_object);
}

} // namespace OpenMesh
} // namespace Python

#endif

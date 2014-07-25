#ifndef OPENMESH_PYTHON_MESH_HH
#define OPENMESH_PYTHON_MESH_HH

#include "Python/Bindings.hh"
#include "Python/Iterator.hh"
#include "Python/Circulator.hh"

namespace OpenMesh {
namespace Python {

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(garbage_collection_overloads, garbage_collection, 0, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(update_halfedge_normals_overloads, update_halfedge_normals, 0, 1)


/**
 * Wrapper for meshes.
 *
 * This class template is used to wrap meshes for %Python. It inherits from it's
 * template parameter (a mesh type) and implements functions that are required
 * by %Python but not provided by mesh types.
 *
 * @tparam Mesh A mesh type.
 */
template <class Mesh>
class MeshWrapperT : public Mesh {
	public:

		/**
		 * Set the status of an item.
		 *
		 * Depending on @ref OPENMESH_PYTHON_DEFAULT_POLICY, Mesh::status may
		 * return by value instead of reference. This function ensures that the
		 * status of an item can be changed nonetheless.
		 */
		template <class IndexHandle>
		void set_status(IndexHandle _h, const OpenMesh::Attributes::StatusInfo& _info) {
			Mesh::status(_h) = _info;
		}

		/**
		 * Thin wrapper for Mesh::add_property.
		 *
		 * @tparam PropHandle A property handle type.
		 *
		 * This wrapper function is required because Mesh::add_property has
		 * default arguments and therefore cannot be exposed directly.
		 */
		template <class PropHandle>
		void add_property_ph(PropHandle& _ph) {
			Mesh::add_property(_ph);
		}

		/**
		 * Thin wrapper for Mesh::add_property.
		 *
		 * @tparam PropHandle A property handle type.
		 *
		 * This wrapper function is required because Mesh::add_property has
		 * default arguments and therefore cannot be exposed directly.
		 */
		template <class PropHandle>
		void add_property_ph_str(PropHandle& _ph, const std::string& _name) {
			Mesh::add_property(_ph, _name);
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
		template <class PropHandle, class IndexHandle>
		void set_property(PropHandle _ph, IndexHandle _h, const object& _value) {
			Mesh::property(_ph, _h) = _value;
		}

		/**
		 * Thin wrapper for Mesh::copy_all_properties.
		 *
		 * @tparam IndexHandle A mesh item handle type.
		 *
		 * This wrapper function is required because Mesh::copy_all_properties
		 * has default arguments and therefore cannot be exposed directly.
		 */
		template <class IndexHandle>
		void copy_all_properties_ih_ih(IndexHandle _from, IndexHandle _to) {
			Mesh::copy_all_properties(_from, _to);
		}

		/**
		 * Thin wrapper for Mesh::copy_all_properties.
		 *
		 * @tparam IndexHandle A mesh item handle type.
		 *
		 * This wrapper function is required because Mesh::copy_all_properties
		 * has default arguments and therefore cannot be exposed directly.
		 */
		template <class IndexHandle>
		void copy_all_properties_ih_ih_bool(IndexHandle _from, IndexHandle _to, bool _copyBuildIn) {
			Mesh::copy_all_properties(_from, _to, _copyBuildIn);
		}

		/**
		 * Get a vertex iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices> vertices() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices>(*this, VertexHandle(0));
		}

		/**
		 * Get a halfedge iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges> halfedges() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges>(*this, HalfedgeHandle(0));
		}

		/**
		 * Get an edge iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges> edges() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges>(*this, EdgeHandle(0));
		}

		/**
		 * Get a face iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces> faces() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces>(*this, FaceHandle(0));
		}

		/**
		 * Get a skipping vertex iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices> svertices() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::VertexIter, &OpenMesh::ArrayKernel::n_vertices>(*this, VertexHandle(0), true);
		}

		/**
		 * Get a skipping halfedge iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges> shalfedges() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::HalfedgeIter, &OpenMesh::ArrayKernel::n_halfedges>(*this, HalfedgeHandle(0), true);
		}

		/**
		 * Get a skipping edge iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges> sedges() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::EdgeIter, &OpenMesh::ArrayKernel::n_edges>(*this, EdgeHandle(0), true);
		}

		/**
		 * Get a skipping face iterator.
		 */
		IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces> sfaces() const {
			return IteratorWrapperT<OpenMesh::PolyConnectivity::FaceIter, &OpenMesh::ArrayKernel::n_faces>(*this, FaceHandle(0), true);
		}

		/**
		 * Get a vertex-vertex circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexVertexIter, VertexHandle> vv(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexVertexIter, VertexHandle>(*this, _handle);
		}

		/**
		 * Get a vertex-(incoming)halfedge circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexIHalfedgeIter, VertexHandle> vih(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexIHalfedgeIter, VertexHandle>(*this, _handle);
		}

		/**
		 * Get a vertex-(outgoing)halfedge circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexOHalfedgeIter, VertexHandle> voh(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexOHalfedgeIter, VertexHandle>(*this, _handle);
		}

		/**
		 * Get a vertex-edge circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexEdgeIter, VertexHandle> ve(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexEdgeIter, VertexHandle>(*this, _handle);
		}

		/**
		 * Get a vertex-face circulator.
		 */
		CirculatorWrapperT<typename Mesh::VertexFaceIter, VertexHandle> vf(VertexHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::VertexFaceIter, VertexHandle>(*this, _handle);
		}

		/**
		 * Get a face-vertex circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceVertexIter, FaceHandle> fv(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceVertexIter, FaceHandle>(*this, _handle);
		}

		/**
		 * Get a face-halfedge circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceHalfedgeIter, FaceHandle> fh(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceHalfedgeIter, FaceHandle>(*this, _handle);
		}

		/**
		 * Get a face-edge circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceEdgeIter, FaceHandle> fe(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceEdgeIter, FaceHandle>(*this, _handle);
		}

		/**
		 * Get a face-face circulator.
		 */
		CirculatorWrapperT<typename Mesh::FaceFaceIter, FaceHandle> ff(FaceHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::FaceFaceIter, FaceHandle>(*this, _handle);
		}

		/**
		 * Get a halfedge-loop circulator.
		 */
		CirculatorWrapperT<typename Mesh::HalfedgeLoopIter, HalfedgeHandle> hl(HalfedgeHandle _handle) const {
			return CirculatorWrapperT<typename Mesh::HalfedgeLoopIter, HalfedgeHandle>(*this, _handle);
		}
};

/**
 * Expose a mesh type to %Python.
 *
 * @tparam Mesh A mesh type.
 * @tparam Connectivity The appropriate connectivity type (e.g. TriConnectivity
 * for triangle mesh types).
 *
 * @param _name The name of the mesh type to be exposed.
 *
 * @note Meshes are wrapped by MeshWrapperT before they are exposed to %Python,
 * i.e. they are not exposed directly.
 */
template <class Mesh, class Connectivity>
void expose_mesh(const char *_name) {
	using OpenMesh::Attributes::StatusInfo;

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
	void (Mesh::*set_status_vh)(VertexHandle,   const StatusInfo&) = &Mesh::set_status;
	void (Mesh::*set_status_hh)(HalfedgeHandle, const StatusInfo&) = &Mesh::set_status;
	void (Mesh::*set_status_eh)(EdgeHandle,     const StatusInfo&) = &Mesh::set_status;
	void (Mesh::*set_status_fh)(FaceHandle,     const StatusInfo&) = &Mesh::set_status;

	// Property management - add property (1/2)
	void (Mesh::*add_property_vph)(VPropHandleT<object>&) = &Mesh::add_property_ph;
	void (Mesh::*add_property_eph)(EPropHandleT<object>&) = &Mesh::add_property_ph;
	void (Mesh::*add_property_hph)(HPropHandleT<object>&) = &Mesh::add_property_ph;
	void (Mesh::*add_property_fph)(FPropHandleT<object>&) = &Mesh::add_property_ph;

	// Property management - add property (2/2)
	void (Mesh::*add_property_vph_str)(VPropHandleT<object>&, const std::string&) = &Mesh::add_property_ph_str;
	void (Mesh::*add_property_eph_str)(EPropHandleT<object>&, const std::string&) = &Mesh::add_property_ph_str;
	void (Mesh::*add_property_hph_str)(HPropHandleT<object>&, const std::string&) = &Mesh::add_property_ph_str;
	void (Mesh::*add_property_fph_str)(FPropHandleT<object>&, const std::string&) = &Mesh::add_property_ph_str;

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
	void (Mesh::*set_property_vertex  )(VPropHandleT<object>, VertexHandle,   const object&) = &Mesh::set_property;
	void (Mesh::*set_property_edge    )(EPropHandleT<object>, EdgeHandle,     const object&) = &Mesh::set_property;
	void (Mesh::*set_property_halfedge)(HPropHandleT<object>, HalfedgeHandle, const object&) = &Mesh::set_property;
	void (Mesh::*set_property_face    )(FPropHandleT<object>, FaceHandle,     const object&) = &Mesh::set_property;

	// Low-level adding new items
	VertexHandle (Mesh::*new_vertex_void )(void                        ) = &Mesh::new_vertex;
	VertexHandle (Mesh::*new_vertex_point)(const typename Mesh::Point& ) = &Mesh::new_vertex;
	FaceHandle   (Mesh::*new_face_void   )(void                        ) = &Mesh::new_face;
	FaceHandle   (Mesh::*new_face_face   )(const typename Mesh::Face&  ) = &Mesh::new_face;

	// Copy all properties (1/2)
	void (Mesh::*copy_all_properties_vh_vh)(VertexHandle,   VertexHandle  ) = &Mesh::copy_all_properties_ih_ih;
	void (Mesh::*copy_all_properties_eh_eh)(EdgeHandle,     EdgeHandle    ) = &Mesh::copy_all_properties_ih_ih;
	void (Mesh::*copy_all_properties_hh_hh)(HalfedgeHandle, HalfedgeHandle) = &Mesh::copy_all_properties_ih_ih;
	void (Mesh::*copy_all_properties_fh_fh)(FaceHandle,     FaceHandle    ) = &Mesh::copy_all_properties_ih_ih;

	// Copy all properties (2/2)
	void (Mesh::*copy_all_properties_vh_vh_bool)(VertexHandle,   VertexHandle,   bool) = &Mesh::copy_all_properties_ih_ih_bool;
	void (Mesh::*copy_all_properties_eh_eh_bool)(EdgeHandle,     EdgeHandle,     bool) = &Mesh::copy_all_properties_ih_ih_bool;
	void (Mesh::*copy_all_properties_hh_hh_bool)(HalfedgeHandle, HalfedgeHandle, bool) = &Mesh::copy_all_properties_ih_ih_bool;
	void (Mesh::*copy_all_properties_fh_fh_bool)(FaceHandle,     FaceHandle,     bool) = &Mesh::copy_all_properties_ih_ih_bool;

	typename Mesh::Scalar (Mesh::*calc_dihedral_angle_hh)(HalfedgeHandle) const = &Mesh::calc_dihedral_angle;
	typename Mesh::Scalar (Mesh::*calc_dihedral_angle_eh)(EdgeHandle    ) const = &Mesh::calc_dihedral_angle;

	class_<Mesh, bases<Connectivity> > class_mesh(_name);

	// Enter mesh scope
	scope scope_mesh = class_mesh;

	// Expose nested type: Mesh::Point
	const type_info point_info = type_id<typename Mesh::Point>();
	const converter::registration * point_registration = converter::registry::query(point_info);
	scope_mesh.attr("Point") = handle<>(point_registration->m_class_object);

	// Expose nested type: Mesh::Normal
	const type_info normal_info = type_id<typename Mesh::Normal>();
	const converter::registration * normal_registration = converter::registry::query(normal_info);
	scope_mesh.attr("Normal") = handle<>(normal_registration->m_class_object);

	// Expose nested type: Mesh::TexCoord2D
	const type_info texcoord2d_info = type_id<typename Mesh::TexCoord2D>();
	const converter::registration * texcoord2d_registration = converter::registry::query(texcoord2d_info);
	scope_mesh.attr("TexCoord2D") = handle<>(texcoord2d_registration->m_class_object);

	// Expose nested type: Mesh::TexCoord3D
	const type_info texcoord3d_info = type_id<typename Mesh::TexCoord3D>();
	const converter::registration * texcoord3d_registration = converter::registry::query(texcoord3d_info);
	scope_mesh.attr("TexCoord3D") = handle<>(texcoord3d_registration->m_class_object);

	// Expose nested type: Mesh::Color
	const type_info color_info = type_id<typename Mesh::Color>();
	const converter::registration * color_registration = converter::registry::query(color_info);
	scope_mesh.attr("Color") = handle<>(color_registration->m_class_object);

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

		.def("vertices", &Mesh::vertices)
		.def("halfedges", &Mesh::halfedges)
		.def("edges", &Mesh::edges)
		.def("faces", &Mesh::faces)

		.def("svertices", &Mesh::svertices)
		.def("shalfedges", &Mesh::shalfedges)
		.def("sedges", &Mesh::sedges)
		.def("sfaces", &Mesh::sfaces)

		.def("vv", &Mesh::vv)
		.def("vih", &Mesh::vih)
		.def("voh", &Mesh::voh)
		.def("ve", &Mesh::ve)
		.def("vf", &Mesh::vf)

		.def("fv", &Mesh::fv)
		.def("fh", &Mesh::fh)
		.def("fe", &Mesh::fe)
		.def("ff", &Mesh::ff)

		.def("hl", &Mesh::hl)

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
		//  PolyMeshT
		//======================================================================

		.def("update_normals", &Mesh::update_normals)
		.def("update_vertex_normals", &Mesh::update_vertex_normals)
		.def("update_halfedge_normals", &Mesh::update_halfedge_normals, update_halfedge_normals_overloads())
		.def("update_face_normals", &Mesh::update_face_normals)

		.def("calc_dihedral_angle", calc_dihedral_angle_hh)
		.def("calc_dihedral_angle", calc_dihedral_angle_eh)



		.def("add_vertex", &Mesh::add_vertex)
		;
}

} // namespace OpenMesh
} // namespace Python

#endif

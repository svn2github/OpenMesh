#ifndef OPENMESH_PYTHON_MESH_HH
#define OPENMESH_PYTHON_MESH_HH

#include "Python/Bindings.hh"
#include "Python/Iterator.hh"
#include "Python/Circulator.hh"

namespace OpenMesh {
namespace Python {

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

} // namespace OpenMesh
} // namespace Python

#endif
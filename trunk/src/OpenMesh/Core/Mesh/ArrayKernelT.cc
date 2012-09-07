/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         * 
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/ 

/*===========================================================================*\
 *                                                                           *             
 *   $Revision: 362 $                                                         *
 *   $Date: 2011-01-26 10:21:12 +0100 (Mi, 26 Jan 2011) $                   *
 *                                                                           *
\*===========================================================================*/

#define OPENMESH_ARRAY_KERNEL_C

//== INCLUDES =================================================================

#include <OpenMesh/Core/Mesh/ArrayKernel.hh>

//== NAMESPACES ===============================================================

namespace OpenMesh
{

//== IMPLEMENTATION ==========================================================

template<typename std_API_Container_VHandlePointer,
         typename std_API_Container_HHandlePointer,
         typename std_API_Container_FHandlePointer>
void ArrayKernel::garbage_collection(std_API_Container_VHandlePointer& vh_to_update,
                                     std_API_Container_HHandlePointer& hh_to_update,
                                     std_API_Container_FHandlePointer& fh_to_update,
                                     bool _v, bool _e, bool _f)
{

#ifdef DEBUG
  #ifndef OM_GARBAGE_NO_STATUS_WARNING
    if ( !this->has_vertex_status() )
      omerr() << "garbage_collection: No vertex status available. You can request it: mesh.request_vertex_status() or define OM_GARBAGE_NO_STATUS_WARNING to silence this warning." << std::endl;
    if ( !this->has_edge_status() )
      omerr() << "garbage_collection: No edge status available. You can request it: mesh.request_edge_status() or define OM_GARBAGE_NO_STATUS_WARNING to silence this warning." << std::endl;
    if ( !this->has_face_status() )
      omerr() << "garbage_collection: No face status available. You can request it: mesh.request_face_status() or define OM_GARBAGE_NO_STATUS_WARNING to silence this warning." << std::endl;
  #endif
#endif

  int i, i0, i1, nV(n_vertices()), nE(n_edges()), nH(2*n_edges()), nF(n_faces());

  std::vector<VertexHandle>    vh_map;
  std::vector<HalfedgeHandle>  hh_map;
  std::vector<FaceHandle>      fh_map;

  // setup handle mapping:
  vh_map.reserve(nV);
  for (i=0; i<nV; ++i) vh_map.push_back(VertexHandle(i));

  hh_map.reserve(nH);
  for (i=0; i<nH; ++i) hh_map.push_back(HalfedgeHandle(i));

  fh_map.reserve(nF);
  for (i=0; i<nF; ++i) fh_map.push_back(FaceHandle(i));

  // remove deleted vertices
  if (_v && n_vertices() > 0 && this->has_vertex_status() )
  {
    i0=0;  i1=nV-1;

    while (1)
    {
      // find 1st deleted and last un-deleted
      while (!status(VertexHandle(i0)).deleted() && i0 < i1)  ++i0;
      while ( status(VertexHandle(i1)).deleted() && i0 < i1)  --i1;
      if (i0 >= i1) break;

      // swap
      std::swap(vertices_[i0], vertices_[i1]);
      std::swap(vh_map[i0],  vh_map[i1]);
      vprops_swap(i0, i1);
    };

    vertices_.resize(status(VertexHandle(i0)).deleted() ? i0 : i0+1);
    vprops_resize(n_vertices());
  }


  // remove deleted edges
  if (_e && n_edges() > 0 && this->has_edge_status() )
  {
    i0=0;  i1=nE-1;

    while (1)
    {
      // find 1st deleted and last un-deleted
      while (!status(EdgeHandle(i0)).deleted() && i0 < i1)  ++i0;
      while ( status(EdgeHandle(i1)).deleted() && i0 < i1)  --i1;
      if (i0 >= i1) break;

      // swap
      std::swap(edges_[i0], edges_[i1]);
      std::swap(hh_map[2*i0], hh_map[2*i1]);
      std::swap(hh_map[2*i0+1], hh_map[2*i1+1]);
      eprops_swap(i0, i1);
      hprops_swap(2*i0,   2*i1);
      hprops_swap(2*i0+1, 2*i1+1);
    };

    edges_.resize(status(EdgeHandle(i0)).deleted() ? i0 : i0+1);
    eprops_resize(n_edges());
    hprops_resize(n_halfedges());
  }


  // remove deleted faces
  if (_f && n_faces() > 0 && this->has_face_status() )
  {
    i0=0;  i1=nF-1;

    while (1)
    {
      // find 1st deleted and last un-deleted
      while (!status(FaceHandle(i0)).deleted() && i0 < i1)  ++i0;
      while ( status(FaceHandle(i1)).deleted() && i0 < i1)  --i1;
      if (i0 >= i1) break;

      // swap
      std::swap(faces_[i0], faces_[i1]);
      std::swap(fh_map[i0], fh_map[i1]);
      fprops_swap(i0, i1);
    };

    faces_.resize(status(FaceHandle(i0)).deleted() ? i0 : i0+1);
    fprops_resize(n_faces());
  }


  // update handles of vertices
  if (_e)
  {
    KernelVertexIter v_it(vertices_begin()), v_end(vertices_end());
    VertexHandle     vh;

    for (; v_it!=v_end; ++v_it)
    {
      vh = handle(*v_it);
      if (!is_isolated(vh))
      {
        set_halfedge_handle(vh, hh_map[halfedge_handle(vh).idx()]);
      }
    }
  }

  HalfedgeHandle hh;
  // update handles of halfedges
  for (KernelEdgeIter e_it(edges_begin()); e_it != edges_end(); ++e_it)
  {//in the first pass update the (half)edges vertices
    hh = halfedge_handle(handle(*e_it), 0);
    set_vertex_handle(hh, vh_map[to_vertex_handle(hh).idx()]);
    hh = halfedge_handle(handle(*e_it), 1);
    set_vertex_handle(hh, vh_map[to_vertex_handle(hh).idx()]);
  }
  for (KernelEdgeIter e_it(edges_begin()); e_it != edges_end(); ++e_it)
  {//in the second pass update the connectivity of the (half)edges
    hh = halfedge_handle(handle(*e_it), 0);
    set_next_halfedge_handle(hh, hh_map[next_halfedge_handle(hh).idx()]);
    if (!is_boundary(hh))
    {
      set_face_handle(hh, fh_map[face_handle(hh).idx()]);
    }
    hh = halfedge_handle(handle(*e_it), 1);
    set_next_halfedge_handle(hh, hh_map[next_halfedge_handle(hh).idx()]);
    if (!is_boundary(hh))
    {
      set_face_handle(hh, fh_map[face_handle(hh).idx()]);
    }
  }

  // update handles of faces
  if (_e)
  {
    KernelFaceIter  f_it(faces_begin()), f_end(faces_end());
    FaceHandle      fh;

    for (; f_it!=f_end; ++f_it)
    {
      fh = handle(*f_it);
      set_halfedge_handle(fh, hh_map[halfedge_handle(fh).idx()]);
    }
  }

  const int vertexCount   = vertices_.size();
  const int halfedgeCount = edges_.size() * 2;
  const int faceCount     = faces_.size();

  // Update the vertex handles in the vertex handle vector
  typename std_API_Container_VHandlePointer::iterator v_it(vh_to_update.begin()), v_it_end(vh_to_update.end());
  for(; v_it != v_it_end; ++v_it)
  {
    if ( (*v_it)->idx() >=  vertexCount )
      (*v_it)->invalidate();
    else
      *(*v_it) = vh_map[(*v_it)->idx()];
  }

  // Update the halfedge handles in the halfedge handle vector
  typename std_API_Container_HHandlePointer::iterator hh_it(hh_to_update.begin()), hh_it_end(hh_to_update.end());
  for(; hh_it != hh_it_end; ++hh_it)
  {
    if ( (*hh_it)->idx() >=  halfedgeCount )
      (*hh_it)->invalidate();
    else
      *(*hh_it) = hh_map[(*hh_it)->idx()];
  }
  // Update the face handles in the face handle vector
  typename std_API_Container_FHandlePointer::iterator fh_it(fh_to_update.begin()), fh_it_end(fh_to_update.end());
  for(; fh_it != fh_it_end; ++fh_it)
  {
    if ( (*fh_it)->idx() >=  faceCount )
      (*fh_it)->invalidate();
    else
      *(*fh_it) = fh_map[(*fh_it)->idx()];
  }
}

}


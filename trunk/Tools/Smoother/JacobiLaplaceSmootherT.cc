//=============================================================================
//                                                                            
//                               OpenMesh                                     
//        Copyright (C) 2003 by Computer Graphics Group, RWTH Aachen          
//                           www.openmesh.org                                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This library is free software; you can redistribute it and/or modify it 
//   under the terms of the GNU Lesser General Public License as published   
//   by the Free Software Foundation, version 2.1.                           
//                                                                             
//   This library is distributed in the hope that it will be useful, but       
//   WITHOUT ANY WARRANTY; without even the implied warranty of                
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
//   Lesser General Public License for more details.                           
//                                                                            
//   You should have received a copy of the GNU Lesser General Public          
//   License along with this library; if not, write to the Free Software       
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//   $Revision$
//   $Date$
//                                                                            
//=============================================================================

/** \file JacobiLaplaceSmootherT.cc
    
 */

//=============================================================================
//
//  CLASS JacobiLaplaceSmootherT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_JACOBI_LAPLACE_SMOOTHERT_C

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Smoother {


//== IMPLEMENTATION ========================================================== 


template <class Mesh>
void
JacobiLaplaceSmootherT<Mesh>::
smooth(unsigned int _n)
{
  if (Base::continuity() > Base::C0)
  {
    Base::mesh_.add_property(umbrellas_);
    if (Base::continuity() > Base::C1)
      Base::mesh_.add_property(squared_umbrellas_);
  }

  LaplaceSmootherT<Mesh>::smooth(_n);

  if (Base::continuity() > Base::C0)
  {
    Base::mesh_.remove_property(umbrellas_);
    if (Base::continuity() > Base::C1)
      Base::mesh_.remove_property(squared_umbrellas_);
  }
}


//-----------------------------------------------------------------------------


template <class Mesh>
void
JacobiLaplaceSmootherT<Mesh>::
compute_new_positions_C0()
{
  typename Mesh::VertexIter  v_it, v_end(Base::mesh_.vertices_end());
  typename Mesh::CVVIter     vv_it;
  typename Mesh::Normal      u, p, zero(0,0,0);
  typename Mesh::Scalar      w;

  for (v_it=Base::mesh_.vertices_begin(); v_it!=v_end; ++v_it)
  {
    if (is_active(v_it))
    {
      // compute umbrella
      u = zero;
      for (vv_it=Base::mesh_.cvv_iter(v_it); vv_it; ++vv_it)
      {
        w = weight(Base::mesh_.edge_handle(vv_it.current_halfedge_handle()));
        u += vector_cast<typename Mesh::Normal>(Base::mesh_.point(vv_it)) * w;
      }
      u *= weight(v_it);
      u -= vector_cast<typename Mesh::Normal>(Base::mesh_.point(v_it));

      // damping
      u *= 0.5;
    
      // store new position
      p  = vector_cast<typename Mesh::Normal>(Base::mesh_.point(v_it));
      p += u;
      set_new_position(v_it, p);
    }
  }
}


//-----------------------------------------------------------------------------


template <class Mesh>
void
JacobiLaplaceSmootherT<Mesh>::
compute_new_positions_C1()
{
  typename Mesh::VertexIter  v_it, v_end(Base::mesh_.vertices_end());
  typename Mesh::CVVIter     vv_it;
  typename Mesh::Normal      u, uu, p, zero(0,0,0);
  typename Mesh::Scalar      w, diag;


  // 1st pass: compute umbrellas
  for (v_it=Base::mesh_.vertices_begin(); v_it!=v_end; ++v_it)
  {
    u = zero;
    for (vv_it=Base::mesh_.cvv_iter(v_it); vv_it; ++vv_it)
    {
      w  = weight(Base::mesh_.edge_handle(vv_it.current_halfedge_handle()));
      u -= vector_cast<typename Mesh::Normal>(Base::mesh_.point(vv_it))*w;
    }
    u *= weight(v_it);
    u += vector_cast<typename Mesh::Normal>(Base::mesh_.point(v_it));

    Base::mesh_.property(umbrellas_, v_it) = u;
  }


  // 2nd pass: compute updates
  for (v_it=Base::mesh_.vertices_begin(); v_it!=v_end; ++v_it)
  {
    if (is_active(v_it))
    {
      uu   = zero;
      diag = 0.0;   
      for (vv_it=Base::mesh_.cvv_iter(v_it); vv_it; ++vv_it)
      {
	w     = weight(Base::mesh_.edge_handle(vv_it.current_halfedge_handle()));
	uu   -= Base::mesh_.property(umbrellas_, vv_it);
	diag += (w * weight(vv_it) + 1.0) * w;
      }
      uu   *= weight(v_it);
      diag *= weight(v_it);
      uu   += Base::mesh_.property(umbrellas_, v_it);
      if (diag) uu *= 1.0/diag;

      // damping
      uu *= 0.25;
    
      // store new position
      p  = vector_cast<typename Mesh::Normal>(Base::mesh_.point(v_it));
      p -= uu;
      set_new_position(v_it, p);
    }
  }
}


//=============================================================================
} // namespace Smoother
} // namespace OpenMesh
//=============================================================================

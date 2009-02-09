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
//   $Revision: 1802 $
//   $Date: 2008-05-19 11:55:07 +0200 (Mo, 19. Mai 2008) $
//                                                                            
//=============================================================================

/** \file JacobiLaplaceSmootherT.hh
    
 */


//=============================================================================
//
//  CLASS JacobiLaplaceSmootherT
//
//=============================================================================

#ifndef OPENMESH_JACOBI_LAPLACE_SMOOTHERT_HH
#define OPENMESH_JACOBI_LAPLACE_SMOOTHERT_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Smoother/LaplaceSmootherT.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Smoother {

//== CLASS DEFINITION =========================================================

/** Laplacian Smoothing.
 *
 */
template <class Mesh>
class JacobiLaplaceSmootherT : public LaplaceSmootherT<Mesh>
{
private:
  typedef LaplaceSmootherT<Mesh>            Base;
  
public:

  JacobiLaplaceSmootherT( Mesh& _mesh ) : LaplaceSmootherT<Mesh>(_mesh) {}

  // override: alloc umbrellas
  void smooth(unsigned int _n);


protected:

  virtual void compute_new_positions_C0();
  virtual void compute_new_positions_C1();


private:

  OpenMesh::VPropHandleT<typename Mesh::Normal>   umbrellas_;
  OpenMesh::VPropHandleT<typename Mesh::Normal>   squared_umbrellas_;
};


//=============================================================================
} // namespace Smoother
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_JACOBI_LAPLACE_SMOOTHERT_C)
#define OPENMESH_JACOBI_LAPLACE_SMOOTHERT_TEMPLATES
#include "JacobiLaplaceSmootherT.cc"
#endif
//=============================================================================
#endif // OPENMESH_JACOBI_LAPLACE_SMOOTHERT_HH defined
//=============================================================================


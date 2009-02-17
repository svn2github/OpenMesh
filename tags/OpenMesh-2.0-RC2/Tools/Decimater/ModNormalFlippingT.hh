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

/** \file ModNormalFlippingT.hh
    
 */

//=============================================================================
//
//  CLASS ModNormalFlipping
//
//=============================================================================


#ifndef OPENMESH_DECIMATER_MODNORMALFLIPPING_HH
#define OPENMESH_DECIMATER_MODNORMALFLIPPING_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModBaseT.hh>

//== NAMESPACES ===============================================================

namespace OpenMesh { // BEGIN_NS_OPENMESH
namespace Decimater { // BEGIN_NS_DECIMATER


//== CLASS DEFINITION =========================================================

/** Decimating module to avoid flipping of faces.
 *  
 *  This module can be used only as a binary module. The criterion
 *  of allowing/disallowing the collapse is the angular deviation between
 *  the face normal of the orignal faces and normals of the faces after the
 *  collapse. The collapse will pass the test, if the deviation is below
 *  a given threshold.
 */	      
template <typename DecimaterT>
class ModNormalFlippingT : public ModBaseT< DecimaterT >
{ 
public:

  DECIMATING_MODULE( ModNormalFlippingT, DecimaterT, NormalFlipping );

public:
  
  /// Constructor
  ModNormalFlippingT( DecimaterT &_dec) : Base(_dec, true)
  {
    set_max_normal_deviation( 90.0f );
  }
  

  ~ModNormalFlippingT() 
  { }
  

public:
  
  /** Compute collapse priority due to angular deviation of face normals
   *  before and after a collapse.
   *
   *  -# Compute for each adjacent face of \c _ci.v0 the face
   *  normal if the collpase would be executed.  
   *
   *  -# Prevent the collapse, if the angle between the original and the
   *  new normal is below a given threshold.
   *  
   *  \param _ci The collapse description
   *  \return LEGAL_COLLAPSE or ILLEGAL_COLLAPSE
   *
   *  \see set_max_normal_deviation()
   */
  float collapse_priority(const CollapseInfo& _ci)
  {
    // simulate collapse
    Base::mesh().set_point(_ci.v0, _ci.p1);
    
    // check for flipping normals
    typename Mesh::ConstVertexFaceIter vf_it(Base::mesh(), _ci.v0);
    typename Mesh::FaceHandle          fh;
    typename Mesh::Scalar              c(1.0);
    
    for (; vf_it; ++vf_it) 
    {
      fh = vf_it.handle();
      if (fh != _ci.fl && fh != _ci.fr)
      {
        typename Mesh::Normal n1 = Base::mesh().normal(fh);
        typename Mesh::Normal n2 = Base::mesh().calc_face_normal(fh);

        c = dot(n1, n2);
        
        if (c < min_cos_)
          break;
      }
    }
      
    // undo simulation changes
    Base::mesh().set_point(_ci.v0, _ci.p0);

    return float( (c < min_cos_) ? Base::ILLEGAL_COLLAPSE : Base::LEGAL_COLLAPSE );
  }



public:
   
  /// get normal deviation
  float max_normal_deviation() const { return max_deviation_ / M_PI * 180.0; }
  
  /// \deprecated
  float normal_deviation() const { return max_normal_deviation(); }
  
  /** Set normal deviation
   *  
   *  Set the maximum angular deviation of the orignal normal and the new
   *  normal in degrees.
   */
  void set_max_normal_deviation(float _f) { 
    max_deviation_ = _f / 180.0 * M_PI; 
    min_cos_       = cos(max_deviation_);
  }

  /// \deprecated
  void set_normal_deviation(float _f) 
  { set_max_normal_deviation(_f); }
  
private:

  // hide this method
  void set_binary(bool _b) {}
   
private:

  // maximum normal deviation
  double max_deviation_, min_cos_;
};


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#endif // OPENACG_MODNORMALFLIPPING_HH defined
//=============================================================================


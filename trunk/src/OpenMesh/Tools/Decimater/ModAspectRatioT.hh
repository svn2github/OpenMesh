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
 *   $Revision: 448 $                                                        *
 *   $Date: 2011-11-04 13:59:37 +0100 (Fri, 04 Nov 2011) $                   *
 *                                                                           *
\*===========================================================================*/

/** \file ModAspectRatioT.hh
 */


//=============================================================================
//
//  CLASS ModAspectRatioT
//
//=============================================================================


#ifndef MODASPECTRATIOT_HH
#define MODASPECTRATIOT_HH


//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/ModBaseT.hh>
#include <OpenMesh/Core/Utils/Property.hh>


//== NAMESPACES ===============================================================

namespace OpenMesh  {
namespace Decimater {


//== CLASS DEFINITION =========================================================


template <class DecimaterT>
class ModAspectRatioT : public ModBaseT<DecimaterT>
{
public:
   
  DECIMATING_MODULE( ModAspectRatioT, DecimaterT, Roundness );

  typedef typename Mesh::Scalar      Scalar;
  typedef typename Mesh::Point       Point;

  // constructor
  ModAspectRatioT(DecimaterT&  _dec,
		float        _min_roundness = 5.0,
		bool         _is_binary = true)
    : Base(_dec, _is_binary),
      mesh_(Base::mesh()),
      min_roundness_(1.0/_min_roundness)
  {
    mesh_.add_property( roundness_ );
  }


  // destructor
  ~ModAspectRatioT()
  {
    mesh_.remove_property( roundness_ );
  }

  

  /// get roundness
  float roundness() const { return 1.0/min_roundness_; }
  /// set roundness
  void set_roundness(float _f) { min_roundness_ = 1.0/_f; }
  
  // precompute face roundness
  void initialize();
  // blabla
  float collapse_priority(const CollapseInfo& _ci);
  // update roundness of one-ring
  void preprocess_collapse(const CollapseInfo& _ci);


private:

  /** \brief return aspect ratio (length/height) of triangle
  *
  */
  Scalar aspectRatio( const Point& _v0,
                      const Point& _v1,
                      const Point& _v2 );

private:

  Mesh&  mesh_;
  float  min_roundness_;
  FPropHandleT<float>  roundness_;
};


//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(MB_MODASPECTRATIOT_C)
#define MODASPECTRATIOT_TEMPLATES
#include "ModAspectRatioT.cc"
#endif
//=============================================================================
#endif // MB_MODASPECTRATIOT_HH defined
//=============================================================================


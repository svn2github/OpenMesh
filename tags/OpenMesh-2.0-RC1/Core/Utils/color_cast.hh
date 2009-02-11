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
//   $Revision: 4118 $
//   $Date: 2009-01-05 07:45:18 +0100 (Mo, 05. Jan 2009) $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


#ifndef OPENMESH_COLOR_CAST_HH
#define OPENMESH_COLOR_CAST_HH


//== INCLUDES =================================================================


#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Utils/vector_cast.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {


//=============================================================================


/** \name Cast vector type to another vector type.
*/
//@{

//-----------------------------------------------------------------------------
#ifndef DOXY_IGNORE_THIS

/// Cast one color vector to another.
template <typename dst_t, typename src_t>
struct color_caster
{
  typedef dst_t  return_type;

  inline static return_type cast(const src_t& _src)
  {
    dst_t dst;
    vector_copy(_src, dst, GenProg::Int2Type<vector_traits<dst_t>::size_>());
    return dst;
  }
};


template <>
struct color_caster<Vec3uc,Vec3f>
{
  typedef Vec3uc return_type;

  inline static return_type cast(const Vec3f& _src)
  {
    return Vec3uc( (unsigned char)(_src[0]* 255.0f + 0.5f),
		               (unsigned char)(_src[1]* 255.0f + 0.5f),
		               (unsigned char)(_src[2]* 255.0f + 0.5f) );
  }
};

template <>
struct color_caster<Vec3uc,Vec4f>
{
  typedef Vec3uc return_type;

  inline static return_type cast(const Vec4f& _src)
  {
    return Vec3uc( (unsigned char)(_src[0]* 255.0f + 0.5f),
                   (unsigned char)(_src[1]* 255.0f + 0.5f),
                   (unsigned char)(_src[2]* 255.0f + 0.5f) );
  }
};

template <>
struct color_caster<Vec4uc,Vec3f>
{
  typedef Vec4uc return_type;

  inline static return_type cast(const Vec3f& _src)
  {
    return Vec4uc( (unsigned char)(_src[0]* 255.0f + 0.5f),
                   (unsigned char)(_src[1]* 255.0f + 0.5f),
                   (unsigned char)(_src[2]* 255.0f + 0.5f),
                   (unsigned char)(255) );
  }
};

template <>
struct color_caster<Vec4uc,Vec4f>
{
  typedef Vec4uc return_type;

  inline static return_type cast(const Vec4f& _src)
  {
    return Vec4uc( (unsigned char)(_src[0]* 255.0f + 0.5f),
                   (unsigned char)(_src[1]* 255.0f + 0.5f),
                   (unsigned char)(_src[2]* 255.0f + 0.5f),
                   (unsigned char)(_src[3]* 255.0f + 0.5f) );
  }
};

template <>
struct color_caster<Vec4uc,Vec3uc>
{
  typedef Vec4uc return_type;

  inline static return_type cast(const Vec3uc& _src)
  {
    return Vec4uc( _src[0], _src[1], _src[2], 255 );
  }
};

template <>
struct color_caster<Vec3f, Vec3uc>
{
  typedef Vec3f return_type;

  inline static return_type cast(const Vec3uc& _src)
  {
    const float f = 1.0f / 255.0f;
    return Vec3f(_src[0] * f, _src[1] *  f, _src[2] * f );
  }
};

template <>
struct color_caster<Vec3f, Vec4uc>
{
  typedef Vec3f return_type;

  inline static return_type cast(const Vec4uc& _src)
  {
    const float f = 1.0f / 255.0f;
    return Vec3f(_src[0] * f, _src[1] *  f, _src[2] * f );
  }
};

template <>
struct color_caster<Vec4f, Vec3uc>
{
  typedef Vec4f return_type;

  inline static return_type cast(const Vec3uc& _src)
  {
    const float f = 1.0f / 255.0f;
    return Vec4f(_src[0] * f, _src[1] *  f, _src[2] * f, 1.0f );
  }
};

template <>
struct color_caster<Vec4f, Vec4uc>
{
  typedef Vec4f return_type;

  inline static return_type cast(const Vec4uc& _src)
  {
    const float f = 1.0f / 255.0f;
    return Vec4f(_src[0] * f, _src[1] *  f, _src[2] * f, _src[3] * f );
  }
};

// ----------------------------------------------------------------------------


#ifndef DOXY_IGNORE_THIS

#if !defined(OM_CC_MSVC)
template <typename dst_t>
struct color_caster<dst_t,dst_t>
{
  typedef const dst_t&  return_type;

  inline static return_type cast(const dst_t& _src)
  {
    return _src;
  }
};
#endif

#endif

//-----------------------------------------------------------------------------


template <typename dst_t, typename src_t>
inline
typename color_caster<dst_t, src_t>::return_type
color_cast(const src_t& _src )
{
  return color_caster<dst_t, src_t>::cast(_src);
}

#endif
//-----------------------------------------------------------------------------

//@}


//=============================================================================
} // namespace OpenMesh
//=============================================================================
#endif // OPENMESH_COLOR_CAST_HH defined
//=============================================================================


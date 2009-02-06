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
//   $Revision: 1801 $
//   $Date: 2008-05-19 11:53:56 +0200 (Mon, 19 May 2008) $
//                                                                            
//=============================================================================


//=============================================================================
//
//  Helper Functions for binary reading / writing
//
//=============================================================================


#define OPENMESH_IO_OMFORMAT_CC


//== INCLUDES =================================================================

#include <OpenMesh/Core/IO/OMFormat.hh>
#include <algorithm>
#include <iomanip>

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace IO {

  // helper to store a an integer
  template< typename T > 
  size_t 
  store( std::ostream& _os, 
	 const T& _val, 
	 OMFormat::Chunk::Integer_Size _b, 
	 bool _swap,
	 t_signed)
  {    
    assert( OMFormat::is_integer( _val ) );

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 	
	OMFormat::int8 v = static_cast<OMFormat::int8>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::int16 v = static_cast<OMFormat::int16>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::int32 v = static_cast<OMFormat::int32>(_val);
	return store( _os, v, _swap );
      }      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::int64 v = static_cast<OMFormat::int64>(_val);
	return store( _os, v, _swap );
      }
    }
    return 0;
  }


  // helper to store a an unsigned integer
  template< typename T > 
  size_t 
  store( std::ostream& _os, 
	 const T& _val, 
	 OMFormat::Chunk::Integer_Size _b, 
	 bool _swap,
	 t_unsigned)
  {    
    assert( OMFormat::is_integer( _val ) );

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 
	OMFormat::uint8 v = static_cast<OMFormat::uint8>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::uint16 v = static_cast<OMFormat::uint16>(_val);
	return store( _os, v, _swap );
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::uint32 v = static_cast<OMFormat::uint32>(_val);
	return store( _os, v, _swap );
      }
      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::uint64 v = static_cast<OMFormat::uint64>(_val);
	return store( _os, v, _swap );
      }
    }
    return 0;
  }


  // helper to store a an integer
  template< typename T > 
  size_t 
  restore( std::istream& _is, 
	   T& _val, 
	   OMFormat::Chunk::Integer_Size _b, 
	   bool _swap,
	   t_signed)
  {    
    assert( OMFormat::is_integer( _val ) );
    size_t bytes = 0;

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 	
	OMFormat::int8 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::int16 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::int32 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
      }      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::int64 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
      }
    }
    return bytes;
  }


  // helper to store a an unsigned integer
  template< typename T > 
  size_t 
  restore( std::istream& _is, 
	   T& _val, 
	   OMFormat::Chunk::Integer_Size _b, 
	   bool _swap,
	   t_unsigned)
  {    
    assert( OMFormat::is_integer( _val ) );
    size_t bytes = 0;

    switch( _b ) 
    {
      case OMFormat::Chunk::Integer_8:
      { 
	OMFormat::uint8 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      case OMFormat::Chunk::Integer_16:
      { 
	OMFormat::uint16 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      case OMFormat::Chunk::Integer_32:
      { 
	OMFormat::uint32 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
      
      case OMFormat::Chunk::Integer_64:
      { 
	OMFormat::uint64 v;
	bytes = restore( _is, v, _swap );
	_val = static_cast<T>(v);
	break;
      }
    }
    return bytes;
  }

//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================

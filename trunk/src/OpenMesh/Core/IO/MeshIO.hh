/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2009 by Computer Graphics Group, RWTH Aachen      *
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
 *   $Revision$                                                         *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/


#ifndef OM_MESHIO_HH
#define OM_MESHIO_HH


//=== INCLUDES ================================================================

// -------------------- system settings
#include <OpenMesh/Core/System/config.h>
// -------------------- check include order
#if defined (OPENMESH_TRIMESH_ARRAY_KERNEL_HH) || \
    defined (OPENMESH_POLYMESH_ARRAY_KERNEL_HH)
#  error "Include MeshIO.hh before including a mesh type!"
#endif
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/SR_store.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/IO/importer/ImporterT.hh>
#include <OpenMesh/Core/IO/exporter/ExporterT.hh>


//== NAMESPACES ==============================================================

namespace OpenMesh {
namespace IO   {


//=== IMPLEMENTATION ==========================================================


/** \name Mesh Reading / Writing
    Convenience functions the map to IOManager functions.
    \see OpenMesh::IO::_IOManager_
*/
//@{


//-----------------------------------------------------------------------------


/** Read a mesh from file _filename. The file format is determined by
    the file extension. */
template <class Mesh>
bool 
read_mesh(Mesh&               _mesh, 
	  const std::string&  _filename, 
	  bool                _clear = true) 
{
  Options opt;
  return read_mesh(_mesh, _filename, opt, _clear);
}


/** Read a mesh from file _filename. The file format is determined by
    the file extension. */
template <class Mesh>
bool 
read_mesh(Mesh&               _mesh, 
	  const std::string&  _filename, 
	  Options&            _opt, 
	  bool                _clear = true) 
{
  if (_clear) _mesh.clear();
  ImporterT<Mesh> importer(_mesh);
  return IOManager().read(_filename, importer, _opt); 
}


/** Read a mesh from file open std::istream. The file format is determined by
    parameter _ext. _ext has to include ".[format]" in order to work properly */
template <class Mesh>
bool 
read_mesh(Mesh&               _mesh, 
	  std::istream&  _is,
	  const std::string& _ext,
	  Options&            _opt, 
	  bool                _clear = true) 
{
  if (_clear) _mesh.clear();
  ImporterT<Mesh> importer(_mesh);
  return IOManager().read(_is,_ext, importer, _opt); 
}



//-----------------------------------------------------------------------------


/** Write a mesh to the file _filename. The file format is determined
    by _filename's extension. */
template <class Mesh>
bool write_mesh(const Mesh& _mesh, const std::string& _filename,
                Options _opt = Options::Default) 
{ 
  ExporterT<Mesh> exporter(_mesh);
  return IOManager().write(_filename, exporter, _opt); 
}


//-----------------------------------------------------------------------------


/** Write a mesh to an open std::ostream. The file format is determined
    by _ext. */
template <class Mesh>
bool write_mesh(const Mesh& _mesh, 
		std::ostream& _os,
		const std::string& _ext,
                Options _opt = Options::Default) 
{ 
  ExporterT<Mesh> exporter(_mesh);
  return IOManager().write(_os,_ext, exporter, _opt); 
}


//-----------------------------------------------------------------------------


template <class Mesh>
size_t binary_size(const Mesh& _mesh, const std::string& _format,
                   Options _opt = Options::Default)
{
  ExporterT<Mesh> exporter(_mesh);
  return IOManager().binary_size(_format, exporter, _opt);
}


//-----------------------------------------------------------------------------

//@}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#if defined(OM_STATIC_BUILD) || defined(ARCH_DARWIN)
#  include <OpenMesh/Core/IO/IOInstances.hh>
#endif
//=============================================================================
#endif
//=============================================================================

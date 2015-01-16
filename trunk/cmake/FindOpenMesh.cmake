#
# Try to find OPENMESH
# Once done this will define
#  
# OPENMESH_FOUND           - system has OPENMESH
# OPENMESH_INCLUDE_DIRS    - the OPENMESH include directories
# OPENMESH_LIBRARIES       - Link these to use OPENMESH
# OPENMESH_LIBRARY_DIR     - directory where the libraries are included
#
# Copyright 2014 Computer Graphics Group, RWTH Aachen University
# Authors: Jan Möbius <moebius@cs.rwth-aachen.de>
#          Hans-Christian Ebke <ebke@cs.rwth-aachen.de>
#
# This file is part of OpenMesh.
#===========================================================================
#                                                                           
#                               OpenMesh                                    
#      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen      
#                           www.openmesh.org                                
#                                                                           
#---------------------------------------------------------------------------
#  This file is part of OpenMesh.                                           
#                                                                           
#  OpenMesh is free software: you can redistribute it and/or modify         
#  it under the terms of the GNU Lesser General Public License as           
#  published by the Free Software Foundation, either version 3 of           
#  the License, or (at your option) any later version with the              
#  following exceptions:                                                    
#                                                                           
#  If other files instantiate templates or use macros                       
#  or inline functions from this file, or you compile this file and         
#  link it with other files to produce an executable, this file does        
#  not by itself cause the resulting executable to be covered by the        
#  GNU Lesser General Public License. This exception does not however       
#  invalidate any other reasons why the executable file might be            
#  covered by the GNU Lesser General Public License.                        
#                                                                           
#  OpenMesh is distributed in the hope that it will be useful,              
#  but WITHOUT ANY WARRANTY; without even the implied warranty of           
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            
#  GNU Lesser General Public License for more details.                      
#                                                                           
#  You should have received a copy of the GNU LesserGeneral Public          
#  License along with OpenMesh.  If not,                                    
#  see <http://www.gnu.org/licenses/>.                                      
#                                                                           
#===========================================================================

cmake_minimum_required(VERSION 2.8.9)

#if already found via finder or simulated finder in openmesh CMakeLists.txt, skip the search
IF (NOT OPENMESH_FOUND) 
  SET (SEARCH_PATHS 
    /usr/local/
    /usr/
    "${CMAKE_SOURCE_DIR}/OpenMesh/src/OpenMesh"
    "${CMAKE_SOURCE_DIR}/libs_required/OpenMesh/src/OpenMesh"
    "${CMAKE_SOURCE_DIR}/../OpenMesh/src/OpenMesh"
    "C:/Program Files/OpenMesh 3.4"
    "C:/Program Files/OpenMesh 3.3"
    "C:/Program Files/OpenMesh 3.2"
    "C:/Program Files/OpenMesh 3.1"
    "C:/Program Files/OpenMesh 3.0"
    "C:/Program Files/OpenMesh 2.4.1"
    "C:/Program Files/OpenMesh 2.4"
    "C:/Program Files/OpenMesh 2.0/include"
    "C:/libs/OpenMesh 3.4"
    "C:/libs/OpenMesh 3.3"
    "C:/libs/OpenMesh 3.2"
    "C:/libs/OpenMesh 3.1"
    "C:/libs/OpenMesh 3.0"
    "C:/libs/OpenMesh 2.4.1"
    "C:/libs/OpenMesh 2.4"
    "${OPENMESH_LIBRARY_DIR}"
  )

  FIND_PATH (OPENMESH_INCLUDE_DIR OpenMesh/Core/Mesh/PolyMeshT.hh
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES include)

  FIND_LIBRARY(OPENMESH_CORE_LIBRARY_RELEASE NAMES OpenMeshCore
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES lib lib64)

  FIND_LIBRARY(OPENMESH_CORE_LIBRARY_DEBUG NAMES OpenMeshCored 
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES lib lib64)

  FIND_LIBRARY(OPENMESH_TOOLS_LIBRARY_RELEASE NAMES OpenMeshTools
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES lib lib64)

  FIND_LIBRARY(OPENMESH_TOOLS_LIBRARY_DEBUG NAMES OpenMeshToolsd
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES lib lib64)

#select configuration depending on platform (optimized... on windows)
  include(SelectLibraryConfigurations)
  select_library_configurations( OPENMESH_TOOLS )
  select_library_configurations( OPENMESH_CORE )

  set(OPENMESH_LIBRARIES ${OPENMESH_CORE_LIBRARY} ${OPENMESH_TOOLS_LIBRARY} )
  set(OPENMESH_INCLUDE_DIRS ${OPENMESH_INCLUDE_DIR} )

#checks, if OPENMESH was found and sets OPENMESH_FOUND if so
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(OpenMesh  DEFAULT_MSG
                                    OPENMESH_CORE_LIBRARY OPENMESH_TOOLS_LIBRARY OPENMESH_INCLUDE_DIR)
 
#sets the library dir 
  if ( OPENMESH_CORE_LIBRARY_RELEASE )
    get_filename_component(_OPENMESH_LIBRARY_DIR ${OPENMESH_CORE_LIBRARY_RELEASE} PATH)
  else( OPENMESH_CORE_LIBRARY_RELEASE )
    get_filename_component(_OPENMESH_LIBRARY_DIR ${OPENMESH_CORE_LIBRARY_DEBUG} PATH)
  endif( OPENMESH_CORE_LIBRARY_RELEASE )
  set (OPENMESH_LIBRARY_DIR "${_OPENMESH_LIBRARY_DIR}" CACHE PATH "The directory where the OpenMesh libraries can be found.")
  
 
  mark_as_advanced(OPENMESH_INCLUDE_DIR OPENMESH_CORE_LIBRARY_RELEASE OPENMESH_CORE_LIBRARY_DEBUG OPENMESH_TOOLS_LIBRARY_RELEASE OPENMESH_TOOLS_LIBRARY_DEBUG OPENMESH_LIBRARY_DIR)
endif()

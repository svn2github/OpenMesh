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
//   $Revision: 1800 $
//   $Date: 2008-05-19 11:51:23 +0200 (Mo, 19. Mai 2008) $
//                                                                            
//=============================================================================


//== INCLUDES =================================================================

#ifdef _MSC_VER
#  pragma warning(disable: 4267 4311)
#endif


#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Apps/Subdivider/MeshViewerWidget.hh>


using namespace OpenMesh;


//== IMPLEMENTATION ========================================================== 


inline bool 
MeshViewerWidget::open_mesh(const char* _filename, IO::Options _opt)
{
  if ( Base::open_mesh( _filename, _opt ) )
  {
    orig_mesh_ = mesh_;
    return true;
  }
  return false;
}




//=============================================================================

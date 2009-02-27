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


//== INCLUDES =================================================================

#ifdef _MSC_VER
#  pragma warning(disable: 4267 4311)
#endif

#include <iostream>
#include <fstream>

#include <qapplication.h>
#include <qdatetime.h>

#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include <OpenMesh/Apps/Decimating/DecimaterViewerWidget.hh>


//== IMPLEMENTATION ========================================================== 


//-----------------------------------------------------------------------------

void DecimaterViewerWidget::keyPressEvent(QKeyEvent* _event)
{
  switch (_event->key())
  {
    case Key_D:
    {
      int rc;
      if ( (rc=decimater_->decimate(steps_)) )
      {
	decimater_->mesh().garbage_collection();
	std::cout << rc << " vertices removed!\n";
	updateGL();
      }
      else
	std::cout << "Decimation failed\n";
      break;
    }

    case Key_Plus:
      steps_ = std::min( ++steps_, (size_t)(mesh_.n_vertices() * 0.1) );
      updateGL();
      std::cout << "# decimating steps increased to " << steps_ << std::endl;
      break;

    case Key_Minus:
      steps_ = std::max( --steps_, size_t(1) );
      updateGL();
      std::cout << "# decimating steps increased to " << steps_ << std::endl;
      break;

    case Key_S:
    {
      OpenMesh::IO::Options opt;

      opt += OpenMesh::IO::Options::Binary;

      if (OpenMesh::IO::write_mesh( mesh(), "result.off", opt ))
	std::cout << "mesh saved in 'result.off'\n";
    }
    break;

    case Key_Q:
    case Key_Escape:
      qApp->quit();      

    default:
      this->inherited_t::keyPressEvent(_event);
  }
}

void DecimaterViewerWidget::animate( void )   
{
//    updateGL();
//    timer_->start(300, true);
}

//=============================================================================

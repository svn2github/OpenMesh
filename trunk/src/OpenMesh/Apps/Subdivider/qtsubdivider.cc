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
//=============================================================================
#ifdef _MSC_VER
#  pragma warning(disable: 4267 4311)
#endif

#include <iostream>
#include <fstream>

#include <QApplication>
#include <QMessageBox>
#include "SubdivideWidget.hh"

#ifdef ARCH_DARWIN
#include <glut.h>
#else
#include <GL/glut.h>
#endif

int main(int argc, char **argv)
{
  // OpenGL check
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication app(argc,argv);

  glutInit(&argc,argv);

  if ( !QGLFormat::hasOpenGL() ) {
    QString msg = "System has no OpenGL support!";
    QMessageBox::critical( NULL, "OpenGL", msg + argv[1], QMessageBox::Ok );
    return -1;
  }

    
  // create widget
  SubdivideWidget* w = new SubdivideWidget(0, "Subdivider");  

  w->resize(400, 400);
  w->show();

  // load scene
  if (argc > 1)  
  {
     if ( ! w->open_mesh(argv[1]) )
     {
        QString msg = "Cannot read mesh from file ";
        QMessageBox::critical( NULL, argv[1], msg + argv[1], QMessageBox::Ok );
        return -1;
     }
  }


  return app.exec();
}

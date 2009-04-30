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
#ifdef _MSC_VER
#  pragma warning(disable: 4267 4311)
#endif

#include <iostream>
#include <fstream>
#include <getopt.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <OpenMesh/Apps/QtViewer/MeshViewerWidgetT.hh>
#include <OpenMesh/Tools/Kernel_OSG/TriMesh_OSGArrayKernelT.hh>


struct MyTraits : public OpenMesh::Kernel_OSG::Traits
{
  VertexAttributes(OpenMesh::Attributes::Normal   |
                   OpenMesh::Attributes::TexCoord );
  HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
  FaceAttributes(OpenMesh::Attributes::Normal);
};

  
typedef OpenMesh::Kernel_OSG::TriMesh_OSGArrayKernelT<MyTraits>  MyMesh;
typedef MeshViewerWidgetT<MyMesh> MeshViewerWidget;

void usage_and_exit(int xcode);

int main(int argc, char **argv)
{
  osg::osgInit(argc, argv);

  // OpenGL check
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication app(argc,argv);

  if ( !QGLFormat::hasOpenGL() ) {
    QString msg = "System has no OpenGL support!";
    QMessageBox::critical( NULL, "OpenGL", msg + argv[1], 0 );
    return -1;
  }


  int c;
  OpenMesh::IO::Options opt;
  
  while ( (c=getopt(argc,argv,"s"))!=-1 )
  {
     switch(c)
     {
       case 's': opt += OpenMesh::IO::Options::Swap; break;
       case 'h':
          usage_and_exit(0);
       default:
          usage_and_exit(1);
     }
  }
  // create widget
  MeshViewerWidget* w = new MeshViewerWidget(0, "Viewer");  
  app.setMainWidget(w);

  // static mesh, hence use strips
  w->enable_strips();

  w->resize(400, 400);
  w->show(); 

  // load scene
  if ( optind < argc )  
  {
     if ( ! w->open_mesh(argv[optind], opt) )
     {
        QString msg = "Cannot read mesh from file:\n '";
        msg += argv[optind];
        msg += "'";
        QMessageBox::critical( NULL, w->caption(), msg, 0 );
        return 1;
     }
  }

  if ( ++optind < argc )
  {
     if ( ! w->open_texture( argv[optind] ) )
     {
         QString msg = "Cannot load texture image from file:\n '";
        msg += argv[optind];
        msg += "'\n\nPossible reasons:\n";
        msg += "- Mesh file didn't provide texture coordinates\n";
        msg += "- Texture file does not exist\n";
        msg += "- Texture file is not accessible.\n";
        QMessageBox::warning( NULL, w->caption(), msg, 0 );
     }
  }

  return app.exec();
}

void usage_and_exit(int xcode)
{
   std::cout << "Usage: meshviewer [-s] [mesh] [texture]\n" << std::endl;
   std::cout << "Options:\n"
             << "  -s\n"
             << "    Reverse byte order, when reading binary files.\n"
             << std::endl;
   exit(xcode);
}

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
//   $Revision: 4483 $
//   $Date: 2009-01-28 11:41:31 +0100 (Mi, 28. Jan 2009) $
//                                                                            
//=============================================================================

//

#if !defined(USE_SOQT)
#  define USE_SOQT 0
#endif

//== INCLUDES =================================================================


#include <cstdlib>
//
// Attention! must include this before including inventor files!
// There some dependencies not solved yet!
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
//
#include <Inventor/SoDB.h>
#if USE_SOQT
#  include <qapplication.h>
#  include <Inventor/Qt/SoQt.h>
#  include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#else
#  include <Inventor/Xt/SoXt.h>
#  include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#endif
#include <Inventor/nodes/SoSeparator.h>
//
#include <OpenMesh/Apps/IvViewer/SoOpenMeshNodeT.hh>


//== CLASS DEFINITION =========================================================


struct MyTraits : public OpenMesh::DefaultTraits
{
   VertexAttributes(OpenMesh::Attributes::Normal);
   HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
   FaceAttributes(OpenMesh::Attributes::Normal);
};

  
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  MyMesh;
typedef OpenMesh::SoOpenMeshNodeT<MyMesh>         MyNode;



//== IMPLEMENTATION ===========================================================



int main(int argc, char **argv)
{
   OpenMesh::IO::Options opt;

#if USE_SOQT
   QApplication app(argc,argv);

   SoQt::init( argv[0] );

   MyNode::initClass();
   SoQtExaminerViewer *myViewer = new SoQtExaminerViewer();


   // Read a mesh
   MyMesh  mesh;
   if (argc > 1 && OpenMesh::IO::read_mesh(mesh, argv[1], opt))
   {
    if (!opt.check( OpenMesh::IO::Options::FaceNormal))
      mesh.update_face_normals();
      
      SoSeparator* root = new SoSeparator();
      root->addChild(new MyNode(&mesh));
      
      myViewer->setSceneGraph(root);
   }

   QObject::connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
   
   myViewer->show();
   SoQt::mainLoop();
#else
  // Inventor stuff  
  Widget myWindow = SoXt::init(argv[0]);
  MyNode::initClass();
  SoXtExaminerViewer *myViewer = 
    new SoXtExaminerViewer(myWindow);

   

  // Read a mesh
  MyMesh  mesh;
  if (argc > 1 && OpenMesh::IO::read_mesh(mesh, argv[1], opt))
  {
    if (!opt.check( OpenMesh::IO::Options::FaceNormal))
      mesh.update_face_normals();

    SoSeparator* root = new SoSeparator();
    root->addChild(new MyNode(&mesh));

    myViewer->setSceneGraph(root);
  }

  myViewer->show();
  SoXt::show(myWindow);
  SoXt::mainLoop();
#endif
}


//=============================================================================

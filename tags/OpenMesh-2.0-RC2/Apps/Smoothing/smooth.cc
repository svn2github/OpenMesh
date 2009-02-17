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


#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh> // include before kernel type!
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>


using namespace OpenMesh;
using namespace Smoother;


struct MyTraits : public OpenMesh::DefaultTraits
{
#if 1
  typedef OpenMesh::Vec3f Point;
  typedef OpenMesh::Vec3f Normal;
#else
  typedef OpenMesh::Vec3d Point;
  typedef OpenMesh::Vec3d Normal;
#endif
};

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  MyMesh;


//-----------------------------------------------------------------------------

void usage_and_exit(int _xcode)
{
  std::cout << std::endl;
  std::cout << "Usage: smooth [Options] <iterations> <input mesh> <output mesh>\n";
  std::cout << std::endl;
  std::cout << "Options \n"
            << std::endl
	    << "  -c <0|1> \t continuity (C0,C1). Default: C1\n"
            << "  -t \t\t smooth tangential direction. Default: Enabled\n"
            << "  -n \t\t smooth normal direction. Default: Enabled\n"
	    << std::endl;
  exit(_xcode);
}


//-----------------------------------------------------------------------------


int main(int argc, char **argv)
{
  int    c;

  MyMesh  mesh;
  OpenMesh::Utils::Timer t;
  std::string ifname;
  std::string ofname;

  SmootherT<MyMesh>::Continuity 
    continuity = SmootherT<MyMesh>::C1;

  SmootherT<MyMesh>::Component  
    component  = SmootherT<MyMesh>::Tangential_and_Normal;

  int iterations;

  // ---------------------------------------- evaluate command line

  while ( (c=getopt(argc, argv, "tnc:h"))!=-1 )
  {
    switch(c)
    {
      case 'c': {
	switch(*optarg)
        { 
	  case '0' : continuity = SmootherT<MyMesh>::C0; break;
	  case '1' : continuity = SmootherT<MyMesh>::C1; break;
	}
	break;
      }
      case 't':
	component = component==SmootherT<MyMesh>::Normal
	  ? SmootherT<MyMesh>::Tangential_and_Normal
	  : SmootherT<MyMesh>::Tangential;
	break;

      case 'n': 
	component = component==SmootherT<MyMesh>::Tangential 
	  ? SmootherT<MyMesh>::Tangential_and_Normal
	  : SmootherT<MyMesh>::Normal;
	break;

      case 'h': usage_and_exit(0);
      case '?':
      default:  usage_and_exit(1);
    }
  }
  
  if (argc-optind < 3)
    usage_and_exit(1);


  // # iterations
  {
    std::stringstream str; str << argv[optind]; str >> iterations;
  }


  // input file
  ifname = argv[++optind];


  // output file
  ofname = argv[++optind];


  OpenMesh::IO::Options opt;

  // ---------------------------------------- read mesh
  
  omout() << "read mesh..." << std::flush;
  t.start();
  OpenMesh::IO::read_mesh(mesh, ifname, opt);
  t.stop();
  omout() << "done (" << t.as_string() << ")\n";

  omout() << "  #V " << mesh.n_vertices() << std::endl;

  // ---------------------------------------- smooth

  JacobiLaplaceSmootherT<MyMesh> smoother(mesh);
  smoother.initialize(component,continuity);
 
  omout() << "smoothing..." << std::flush;

  t.start(); 
  smoother.smooth(iterations); 
  t.stop();

  omout() << "done (";
  omout() << t.seconds() << "s ~ ";
  omout() << t.as_string() << ", " 
        << (iterations*mesh.n_vertices())/t.seconds() << " Vertices/s)\n";

  // ---------------------------------------- write mesh

  omout() << "write mesh..." << std::flush;
  t.start();
  OpenMesh::IO::write_mesh(mesh, ofname, opt);
  t.stop();
  omout() << "done (" << t.as_string() << ")\n";

  return 0;
}

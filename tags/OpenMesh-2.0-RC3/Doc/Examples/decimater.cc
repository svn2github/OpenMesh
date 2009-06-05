// 
using namespace OpenMesh

// ---------------------------------------- necessary types

// Mesh type
typedef TriMesh_ArrayKernelT<>                      Mesh;

// Decimater type
typedef Decimater::DecimaterT< Mesh >               Decimater;

// Decimation Module Handle type
typedef Decimater::ModQuadricT< decimater >::Handle HModQuadric;

// ---------------------------------------- decimater setup

Mesh        mesh;             // a mesh object
Decimater   decimater(mesh);  // a decimater object, connected to a mesh
HModQuadric hModQuadric;      // use a quadric module

decimater.add( hModQuadric ); // register module at the decimater

std::cout << decimater.module( hModQuadric ).name() << std::endl;
                              // the way to access the module 

decimater.initialize();       // let the decimater initialize the mesh and the
                              // modules

decimater.decimate();         // do decimation


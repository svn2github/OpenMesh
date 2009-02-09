#ifndef OPENMESH_APPS_VDPMSTREAMING_CLIENT_MYMESH_HH
#define OPENMESH_APPS_VDPMSTREAMING_CLIENT_MYMESH_HH

#include <qthread.h>
#include <qmutex.h>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/VDPM/MeshTraits.hh>


using OpenMesh::VDPM::MeshTraits;


//== CLASS DEFINITION =========================================================


typedef OpenMesh::TriMesh_ArrayKernelT<MeshTraits>  MyMesh;

static QMutex  mutex_;


//== CLASS DEFINITION =========================================================

#endif //OPENMESH_APPS_VDPMSTREAMING_CLIENT_MYMESH_HH defined

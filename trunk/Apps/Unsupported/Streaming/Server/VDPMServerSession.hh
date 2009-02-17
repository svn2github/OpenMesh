#ifndef OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSESSION_HH
#define OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSESSION_HH

#include <qsocket.h>
#include <qthread.h>
#include <qdatastream.h>
#include <qtimer.h>
#include <iostream>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Geometry/Plane3d.hh>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Tools/VDPM/StreamingDef.hh>
#include <OpenMesh/Tools/VDPM/VHierarchyNodeIndex.hh>
#include <OpenMesh/Tools/VDPM/ViewingParameters.hh>
#include <OpenMesh/Tools/VDPM/VHierarchy.hh>
#include <OpenMesh/Tools/VDPM/VFront.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/ServerSideVDPM.hh>
#include <OpenMesh/Tools/VDPM/VHierarchyWindow.hh>


using OpenMesh::VDPM::VDPMStreamingPhase;
using OpenMesh::VDPM::kBaseMesh;
using OpenMesh::VDPM::kVSplits;

using OpenMesh::VDPM::VHierarchyWindow;
using OpenMesh::VDPM::VHierarchyNodeIndex;
using OpenMesh::VDPM::VHierarchyNodeHandle;
using OpenMesh::VDPM::ViewingParameters;

using OpenMesh::VDPM::set_debug_print;

class VDPMServerSession : public QSocket, public QThread
{

  Q_OBJECT

public:

  VDPMServerSession(int sock, QObject *parent=0, const char *name=0)
    : QSocket(parent, name)
  {
    set_debug_print(true);

    streaming_phase_       = kBaseMesh;
    transmission_complete_ = false;

    connect(this, SIGNAL(connected()), SLOT(socketConnected()));
    connect(this, SIGNAL(readyRead()), SLOT(socketReadyRead()));
    //connect(this, SIGNAL(connectionClosed()), SLOT(deleteLater()));
    connect(this, SIGNAL(connectionClosed()), SLOT(delayedCloseFinished()));
    setSocket(sock);


    qStatisticsTimer_ = new QTimer(this);
    connect(qStatisticsTimer_, SIGNAL(timeout()), this, SLOT(print_statistics()));

    mem_file = fopen("mem.txt", "w");
    
    start();
  }

  ~VDPMServerSession()
  {
    fclose(mem_file);
  }

  void run()
  {
    while (true)
    {
      sleep(1);
    }
  }

private:  

  VDPMStreamingPhase  streaming_phase_;
  bool                transmission_complete_;

private:

  void sendBaseMeshToClient();
  void send_vsplit_packets();
  void readBaseMeshRequestFromClient();
  void readViewingParametersFromClient();

  void PrintOutVFront();

private slots:

  void socketConnected()
  {
    std::cout << "socket is connected" << std::endl;
  }

  void socketReadyRead()
  {
    if (streaming_phase_ == kBaseMesh)
    {
      readBaseMeshRequestFromClient();      
    }
    else if (streaming_phase_ == kVSplits)
    {
      readViewingParametersFromClient();
    }
  }

  void print_statistics()
  {
    //std::cout << memory_requirements(true) << " " << memory_requirements(false) << std::endl;
  }

private:  
  unsigned short                tree_id_bits_;    // obsolete
  ServerSideVDPM*               vdpm_;
  VHierarchy*                   vhierarchy_;
  VHierarchyWindow              vhwindow_;
  ViewingParameters             viewing_parameters_;
  float                         kappa_square_;
  VHierarchyNodeHandleContainer vsplits_;

private:
  bool outside_view_frustum(const OpenMesh::Vec3f &pos, float radius);
  bool oriented_away(float sin_square, float distance_square, float product_value);
  bool screen_space_error(float mue_square, float sigma_square, float distance_square, float product_value);

  void adaptive_refinement();
  void sequential_refinement();
  bool qrefine(VHierarchyNodeHandle _node_handle);
  void force_vsplit(VHierarchyNodeHandle node_handle);
  void vsplit(VHierarchyNodeHandle _node_handle);
  VHierarchyNodeHandle active_ancestor_handle(VHierarchyNodeIndex &node_index);
  void stream_vsplits();

public:
  bool set_vdpm(const char _vdpm_name[256]);
  unsigned int  memory_requirements_using_window(bool _estimate);
  unsigned int  memory_requirements_using_vfront();

  // for example
private:
  QTimer  *qStatisticsTimer_;
  FILE    *mem_file;
};

#endif //OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSESSION_HH defined

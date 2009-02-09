#ifndef OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSTREAMINGSERVERWIDGET_HH
#define OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSTREAMINGSERVERWIDGET_HH

#include <QApplication>
#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/ServerSideVDPM.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerSocket.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerSession.hh>


using OpenMesh::VDPM::set_debug_print;
using OpenMesh::VDPM::debug_print;


class VDPMServerViewerWidget : public QWidget
{
  Q_OBJECT
public:
  //VDPMServerViewerWidget(QWidget *_parent) : QWidget(_parent)
  VDPMServerViewerWidget() : QWidget()
  {
    server = new VDPMServerSocket(this);

//     connect(server, 
// 	    SIGNAL(newConnection()), 
// 	    SLOT(newConnect()));

    vd_streaming_ = true;
  }

private:
  typedef ServerSideVDPMList::iterator  ServerSideVDPMListIter;
  ServerSideVDPMList  vdpms_;
  bool                vd_streaming_;
  VDPMServerSocket    *server;

public:

  ServerSideVDPM* get_vdpm(const char _vdpm_name[256]);


public:

  bool  open_vd_prog_mesh(const char *_filename);
  bool  vd_streaming() const      { return  vd_streaming_; }

private slots:

  void newConnect(VDPMServerSession *s)
  {
    std::cout << "New connection" << std::endl;

//     connect(s, SIGNAL(connectionClosed()), SLOT(connectionClosed()));
  }

  void connectionClosed()
  {
    std::cout << "Client closed connection" << std::endl;
  }

protected:

  virtual void keyPressEvent(QKeyEvent* _event);

};


#endif //OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSTREAMINGSERVERWIDGET_HH defined



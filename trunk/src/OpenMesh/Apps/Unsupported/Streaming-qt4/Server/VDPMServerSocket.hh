#ifndef OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSOCKET_HH
#define OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSOCKET_HH

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVBoxLayout>

#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerSession.hh>
#include <OpenMesh/Tools/VDPM/StreamingDef.hh>
#include <iostream>


class VDPMServerSocket : public QTcpServer
{
  Q_OBJECT

public:

  VDPMServerSocket(QObject *parent=0)
    : QTcpServer(parent)
  {
    setMaxPendingConnections(1);
    if ( !listen(QHostAddress::Any, VDPM_STREAMING_PORT)  )
    {
      std::cerr << "Failed to bind to port " << VDPM_STREAMING_PORT << std::endl;
      exit(1);
    }

    connect(this, SIGNAL(newConnection()),this,SLOT(newConnections()));
  }

public slots:

  void newConnections()
  {
    VDPMServerSession *s = new VDPMServerSession(nextPendingConnection(), this);
    //s->set_vdpm();
    emit newConnect(s);
    std::cout << "new connection"<< /*socket << */std::endl;
  }

signals:

  void newConnect(VDPMServerSession*);

};



#endif //OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSOCKET_HH defined

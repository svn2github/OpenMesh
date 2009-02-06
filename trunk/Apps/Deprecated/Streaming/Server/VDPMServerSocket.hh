#ifndef OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSOCKET_HH
#define OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSOCKET_HH

#include <qobject.h>
#include <qsocket.h>
#include <qserversocket.h>
#include <qvbox.h>

#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerSession.hh>
#include <OpenMesh/Tools/VDPM/StreamingDef.hh>
#include <iostream>


class VDPMServerSocket : public QServerSocket
{
  Q_OBJECT

public:

  VDPMServerSocket(QObject *parent=0)
    : QServerSocket(VDPM_STREAMING_PORT, 1, parent)
  {
    if (!ok())
    {
      std::cerr << "Failed to bind to port " 
		<< VDPM_STREAMING_PORT << std::endl;
      exit(1);
    }
  }

  void newConnection(int socket)
  {
    VDPMServerSession *s = new VDPMServerSession(socket, this);
    //s->set_vdpm();
    emit newConnect(s);
    std::cout << "new connection with: " << socket << std::endl;
  }

signals:

  void newConnect(VDPMServerSession*);

};



#endif //OPENMESH_APP_VDPMSTREAMING_SERVER_VDPMSERVERSOCKET_HH defined

#include <iostream>

#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMStreamingServer.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/ServerSideVDPM.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerSocket.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerSession.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerViewerWidget.hh>
#include <OpenMesh/Tools/Utils/getopt.h>

#include <QHostAddress>
#include <QApplication>



int main(int argc, char **argv)
{
  std::cout << "View-dependent streaming of PM server." << std::endl;

  QApplication app(argc, argv);
  VDPMServerViewerWidget server_widget;
  
  server_widget.resize(50, 50);
//   app.setMainWidget(&server_widget);
  server_widget.show();

  for (int idx=1; idx < argc; ++idx)
  {
    std::cout << "loading " << argv[idx] << std::endl;
    server_widget.open_vd_prog_mesh( argv[idx] ) ;
  }

  return  app.exec();
}


// #ifdef _MSC_VER
// #  pragma warning(disable: 4267 4311)
// #endif

#include <iostream>
#include <fstream>

#include <OpenMesh/Tools/Utils/getopt.h>

//#include <OpenMesh/Apps/VDProgMesh/Streaming/Client/ClientMainWindow.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Client/VDPMClientViewerWidget.hh>
//#include <OpenMesh/Apps/VDProgMesh/Streaming/Client/PropertyWidget.hh>
//#include <OpenMesh/Apps/VDProgMesh/Streaming/Client/OutputWidget.hh>

#include <QString>
#include <QApplication>
#include <QGLWidget>


int main(int argc, char **argv)
{
  // OpenGL check
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication app(argc,argv);

  if ( !QGLFormat::hasOpenGL() ) {
    std::cerr << "This system has no OpenGL support.\n";
    return -1;
  }
    
  //
  int         c;
  int         port  = -1;
  std::string bmesh = "";
  std::string sname = "localhost";

  while ( (c=getopt(argc, argv, "b:p:s:"))!=-1 )
  {
    switch(c)
    {
      case 'b': bmesh = optarg; break;
      case 's': sname = optarg; break; 
      case 'p': { std::istringstream istr(optarg); istr >> port; } break;
    }
  }

  // create widget
  VDPMClientViewerWidget* w = 
    new VDPMClientViewerWidget(0, "VDPMClientViewer");

  if (port == -1)
    w->connectToServer( sname );
  else
    w->connectToServer( sname, port );

  w->resize(800, 800);
//   app.setMainWidget(w);
  w->show();

  w->openBaseMesh( bmesh );

  // print usage info
  std::cout << "\n\n"
	    << "Press  Minus : Coarsen mesh\n"
	    << "       Plus  : Refine mesh\n"
	    << "       Home  : Coarsen down to base mesh\n"
	    << "       End   : Refine up to finest mesh\n"
	    << "\n";
	    
  return app.exec();
}


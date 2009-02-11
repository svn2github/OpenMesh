#ifdef _MSC_VER
#  pragma warning(disable: 4267 4311)
#endif

#include <iostream>

#include <QString>
#include <QApplication>

#include "VDPMSynthesizerViewerWidget.hh"

#include <GL/glut.h>


int main(int argc, char **argv)
{
  // OpenGL check
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication app(argc,argv);

  glutInit(&argc,argv);

  if ( !QGLFormat::hasOpenGL() ) {
    std::cerr << "This system has no OpenGL support.\n";
    return 1;
  }

  // create widget
  VDPMSynthesizerViewerWidget* 
    w = new VDPMSynthesizerViewerWidget(0, "VDPMSynthesizerViewer");

  w->resize(400, 400);
  w->show();

  // load scene
  if (argc==2)  
    w->open_vd_prog_mesh(argv[1]);
  else
  {
    std::cerr << "Usage: vdpmsynthesizer <vdpm-file.spm>\n";
    return 1;
  }

  // print usage info
  std::cout << "\n\n"
	    << "Press  Minus : Coarsen mesh\n"
	    << "       Plus  : Refine mesh\n"
	    << "       Home  : Coarsen down to base mesh\n"
	    << "       End   : Refine up to finest mesh\n"
	    << "\n";
	    
  return app.exec();
}

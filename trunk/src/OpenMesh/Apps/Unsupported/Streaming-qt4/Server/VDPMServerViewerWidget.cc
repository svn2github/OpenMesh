#include <QKeyEvent>
#include <iterator>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Server/VDPMServerViewerWidget.hh>

bool
VDPMServerViewerWidget::
open_vd_prog_mesh(const char *_filename)
{
  ServerSideVDPMListIter  vdpm_it;

  vdpm_it = vdpms_.insert(vdpms_.end(), ServerSideVDPM());

  ServerSideVDPM &vdpm = *vdpm_it;

  return  vdpm.open_vd_prog_mesh(_filename);
}

ServerSideVDPM*
VDPMServerViewerWidget::
get_vdpm(const char _vdpm_name[256])
{
  ServerSideVDPMListIter  vdpm_it;

  for (vdpm_it=vdpms_.begin(); vdpm_it!=vdpms_.end(); ++vdpm_it)
  {
    if (vdpm_it->is_same_name(_vdpm_name) == true)
    {
      return  &(*vdpm_it);
    }
  }

  return  NULL;
}

void
VDPMServerViewerWidget::
keyPressEvent(QKeyEvent* _event)
{
  bool handled(false);

  QString filename;

  switch (_event->key())
  {
    case Qt::Key_D:
      set_debug_print(!debug_print());
      std::cout << "debug print mode "
    << (debug_print() == true ? "on" : "off") << std::endl;
      break;

    case Qt::Key_O:
#if defined(OM_CC_MSVC)
      filename = QFileDialog::getOpenFileName(0, "", "d:/data/models/spm/", "*.spm");
#else
      filename = QFileDialog::getOpenFileName(0, "", "~/data/models/spm/", "*.spm");
#endif
      open_vd_prog_mesh( filename.toStdString().c_str() );
      break;

    case Qt::Key_I:
      std::copy( vdpms_.begin(), vdpms_.end(),
                 std::ostream_iterator<ServerSideVDPM>(std::cout, "\n") );
      break;

    case Qt::Key_V:
      vd_streaming_ = !(vd_streaming_);
      if (vd_streaming_)
        std::cout << "View-dependent streaming mode" << std::endl;
      else
        std::cout << "Sequential streaming mode" << std::endl;

      break;

    case Qt::Key_Q:
    case Qt::Key_Escape:
      qApp->quit();
  }

  if (!handled)
    _event->ignore();
}

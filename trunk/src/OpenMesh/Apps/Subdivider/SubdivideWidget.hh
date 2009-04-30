//=============================================================================
//
//  CLASS SubdivideWidget
//
//=============================================================================

#ifndef SUBDIVIDEWIDGET_HH
#define SUBDIVIDEWIDGET_HH


//== INCLUDES =================================================================

#include <QTimer>
#include <QMainWindow>
#include <QStatusBar>
#include <OpenMesh/Apps/Subdivider/MeshViewerWidget.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/SubdividerT.hh>




//== CLASS DEFINITION =========================================================

class SubdivideWidget : public QWidget
{
  Q_OBJECT
  
public:

  typedef OpenMesh::Subdivider::Uniform::SubdividerT< Mesh > Subdivider;

  enum SOPType {
    SOP_UniformCompositeLoop,
    SOP_UniformCompositeSqrt3,
    SOP_UniformLoop,
    SOP_UniformSqrt3,
    SOP_Undefined
  };

  typedef std::map< SOPType, Subdivider* > SubdividerPool;

public:
   
  /// constructor
  SubdivideWidget(QWidget* _parent=0, const char* _name=0);

  /// destructor
  ~SubdivideWidget() {};


  /// open mesh from _filename
  bool open_mesh(const char* _filename);

  void keyPressEvent( QKeyEvent *k );

  /// Updates Status Bar Information
  QStatusBar *status_bar;
  void update();

 
private slots:

  void animate_slot(void);
  void slot_select_sop(int i);
  void subdiv_slot();
  void reset_slot();
  void save_slot();
  void load_slot();

private:

  // widgets
  MeshViewerWidget* viewer_widget_;

  QTimer *timer_;
   
  int animate_step_;
  int max_animate_steps_;
  int msecs_;

  // Selected and current subdivision operator
  SOPType sel_topo_type; // selected operator
  SOPType cur_topo_type; // active operator

  // 
  SubdividerPool subdivider_;

};


//=============================================================================
#endif // SUBDIVIDEWIDGET_HH defined
//=============================================================================


//=============================================================================
//                                                                            
//                               OpenMesh                                     
//        Copyright (C) 2003 by Computer Graphics Group, RWTH Aachen          
//                           www.openmesh.org                                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This library is free software; you can redistribute it and/or modify it 
//   under the terms of the GNU Lesser General Public License as published   
//   by the Free Software Foundation, version 2.1.                           
//                                                                             
//   This library is distributed in the hope that it will be useful, but       
//   WITHOUT ANY WARRANTY; without even the implied warranty of                
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
//   Lesser General Public License for more details.                           
//                                                                            
//   You should have received a copy of the GNU Lesser General Public          
//   License along with this library; if not, write to the Free Software       
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//   $Revision: 1800 $
//   $Date: 2008-05-19 11:51:23 +0200 (Mo, 19. Mai 2008) $
//                                                                            
//=============================================================================


#ifndef OPENMESHAPPS_PROGVIEWERWIDGET_HH
#define OPENMESHAPPS_PROGVIEWERWIDGET_HH


//== INCLUDES =================================================================

#include <QTimer>
#include <OpenMesh/Apps/QtViewer/MeshViewerWidgetT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Attributes.hh>
#include <string>



//== CLASS DEFINITION =========================================================


using namespace OpenMesh;
using namespace OpenMesh::Attributes;


struct MyTraits : public OpenMesh::DefaultTraits
{
  VertexAttributes  ( OpenMesh::Attributes::Normal       |
                      OpenMesh::Attributes::Status       );
  EdgeAttributes    ( OpenMesh::Attributes::Status       );
  HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
  FaceAttributes    ( OpenMesh::Attributes::Normal       |
		      OpenMesh::Attributes::Status       );  
};

  
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  MyMesh;
typedef MeshViewerWidgetT<MyMesh>                 MeshViewerWidget;


//== CLASS DEFINITION =========================================================

	      

class ProgViewerWidget : public MeshViewerWidget
{
  Q_OBJECT
   
public:

  typedef MeshViewerWidget Base;
  typedef ProgViewerWidget This;


public:   
  /// default constructor
  ProgViewerWidget(QWidget* _parent=0)
    : MeshViewerWidget(_parent)
  {
    timer_ = new QTimer(this);

    connect( timer_, SIGNAL(timeout()), SLOT(animate()) );
  }

  /// destructor
  ~ProgViewerWidget()
  {
    delete timer_;
  }

  /// open progressive mesh
  void open_prog_mesh(const char* _filename);

protected slots:

void animate( void );
   
private:

  QTimer *timer_;
   
  struct PMInfo 
  {
    MyMesh::Point        p0;
    MyMesh::VertexHandle v0, v1, vl, vr;
  };
  typedef std::vector<PMInfo>          PMInfoContainer;
  typedef PMInfoContainer::iterator    PMInfoIter;

  /// refine mesh up to _n vertices
  void refine(unsigned int _n);

  /// coarsen mesh down to _n vertices
  void coarsen(unsigned int _n);

  virtual void keyPressEvent(QKeyEvent* _event);

  // mesh data
  bool              animateRefinement_;
  PMInfoContainer   pminfos_;
  PMInfoIter        pmiter_;
  size_t            n_base_vertices_, n_base_faces_, n_detail_vertices_;
  size_t            n_max_vertices_;
};


//=============================================================================
#endif // OPENMESHAPPS_PROGVIEWERWIDGET_HH defined
//=============================================================================


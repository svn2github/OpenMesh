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
//   $Revision: 1.3 $
//   $Date: 2008-03-11 09:18:01 $
//                                                                            
//=============================================================================


#ifndef OPENMESHAPPS_DECIMATERVIEWERWIDGET_HH
#define OPENMESHAPPS_DECIMATERVIEWERWIDGET_HH


//== INCLUDES =================================================================

#if !defined(OM_USE_OSG)
#  define OM_USE_OSG 0
#endif


#include <qtimer.h>
#include <string>
#include <memory>

//--------------------
#include <OpenMesh/Core/IO/MeshIO.hh>
//--------------------
#if OM_USE_OSG
#  include <OpenMesh/Tools/Kernel_OSG/TriMesh_OSGArrayKernelT.hh>
#  define DEFAULT_TRAITS Kernel_OSG::Traits
#  define TRIMESH_KERNEL Kernel_OSG::TriMesh_OSGArrayKernelT
#else
//--------------------
#  include <OpenMesh/Core/Mesh/Traits.hh>
#  include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#  define DEFAULT_TRAITS DefaultTraits
#  define TRIMESH_KERNEL TriMesh_ArrayKernelT
#endif

#include <OpenMesh/Apps/QtViewer/MeshViewerWidgetT.hh>

#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModNormalFlippingT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>


//== CLASS DEFINITION =========================================================


using namespace OpenMesh;

struct MyTraits : public DEFAULT_TRAITS
{
  VertexAttributes  ( Attributes::Normal       );
  FaceAttributes    ( Attributes::Normal       );  
};

typedef TRIMESH_KERNEL<MyTraits>                     mesh_t;
typedef MeshViewerWidgetT<mesh_t>                    MeshViewerWidget;

//== CLASS DEFINITION =========================================================


class DecimaterViewerWidget : public MeshViewerWidget
{
  Q_OBJECT
   
public:
  
  typedef MeshViewerWidget inherited_t;

  typedef Decimater::DecimaterT<mesh_t>                decimater_t;  
  typedef Decimater::ModQuadricT< decimater_t >        mod_quadric_t;
  typedef Decimater::ModNormalFlippingT< decimater_t > mod_nf_t;

  // object types
  typedef std::auto_ptr< decimater_t   >   decimater_o;
  typedef std::auto_ptr< mod_quadric_t >   mod_quadric_o;
  typedef std::auto_ptr< mod_nf_t      >   mod_nf_o;

  /// default constructor
  DecimaterViewerWidget(QWidget* _parent=0)
    : MeshViewerWidget(_parent), steps_(1)
  {
    timer_ = new QTimer(this);
    
    connect( timer_, SIGNAL(timeout()), SLOT(animate()) );
  }
  
  /// destructor
  ~DecimaterViewerWidget()
  {
    delete timer_;
  }

public: // inherited

  bool open_mesh(const char* _filename, OpenMesh::IO::Options _opt)
  {
    bool rc;

    if ( (rc = inherited_t::open_mesh( _filename, _opt )) )
    {
      std::cout << "prepare decimater" << std::endl;

      // to be compatible with gcc 2.95.3
      {
	decimater_o  tmp( new decimater_t  ( mesh() ) );
	decimater_ = tmp;
      }
      {
	mod_quadric_o  tmp( new mod_quadric_t( *decimater_ ) );
	mod_quadric_ = tmp;
      }
      {
	mod_nf_o       tmp( new mod_nf_t     ( *decimater_ ) );
	mod_nf_      = tmp;
      }

      decimater_->initialize();
    }
    return rc;
  }
  
protected slots:

  void animate( void );

protected:   

  virtual void keyPressEvent(QKeyEvent* _event);  


private:
  
  bool              animate_;
  QTimer           *timer_;

  decimater_o       decimater_;  
  mod_quadric_o     mod_quadric_;
  mod_nf_o          mod_nf_;

  size_t            steps_;
};


//=============================================================================
#endif // OPENMESHAPPS_DECIMATERVIEWERWIDGET_HH defined
//=============================================================================


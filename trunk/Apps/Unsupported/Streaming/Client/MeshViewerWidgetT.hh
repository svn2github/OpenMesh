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


#ifndef OPENMESHAPPS_MESHVIEWERWIDGETT_HH
#define OPENMESHAPPS_MESHVIEWERWIDGETT_HH


//== INCLUDES =================================================================

#include <string>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>
#include <OpenMesh/Core/Mesh/Attributes.hh>
#include <OpenMesh/Tools/Utils/StripifierT.hh>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Apps/VDProgMesh/Streaming/Client/QGLViewerWidget.hh>


//== FORWARDS =================================================================

class QImage;


//== CLASS DEFINITION =========================================================

	      
template <typename M>
class MeshViewerWidgetT : public QGLViewerWidget
{
public:
  typedef M                  Mesh;
  typedef OpenMesh::StripifierT<Mesh>   MyStripifier;


  /// default constructor
  MeshViewerWidgetT(QWidget* _parent=0, const char* _name=0)
    : QGLViewerWidget(_parent, _name),
      f_strips_(false), 
      tex_id_(0),
      tex_mode_(GL_MODULATE),
      strips_(mesh_)
  {
    add_draw_mode("Points");
    add_draw_mode("Hidden-Line");
#if defined(OM_USE_OSG) && OM_USE_OSG
    add_draw_mode("OpenSG Indices");
#endif
  }
  
  void enable_strips() { 
    f_strips_ = true;  
    add_draw_mode("Strips'n VertexArrays");
    add_draw_mode("Show Strips");    
  }
  void disable_strips() { f_strips_ = false; }
  
  /// destructor
  ~MeshViewerWidgetT() {}
  
  /// open mesh
  virtual bool open_mesh(const char* _filename, OpenMesh::IO::Options _opt);
  
  /// load texture
  virtual bool open_texture( const char *_filename );
  bool set_texture( QImage& _texsrc );
  
  
  Mesh& mesh() { return mesh_; }
  const Mesh& mesh() const { return mesh_; }
  
  
protected:
  
  /// inherited drawing method
  virtual void draw_scene(const std::string& _draw_mode);
  
protected:
  
  /// draw the mesh
  virtual void draw_openmesh(const std::string& _drawmode);
  
  void glVertex( const typename Mesh::VertexHandle vh )
  { glVertex3fv( &mesh_.point( vh )[0] ); }
  
  void glNormal( const typename Mesh::VertexHandle vh )
  { glNormal3fv( &mesh_.normal( vh )[0] ); }

  void glTexCoord( const typename Mesh::VertexHandle vh )
  { glTexCoord2fv( &mesh_.texcoord(vh)[0] ); }
  
  void glColor( const typename Mesh::VertexHandle vh )
  { glColor3ubv( &mesh_.color(vh)[0] ); }

  void glColor( const typename Mesh::FaceHandle fh )
  { glColor3ubv( &mesh_.color(fh)[0] ); }
  

  
protected: // Strip support
  
  void compute_strips(void)
  {
    if (f_strips_)
    {
      strips_.clear();
      strips_.stripify();
    }
  }    

protected: // inherited
   
  virtual void keyPressEvent( QKeyEvent* _event);

protected:
   
  bool                   f_strips_; // enable/disable strip usage
  GLuint                 tex_id_;
  GLint                  tex_mode_;
  OpenMesh::IO::Options  opt_; // mesh file contained texcoords?

  Mesh                   mesh_;
  MyStripifier           strips_;
};


//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESHAPPS_MESHVIEWERWIDGET_CC)
#  define OPENMESH_MESHVIEWERWIDGET_TEMPLATES
#  include "MeshViewerWidgetT.cc"
#endif
//=============================================================================
#endif // OPENMESHAPPS_MESHVIEWERWIDGETT_HH defined
//=============================================================================


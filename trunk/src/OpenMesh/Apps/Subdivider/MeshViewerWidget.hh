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
//   $Revision$
//   $Date$
//                                                                            
//=============================================================================


#ifndef OPENMESHAPPS_MESHVIEWERWIDGET_HH
#define OPENMESHAPPS_MESHVIEWERWIDGET_HH


//== INCLUDES =================================================================


// -------------------- OpenMesh
#include <OpenMesh/Apps/QtViewer/MeshViewerWidgetT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/Composite/CompositeTraits.hh>

//

typedef OpenMesh::Subdivider::Uniform::CompositeTraits Traits;
typedef OpenMesh::TriMesh_ArrayKernelT<Traits>         Mesh;



//== CLASS DEFINITION =========================================================

	      

class MeshViewerWidget : public MeshViewerWidgetT<Mesh>
{
public:
   typedef MeshViewerWidgetT<Mesh> Base;
   
  /// default constructor
  MeshViewerWidget(QWidget* _parent=0)
    : Base(_parent)
  {}

  /// destructor
  ~MeshViewerWidget() {}

  /// open mesh
  inline bool open_mesh(const char* _filename, OpenMesh::IO::Options _opt)
  {
    if ( Base::open_mesh( _filename, _opt ) )
    {
      orig_mesh_ = mesh_;
      return true;
    }
    return false;
  }

  Mesh& orig_mesh() { return orig_mesh_; }
  const Mesh& orig_mesh() const { return orig_mesh_; }
   
protected:

  Mesh orig_mesh_;

};


//=============================================================================
#endif // OPENMESHAPPS_MESHVIEWERWIDGET_HH defined
//=============================================================================


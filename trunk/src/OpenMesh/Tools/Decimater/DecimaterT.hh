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
//   by the Free Software Foundation, version 2.
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

/** \file DecimaterT.hh
 */

//=============================================================================
//
//  CLASS DecimaterT
//
//=============================================================================

#ifndef OPENMESH_DECIMATER_DECIMATERT_HH
#define OPENMESH_DECIMATER_DECIMATERT_HH


//== INCLUDES =================================================================

#include <memory>

#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Tools/Utils/HeapT.hh>
#include <OpenMesh/Tools/Decimater/ModBaseT.hh>



//== NAMESPACE ================================================================

namespace OpenMesh  {
namespace Decimater {


//== CLASS DEFINITION =========================================================


/** Decimater framework.
    \see BaseModT, \ref decimater_docu
*/
template < typename MeshT >
class DecimaterT
{
public: //-------------------------------------------------------- public types

  typedef DecimaterT< MeshT >        Self;
  typedef MeshT                      Mesh;
  typedef CollapseInfoT<MeshT>       CollapseInfo;
  typedef ModBaseT<Self>             Module;
  typedef std::vector< Module* >     ModuleList;

public: //------------------------------------------------------ public methods

  /// Constructor
  DecimaterT( Mesh& _mesh );

  /// Destructor
  ~DecimaterT();


  /** Initialize decimater and decimating modules.

      Return values:
      true   ok
      false  No ore more than one non-binary module exist. In that case
             the decimater is uninitialized!
   */
  bool initialize();


  /// Returns whether decimater has been successfully initialized.
  bool is_initialized() const { return initialized_; }


  /// Print information about modules to _os
  void info( std::ostream& _os );

public: //--------------------------------------------------- module management

  /// access mesh. used in modules.
  Mesh& mesh() { return mesh_; }

  /// add module to decimater
  template < typename _Module >
  bool add( ModHandleT<_Module>& _mh )
  {
    if (_mh.is_valid())
      return false;

    _mh.init( new _Module(*this) );
    bmodules_.push_back( _mh.module() );

    initialized_ = false;
    return true;
  }


  /// remove module
  template < typename _Module >
  bool remove( ModHandleT<_Module>& _mh )
  {
    if (!_mh.is_valid())
      return false;

    if ( cmodule_ == _mh.module() ) {
      cmodule_     = 0;
      initialized_ = false; // reset initialized state
      _mh.clear();
      return true;
    }

    typename ModuleList::iterator it = std::find(bmodules_.begin(),
             bmodules_.end(),
             _mh.module() );

    if ( it == bmodules_.end() ) // module not found
      return false;

    delete *it;
    bmodules_.erase( it ); // finally remove from list
    _mh.clear();

    initialized_ = false; // reset initialized state
    return true;
  }


  /// get module referenced by handle _mh
  template < typename Module >
  Module& module( ModHandleT<Module>& _mh )
  {
    assert( _mh.is_valid() );
    return *_mh.module();
  }

public:

  /** Decimate (perform _n_collapses collapses). Return number of
      performed collapses. If _n_collapses is not given reduce as
      much as possible */
  size_t decimate( size_t _n_collapses = 0 );

  /// Decimate to target complexity, returns number of collapses
  size_t decimate_to( size_t  _n_vertices )
  {
    return ( (_n_vertices < mesh().n_vertices()) ?
	     decimate( mesh().n_vertices() - _n_vertices ) : 0 );
  }


private:

  void update_modules(CollapseInfo& _ci)
  {
    typename ModuleList::iterator m_it, m_end = bmodules_.end();
    for (m_it = bmodules_.begin(); m_it != m_end; ++m_it)
      (*m_it)->postprocess_collapse(_ci);
    cmodule_->postprocess_collapse(_ci);
  }

public:

  typedef typename Mesh::VertexHandle    VertexHandle;
  typedef typename Mesh::HalfedgeHandle  HalfedgeHandle;

  /// Heap interface
  class HeapInterface
  {
  public:

    HeapInterface(Mesh&               _mesh,
      VPropHandleT<float> _prio,
      VPropHandleT<int>   _pos)
      : mesh_(_mesh), prio_(_prio), pos_(_pos)
    { }

    inline bool
    less( VertexHandle _vh0, VertexHandle _vh1 )
    { return mesh_.property(prio_, _vh0) < mesh_.property(prio_, _vh1); }

    inline bool
    greater( VertexHandle _vh0, VertexHandle _vh1 )
    { return mesh_.property(prio_, _vh0) > mesh_.property(prio_, _vh1); }

    inline int
    get_heap_position(VertexHandle _vh)
    { return mesh_.property(pos_, _vh); }

    inline void
    set_heap_position(VertexHandle _vh, int _pos)
    { mesh_.property(pos_, _vh) = _pos; }


  private:
    Mesh&                mesh_;
    VPropHandleT<float>  prio_;
    VPropHandleT<int>    pos_;
  };

  typedef Utils::HeapT<VertexHandle, HeapInterface>  DeciHeap;


private: //---------------------------------------------------- private methods

  /// Insert vertex in heap
  void heap_vertex(VertexHandle _vh);

  /// Is an edge collapse legal?  Performs topological test only.
  /// The method evaluates the status bit Locked, Deleted, and Feature.
  /// \attention The method temporarily sets the bit Tagged. After usage
  ///            the bit will be disabled!
  bool is_collapse_legal(const CollapseInfo& _ci);

  /// Calculate priority of an halfedge collapse (using the modules)
  float collapse_priority(const CollapseInfo& _ci);

  /// Post-process a collapse
  void postprocess_collapse(CollapseInfo& _ci);




private: //------------------------------------------------------- private data


  // reference to mesh
  Mesh&      mesh_;

  // heap
  std::auto_ptr<DeciHeap> heap_;

  // list of modules
  ModuleList bmodules_;
  Module*    cmodule_;

  bool       initialized_;


  // vertex properties
  VPropHandleT<HalfedgeHandle>  collapse_target_;
  VPropHandleT<float>           priority_;
  VPropHandleT<int>             heap_position_;



private: // Noncopyable

  DecimaterT(const Self&);
  Self& operator = (const Self&);

};

//=============================================================================
} // END_NS_DECIMATER
} // END_NS_OPENMESH
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_DECIMATER_DECIMATERT_CC)
#define OPENMESH_DECIMATER_TEMPLATES
#include "DecimaterT.cc"
#endif
//=============================================================================
#endif // OPENMESH_DECIMATER_DECIMATERT_HH defined
//=============================================================================


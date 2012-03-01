/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2011 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         * 
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/ 

/*===========================================================================*\
 *                                                                           *             
 *   $Revision$                                                         *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#ifndef OPENMESH_ITERATORS_HH
#define OPENMESH_ITERATORS_HH

//=============================================================================
//
//  Iterators for PolyMesh/TriMesh
//
//=============================================================================



//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Mesh/Status.hh>
#include <assert.h>
#include <cstddef>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Iterators {


//== FORWARD DECLARATIONS =====================================================


template <class Mesh> class ConstVertexIterT;
template <class Mesh> class VertexIterT;
template <class Mesh> class ConstHalfedgeIterT;
template <class Mesh> class HalfedgeIterT;
template <class Mesh> class ConstEdgeIterT;
template <class Mesh> class EdgeIterT;
template <class Mesh> class ConstFaceIterT;
template <class Mesh> class FaceIterT;




//== CLASS DEFINITION =========================================================
	      
/** \class VertexIterT IteratorsT.hh <OpenMesh/Mesh/Iterators/IteratorsT.hh>
    Linear iterator.
*/

template <class Mesh>
class ConstVertexIterT
{
public:
  

  //--- Typedefs ---

  typedef typename Mesh::VertexHandle           value_handle;
  typedef value_handle                          value_type;
  typedef std::bidirectional_iterator_tag       iterator_category;
  typedef std::ptrdiff_t                        difference_type;
  typedef const value_type&                     reference;
  typedef const value_type*                     pointer;
  typedef const Mesh*                           mesh_ptr;
  typedef const Mesh&                           mesh_ref;

  /// Default constructor.
  ConstVertexIterT()
    : mesh_(0), skip_bits_(0) 
  {}


  /// Construct with mesh and a target handle. 
  ConstVertexIterT(mesh_ref _mesh, value_handle _hnd, bool _skip=false)
    : mesh_(&_mesh), hnd_(_hnd), skip_bits_(0) 
  {
    if (_skip) enable_skipping();
    
    // Set vertex handle invalid if the mesh contains no vertex
    if(_mesh.n_vertices() == 0) hnd_ = value_handle(-1);
  }

  /// Standard dereferencing operator.
  reference operator*()  const { return hnd_; }
  
  /// Standard pointer operator.
  pointer   operator->() const { return &hnd_; }
  
  /// Get the handle of the item the iterator refers to.
  value_handle handle() const { return hnd_; }

  /// Cast to the handle of the item the iterator refers to.
  operator value_handle() const { return hnd_; }

  /// Are two iterators equal? Only valid if they refer to the same mesh!
  bool operator==(const ConstVertexIterT& _rhs) const
  { return ((mesh_ == _rhs.mesh_) && (hnd_ == _rhs.hnd_)); }

  /// Not equal?
  bool operator!=(const ConstVertexIterT& _rhs) const
  { return !operator==(_rhs); }
  
  /// Standard pre-increment operator
  ConstVertexIterT& operator++()
  { hnd_.__increment(); if (skip_bits_) skip_fwd(); return *this; }

  /// Standard pre-decrement operator
  ConstVertexIterT& operator--()
  { hnd_.__decrement(); if (skip_bits_) skip_bwd(); return *this; }
  

  /// Turn on skipping: automatically skip deleted/hidden elements
  void enable_skipping()
  {
    if (mesh_ && mesh_->has_vertex_status())
    {
      Attributes::StatusInfo status;
      status.set_deleted(true);
      status.set_hidden(true);
      skip_bits_ = status.bits();
      skip_fwd();
    }
    else skip_bits_ = 0;
  }


  /// Turn on skipping: automatically skip deleted/hidden elements
  void disable_skipping() { skip_bits_ = 0; }



private:

  void skip_fwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() < (signed) mesh_->n_vertices()) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__increment();
  }


  void skip_bwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() >= 0) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__decrement();
  }



protected:
  mesh_ptr      mesh_;
  value_handle  hnd_;
  unsigned int  skip_bits_;
};

// We would prefer a templated typedef here but that's C++11.
template<class Mesh>
class VertexIterT: public ConstVertexIterT<Mesh> {
    public:
        VertexIterT() {}
        VertexIterT(typename ConstVertexIterT<Mesh>::mesh_ref _mesh,
                         typename ConstVertexIterT<Mesh>::value_handle _hnd, bool _skip = false) :
                ConstVertexIterT<Mesh>(_mesh, _hnd, _skip) {}
};

//== CLASS DEFINITION =========================================================

	      
/** \class HalfedgeIterT IteratorsT.hh <OpenMesh/Mesh/Iterators/IteratorsT.hh>
    Linear iterator.
*/

template <class Mesh>
class ConstHalfedgeIterT
{
public:
  

  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle         value_handle;
  typedef value_handle                          value_type;

  typedef std::bidirectional_iterator_tag       iterator_category;
  typedef std::ptrdiff_t                        difference_type;
  typedef const value_type&                     reference;
  typedef const value_type*                     pointer;
  typedef const Mesh*                           mesh_ptr;
  typedef const Mesh&                           mesh_ref;




  /// Default constructor.
  ConstHalfedgeIterT()
    : mesh_(0), skip_bits_(0) 
  {}


  /// Construct with mesh and a target handle. 
  ConstHalfedgeIterT(mesh_ref _mesh, value_handle _hnd, bool _skip=false)
    : mesh_(&_mesh), hnd_(_hnd), skip_bits_(0) 
  {
    if (_skip) enable_skipping();
    
    // Set halfedge handle invalid if the mesh contains no edge
    if(_mesh.n_edges() == 0) hnd_ = value_handle(-1);
  }

  /// Standard dereferencing operator.
  reference operator*()  const { return hnd_; }
  
  /// Standard pointer operator.
  pointer   operator->() const { return &hnd_; }
  
  /// Get the handle of the item the iterator refers to.
  value_handle handle() const { return hnd_; }

  /// Cast to the handle of the item the iterator refers to.
  operator value_handle() const { return hnd_; }

  /// Are two iterators equal? Only valid if they refer to the same mesh!
  bool operator==(const ConstHalfedgeIterT& _rhs) const
  { return ((mesh_ == _rhs.mesh_) && (hnd_ == _rhs.hnd_)); }

  /// Not equal?
  bool operator!=(const ConstHalfedgeIterT& _rhs) const
  { return !operator==(_rhs); }
  
  /// Standard pre-increment operator
  ConstHalfedgeIterT& operator++()
  { hnd_.__increment(); if (skip_bits_) skip_fwd(); return *this; }

  /// Standard pre-decrement operator
  ConstHalfedgeIterT& operator--()
  { hnd_.__decrement(); if (skip_bits_) skip_bwd(); return *this; }
  

  /// Turn on skipping: automatically skip deleted/hidden elements
  void enable_skipping()
  {
    if (mesh_ && mesh_->has_halfedge_status())
    {
      Attributes::StatusInfo status;
      status.set_deleted(true);
      status.set_hidden(true);
      skip_bits_ = status.bits();
      skip_fwd();
    }
    else skip_bits_ = 0;
  }


  /// Turn on skipping: automatically skip deleted/hidden elements
  void disable_skipping() { skip_bits_ = 0; }



private:

  void skip_fwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() < (signed) mesh_->n_halfedges()) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__increment();
  }


  void skip_bwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() >= 0) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__decrement();
  }



private:
  mesh_ptr      mesh_;
  value_handle  hnd_;
  unsigned int  skip_bits_;
};


// We would prefer a templated typedef here but that's C++11.
template<class Mesh>
class HalfedgeIterT: public ConstHalfedgeIterT<Mesh> {
    public:
        HalfedgeIterT() {}

        HalfedgeIterT(typename ConstHalfedgeIterT<Mesh>::mesh_ref _mesh,
                      typename ConstHalfedgeIterT<Mesh>::value_handle _hnd, bool _skip = false) :
                ConstHalfedgeIterT<Mesh>(_mesh, _hnd, _skip) {}
};


//== CLASS DEFINITION =========================================================

	      
/** \class EdgeIterT IteratorsT.hh <OpenMesh/Mesh/Iterators/IteratorsT.hh>
    Linear iterator.
*/

template <class Mesh>
class ConstEdgeIterT
{
public:
  

  //--- Typedefs ---

  typedef typename Mesh::EdgeHandle             value_handle;
  typedef value_handle                          value_type;

  typedef std::bidirectional_iterator_tag       iterator_category;
  typedef std::ptrdiff_t                        difference_type;
  typedef const value_type&                     reference;
  typedef const value_type*                     pointer;
  typedef const Mesh*                           mesh_ptr;
  typedef const Mesh&                           mesh_ref;

  /// Default constructor.
  ConstEdgeIterT()
    : mesh_(0), skip_bits_(0) 
  {}


  /// Construct with mesh and a target handle. 
  ConstEdgeIterT(mesh_ref _mesh, value_handle _hnd, bool _skip=false)
    : mesh_(&_mesh), hnd_(_hnd), skip_bits_(0) 
  {
    if (_skip) enable_skipping();
    
    // Set halfedge handle invalid if the mesh contains no edge
    if(_mesh.n_edges() == 0) hnd_ = value_handle(-1);
  }


  /// Standard dereferencing operator.
  reference operator*()  const { return hnd_; }
  
  /// Standard pointer operator.
  pointer   operator->() const { return &hnd_; }
  
  /// Get the handle of the item the iterator refers to.
  value_handle handle() const { return hnd_; }

  /// Cast to the handle of the item the iterator refers to.
  operator value_handle() const { return hnd_; }

  /// Are two iterators equal? Only valid if they refer to the same mesh!
  bool operator==(const ConstEdgeIterT& _rhs) const
  { return ((mesh_ == _rhs.mesh_) && (hnd_ == _rhs.hnd_)); }

  /// Not equal?
  bool operator!=(const ConstEdgeIterT& _rhs) const
  { return !operator==(_rhs); }
  
  /// Standard pre-increment operator
  ConstEdgeIterT& operator++()
  { hnd_.__increment(); if (skip_bits_) skip_fwd(); return *this; }

  /// Standard pre-decrement operator
  ConstEdgeIterT& operator--()
  { hnd_.__decrement(); if (skip_bits_) skip_bwd(); return *this; }
  

  /// Turn on skipping: automatically skip deleted/hidden elements
  void enable_skipping()
  {
    if (mesh_ && mesh_->has_edge_status())
    {
      Attributes::StatusInfo status;
      status.set_deleted(true);
      status.set_hidden(true);
      skip_bits_ = status.bits();
      skip_fwd();
    }
    else skip_bits_ = 0;
  }


  /// Turn on skipping: automatically skip deleted/hidden elements
  void disable_skipping() { skip_bits_ = 0; }



private:

  void skip_fwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() < (signed) mesh_->n_edges()) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__increment();
  }


  void skip_bwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() >= 0) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__decrement();
  }



private:
  mesh_ptr      mesh_;
  value_handle  hnd_;
  unsigned int  skip_bits_;
};

// We would prefer a templated typedef here but that's C++11.
template<class Mesh>
class EdgeIterT: public ConstEdgeIterT<Mesh> {
    public:
        EdgeIterT() {}

        EdgeIterT(typename ConstEdgeIterT<Mesh>::mesh_ref _mesh,
                       typename ConstEdgeIterT<Mesh>::value_handle _hnd, bool _skip = false) :
                ConstEdgeIterT<Mesh>(_mesh, _hnd, _skip) {}
};

//== CLASS DEFINITION =========================================================

	      
/** \class FaceIterT IteratorsT.hh <OpenMesh/Mesh/Iterators/IteratorsT.hh>
    Linear iterator.
*/

template <class Mesh>
class ConstFaceIterT
{
public:
  

  //--- Typedefs ---

  typedef typename Mesh::FaceHandle             value_handle;
  typedef value_handle                          value_type;

  typedef std::bidirectional_iterator_tag       iterator_category;
  typedef std::ptrdiff_t                        difference_type;
  typedef const value_type&                     reference;
  typedef const value_type*                     pointer;
  typedef const Mesh*                           mesh_ptr;
  typedef const Mesh&                           mesh_ref;


  /// Default constructor.
  ConstFaceIterT()
    : mesh_(0), skip_bits_(0) 
  {}


  /// Construct with mesh and a target handle. 
  ConstFaceIterT(mesh_ref _mesh, value_handle _hnd, bool _skip=false)
    : mesh_(&_mesh), hnd_(_hnd), skip_bits_(0) 
  {
    if (_skip) enable_skipping();
    
    // Set face handle invalid if the mesh contains no faces
    if(_mesh.n_faces() == 0) hnd_ = value_handle(-1);
  }


  /// Standard dereferencing operator.
  reference operator*()  const { return hnd_; }
  
  /// Standard pointer operator.
  pointer   operator->() const { return &hnd_; }
  
  /// Get the handle of the item the iterator refers to.
  value_handle handle() const { return hnd_; }

  /// Cast to the handle of the item the iterator refers to.
  operator value_handle() const { return hnd_; }

  /// Are two iterators equal? Only valid if they refer to the same mesh!
  bool operator==(const ConstFaceIterT& _rhs) const
  { return ((mesh_ == _rhs.mesh_) && (hnd_ == _rhs.hnd_)); }

  /// Not equal?
  bool operator!=(const ConstFaceIterT& _rhs) const
  { return !operator==(_rhs); }
  
  /// Standard pre-increment operator
  ConstFaceIterT& operator++()
  { hnd_.__increment(); if (skip_bits_) skip_fwd(); return *this; }

  /// Standard pre-decrement operator
  ConstFaceIterT& operator--()
  { hnd_.__decrement(); if (skip_bits_) skip_bwd(); return *this; }
  

  /// Turn on skipping: automatically skip deleted/hidden elements
  void enable_skipping()
  {
    if (mesh_ && mesh_->has_face_status())
    {
      Attributes::StatusInfo status;
      status.set_deleted(true);
      status.set_hidden(true);
      skip_bits_ = status.bits();
      skip_fwd();
    }
    else skip_bits_ = 0;
  }


  /// Turn on skipping: automatically skip deleted/hidden elements
  void disable_skipping() { skip_bits_ = 0; }



private:

  void skip_fwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() < (signed) mesh_->n_faces()) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__increment();
  }


  void skip_bwd() 
  {
    assert(mesh_ && skip_bits_);
    while ((hnd_.idx() >= 0) && 
	   (mesh_->status(hnd_).bits() & skip_bits_))
      hnd_.__decrement();
  }



private:
  mesh_ptr      mesh_;
  value_handle  hnd_;
  unsigned int  skip_bits_;
};

// We would prefer a templated typedef here but that's C++11.
template<class Mesh>
class FaceIterT: public ConstFaceIterT<Mesh> {
    public:
        FaceIterT() {}

        FaceIterT(typename ConstFaceIterT<Mesh>::mesh_ref _mesh,
                       typename ConstFaceIterT<Mesh>::value_handle _hnd, bool _skip = false) :
                ConstFaceIterT<Mesh>(_mesh, _hnd, _skip) {}
};

//=============================================================================
} // namespace Iterators
} // namespace OpenMesh
//=============================================================================
#endif 
//=============================================================================

/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2012 by Computer Graphics Group, RWTH Aachen      *
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

#ifndef OPENMESH_CIRCULATORS_HH
#define OPENMESH_CIRCULATORS_HH
//=============================================================================
//
//  Vertex and Face circulators for PolyMesh/TriMesh
//
//=============================================================================



//== INCLUDES =================================================================

#include <OpenMesh/Core/System/config.h>
#include <cassert>
#include <cstddef>


//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Iterators {

template<class Mesh, class CenterEntityHandle>
class GenericCirculator_CenterEntityFnsT {
    public:
        static void increment(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter);
        static void decrement(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter);
};

template<class Mesh>
class GenericCirculator_CenterEntityFnsT<Mesh, typename Mesh::VertexHandle> {
    public:
        inline static void increment(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            heh = mesh->cw_rotated_halfedge_handle(heh);
            if (heh == start) ++lap_counter;
        }
        inline static void decrement(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            if (heh == start) --lap_counter;
            heh = mesh->ccw_rotated_halfedge_handle(heh);
        }
};

template<class Mesh>
class GenericCirculator_CenterEntityFnsT<Mesh, typename Mesh::FaceHandle> {
    public:
        inline static void increment(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            heh = mesh->next_halfedge_handle(heh);
            if (heh == start) ++lap_counter;
        }
        inline static void decrement(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            if (heh == start) --lap_counter;
            heh = mesh->prev_halfedge_handle(heh);
        }
};

template<class Mesh, class CenterEntityHandle, class ValueHandle>
class GenericCirculator_ValueHandleFnsT {
    public:
        inline static bool is_valid(const Mesh *mesh, const typename Mesh::HalfedgeHandle &heh, const typename Mesh::HalfedgeHandle &start, const int &lap_counter) {
            return heh.is_valid() && ((start != heh) || (lap_counter == 0));
        }
        inline static void init(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {};
        inline static void increment(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            GenericCirculator_CenterEntityFnsT<Mesh, CenterEntityHandle>::increment(mesh, heh, start, lap_counter);
        }
        inline static void decrement(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            GenericCirculator_CenterEntityFnsT<Mesh, CenterEntityHandle>::decrement(mesh, heh, start, lap_counter);
        }
};

template<class Mesh, class CenterEntityHandle>
class GenericCirculator_ValueHandleFnsT<Mesh, CenterEntityHandle, typename Mesh::FaceHandle> {
    public:
        inline static bool is_valid(const Mesh *mesh, const typename Mesh::HalfedgeHandle &heh, const typename Mesh::HalfedgeHandle &start, const int &lap_counter) {
            return heh.is_valid() && ((start != heh) || (lap_counter == 0));
        }
        inline static void init(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            if (is_valid(mesh, heh, start, lap_counter) && !mesh->face_handle(heh).is_valid())
                increment(mesh, heh, start, lap_counter);
        };
        inline static void increment(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            do {
                GenericCirculator_CenterEntityFnsT<Mesh, CenterEntityHandle>::increment(mesh, heh, start, lap_counter);
            } while (is_valid(mesh, heh, start, lap_counter) && !mesh->face_handle(heh).is_valid());
        }
        inline static void decrement(const Mesh *mesh, typename Mesh::HalfedgeHandle &heh, typename Mesh::HalfedgeHandle &start, int &lap_counter) {
            do {
                GenericCirculator_CenterEntityFnsT<Mesh, CenterEntityHandle>::decrement(mesh, heh, start, lap_counter);
            } while (is_valid(mesh, heh, start, lap_counter) && !mesh->face_handle(heh).is_valid());
        }
};

template<class Mesh>
class GenericCirculatorBaseT {
    public:
        typedef const Mesh* mesh_ptr;
        typedef const Mesh& mesh_ref;

    public:
        GenericCirculatorBaseT() : mesh_(0), lap_counter_(0) {}

        GenericCirculatorBaseT(mesh_ref mesh, HalfedgeHandle heh, bool end = false) :
            mesh_(&mesh), start_(heh), heh_(heh), lap_counter_(static_cast<int>(end)) {}

        GenericCirculatorBaseT(const GenericCirculatorBaseT &rhs) :
            mesh_(rhs.mesh_), start_(rhs.start_), heh_(rhs.heh_), lap_counter_(rhs.lap_counter_) {}

        inline typename Mesh::FaceHandle toFaceHandle() const {
            return mesh_->face_handle(heh_);
        }

        inline typename Mesh::EdgeHandle toEdgeHandle() const {
            return mesh_->edge_handle(heh_);
        }

        inline typename Mesh::HalfedgeHandle toHalfedgeHandle() const {
            return heh_;
        }

        inline typename Mesh::HalfedgeHandle toOppositeHalfedgeHandle() const {
            return mesh_->opposite_halfedge_handle(heh_);
        }

        inline typename Mesh::VertexHandle toVertexHandle() const {
            return mesh_->to_vertex_handle(heh_);
        }

        inline GenericCirculatorBaseT &operator=(const GenericCirculatorBaseT &rhs) {
            mesh_ = rhs.mesh_;
            start_ = rhs.start_;
            heh_ = rhs.heh_;
            lap_counter_ = rhs.lap_counter_;
            return *this;
        }

        inline bool operator==(const GenericCirculatorBaseT &rhs) const {
            return mesh_ == rhs.mesh_ && start_ == rhs.start_ && heh_ == rhs.heh_ && lap_counter_ == rhs.lap_counter_;
        }

        inline bool operator!=(const GenericCirculatorBaseT &rhs) const {
            return !operator==(rhs);
        }

    protected:
        mesh_ptr mesh_;
        typename Mesh::HalfedgeHandle start_, heh_;
        int lap_counter_;
};

template<class Mesh, class CenterEntityHandle, class ValueHandle,
//        void (GenericCirculatorBaseT<Mesh>::*Increment)(),
//        void (GenericCirculatorBaseT<Mesh>::*Decrement)(),
        ValueHandle (GenericCirculatorBaseT<Mesh>::*Handle2Value)() const>
class GenericCirculatorT : protected GenericCirculatorBaseT<Mesh> {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef ValueHandle value_type;
        typedef const value_type& reference;
        typedef const value_type* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        typedef typename GenericCirculatorBaseT<Mesh>::mesh_ptr mesh_ptr;
        typedef typename GenericCirculatorBaseT<Mesh>::mesh_ref mesh_ref;
        typedef GenericCirculator_ValueHandleFnsT<Mesh, CenterEntityHandle, ValueHandle> GenericCirculator_ValueHandleFns;

    public:
        GenericCirculatorT() {}
        GenericCirculatorT(mesh_ref mesh, CenterEntityHandle start, bool end = false) :
            GenericCirculatorBaseT<Mesh>(mesh, mesh.halfedge_handle(start), end) {

            GenericCirculator_ValueHandleFns::init(this->mesh_, this->heh_, this->start_, this->lap_counter_);
        }
        GenericCirculatorT(mesh_ref mesh, HalfedgeHandle heh, bool end = false) :
            GenericCirculatorBaseT<Mesh>(mesh, heh, end) {

            GenericCirculator_ValueHandleFns::init(this->mesh_, this->heh_, this->start_, this->lap_counter_);
        }
        GenericCirculatorT(const GenericCirculatorT &rhs) : GenericCirculatorBaseT<Mesh>(rhs) {}

        GenericCirculatorT& operator++() {
            assert(this->mesh_);
            GenericCirculator_ValueHandleFns::increment(this->mesh_, this->heh_, this->start_, this->lap_counter_);
            return *this;
        }
        GenericCirculatorT& operator--() {
            assert(this->mesh_);
            GenericCirculator_ValueHandleFns::decrement(this->mesh_, this->heh_, this->start_, this->lap_counter_);
            return *this;
        }

        /// Post-increment
        GenericCirculatorT operator++(int) {
            assert(this->mesh_);
            GenericCirculatorT cpy(*this);
            ++(*this);
            return cpy;
        }

        /// Post-decrement
        GenericCirculatorT operator--(int) {
            assert(this->mesh_);
            GenericCirculatorT cpy(*this);
            --(*this);
            return cpy;
        }

        /// Standard dereferencing operator.
        value_type operator*() const {
            return (this->*Handle2Value)();
        }

        /// Standard pointer operator.
        value_type operator->() const {
            return (this->*Handle2Value)();
        }

        GenericCirculatorT &operator=(const GenericCirculatorT &rhs) {
            GenericCirculatorBaseT<Mesh>::operator=(rhs);
            return *this;
        };

        bool operator==(const GenericCirculatorT &rhs) const {
            return GenericCirculatorBaseT<Mesh>::operator==(rhs);
        }

        bool operator!=(const GenericCirculatorT &rhs) const {
            return GenericCirculatorBaseT<Mesh>::operator!=(rhs);
        }

        bool is_valid() const {
            return GenericCirculator_ValueHandleFns::is_valid(this->mesh_, this->heh_, this->start_, this->lap_counter_);
        }

        DEPRECATED("current_halfedge_handle() is an implementation detail and should not be accessed from outside the iterator class.")
        const HalfedgeHandle &current_halfedge_handle() const {
            return this->heh_;
        }

        DEPRECATED("Do not use this error prone implicit cast. Compare to end-iterator, instead.")
        operator bool() const {
            return is_valid();
        }

        /// Return the handle of the current target.
        DEPRECATED("This function clutters your code. Use dereferencing operators -> and * instead.")
        value_type handle() const {
          return **this;
        }

        /// Cast to the handle of the current target.
        DEPRECATED("Implicit casts of iterators are unsafe. Use dereferencing operators -> and * instead.")
        operator value_type() const {
          return **this;
        }
};

//== FORWARD DECLARATIONS =====================================================

template <class Mesh> class FaceVertexIterT;
template <class Mesh> class FaceHalfedgeIterT;
template <class Mesh> class FaceEdgeIterT;
template <class Mesh> class FaceFaceIterT;

template <class Mesh> class ConstFaceVertexIterT;
template <class Mesh> class ConstFaceHalfedgeIterT;
template <class Mesh> class ConstFaceEdgeIterT;
template <class Mesh> class ConstFaceFaceIterT;

//== CLASS DEFINITION =========================================================


/** \class FaceVertexIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceVertexIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Vertex             value_type;
  typedef typename Mesh::VertexHandle       value_handle;

#if 0
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Vertex&      reference;
  typedef const typename Mesh::Vertex*      pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Vertex&            reference;
  typedef typename Mesh::Vertex*            pointer;
#endif



  /// Default constructor
  FaceVertexIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceVertexIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  {  ; }


  /// Construct with mesh and start halfedge
  FaceVertexIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  {  ; }


  /// Copy constructor
  FaceVertexIterT(const FaceVertexIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// Assignment operator
  FaceVertexIterT& operator=(const FaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceVertexIterT(const FaceVertexIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// assign from non-const circulator
  FaceVertexIterT& operator=(const FaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceVertexIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceVertexIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
            (start_  == _rhs.start_) &&
            (heh_    == _rhs.heh_) &&
            (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const FaceVertexIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceVertexIterT& operator++() { 
    assert(mesh_);
    heh_=mesh_->next_halfedge_handle(heh_);
    if(heh_ == start_) lap_counter_++;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceVertexIterT& operator--() {
    assert(mesh_);
    if(heh_ == start_) lap_counter_--;
    heh_=mesh_->prev_halfedge_handle(heh_);
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::VertexHandle handle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::VertexHandle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceVertexIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceVertexIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Vertex             value_type;
  typedef typename Mesh::VertexHandle       value_handle;

#if 1
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Vertex&      reference;
  typedef const typename Mesh::Vertex*      pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Vertex&            reference;
  typedef typename Mesh::Vertex*            pointer;
#endif



  /// Default constructor
  ConstFaceVertexIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceVertexIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstFaceVertexIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  {  ; }


  /// Copy constructor
  ConstFaceVertexIterT(const ConstFaceVertexIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// Assignment operator
  ConstFaceVertexIterT& operator=(const ConstFaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceVertexIterT(const FaceVertexIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// assign from non-const circulator
  ConstFaceVertexIterT& operator=(const FaceVertexIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceVertexIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceVertexIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceVertexIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceVertexIterT& operator++() { 
    assert(mesh_);
    heh_=mesh_->next_halfedge_handle(heh_);
    if(heh_ == start_) lap_counter_++;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceVertexIterT& operator--() { 
    assert(mesh_);
    if(heh_ == start_) lap_counter_--;
    heh_=mesh_->prev_halfedge_handle(heh_);
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::VertexHandle handle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::VertexHandle() const {
    assert(mesh_);
    return mesh_->to_vertex_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle     value_handle;

#if 0
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Halfedge&    reference;
  typedef const typename Mesh::Halfedge*    pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Halfedge&          reference;
  typedef typename Mesh::Halfedge*          pointer;
#endif



  /// Default constructor
  FaceHalfedgeIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceHalfedgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  {  ; }


  /// Construct with mesh and start halfedge
  FaceHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  {  ; }


  /// Copy constructor
  FaceHalfedgeIterT(const FaceHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// Assignment operator
  FaceHalfedgeIterT& operator=(const FaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceHalfedgeIterT(const FaceHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// assign from non-const circulator
  FaceHalfedgeIterT& operator=(const FaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const FaceHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceHalfedgeIterT& operator++() { 
    assert(mesh_);
    heh_=mesh_->next_halfedge_handle(heh_);
    if(heh_ == start_) lap_counter_++;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceHalfedgeIterT& operator--() { 
    assert(mesh_);
    if(heh_ == start_) lap_counter_--;
    heh_=mesh_->prev_halfedge_handle(heh_);
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns.
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return heh_; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return heh_; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceHalfedgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceHalfedgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Halfedge           value_type;
  typedef typename Mesh::HalfedgeHandle     value_handle;

#if 1
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Halfedge&    reference;
  typedef const typename Mesh::Halfedge*    pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Halfedge&          reference;
  typedef typename Mesh::Halfedge*          pointer;
#endif



  /// Default constructor
  ConstFaceHalfedgeIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceHalfedgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstFaceHalfedgeIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  {  ; }


  /// Copy constructor
  ConstFaceHalfedgeIterT(const ConstFaceHalfedgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// Assignment operator
  ConstFaceHalfedgeIterT& operator=(const ConstFaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceHalfedgeIterT(const FaceHalfedgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// assign from non-const circulator
  ConstFaceHalfedgeIterT& operator=(const FaceHalfedgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceHalfedgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceHalfedgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceHalfedgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceHalfedgeIterT& operator++() { 
    assert(mesh_);
    heh_=mesh_->next_halfedge_handle(heh_);
    if(heh_ == start_) lap_counter_++;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceHalfedgeIterT& operator--() { 
    assert(mesh_);
    if(heh_ == start_) lap_counter_--;
    heh_=mesh_->prev_halfedge_handle(heh_);
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::HalfedgeHandle handle() const {
    assert(mesh_);
    return heh_; 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::HalfedgeHandle() const {
    assert(mesh_);
    return heh_; 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceEdgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceEdgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Edge               value_type;
  typedef typename Mesh::EdgeHandle         value_handle;

#if 0
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Edge&        reference;
  typedef const typename Mesh::Edge*        pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Edge&              reference;
  typedef typename Mesh::Edge*              pointer;
#endif



  /// Default constructor
  FaceEdgeIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceEdgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  {  ; }


  /// Construct with mesh and start halfedge
  FaceEdgeIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  {  ; }


  /// Copy constructor
  FaceEdgeIterT(const FaceEdgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// Assignment operator
  FaceEdgeIterT& operator=(const FaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceEdgeIterT(const FaceEdgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// assign from non-const circulator
  FaceEdgeIterT& operator=(const FaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceEdgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceEdgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const FaceEdgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceEdgeIterT& operator++() { 
    assert(mesh_);
    heh_=mesh_->next_halfedge_handle(heh_);
    if(heh_ == start_) lap_counter_++;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceEdgeIterT& operator--() { 
    assert(mesh_);
    if(heh_ == start_) lap_counter_--;
    heh_=mesh_->prev_halfedge_handle(heh_);
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::EdgeHandle handle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::EdgeHandle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceEdgeIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceEdgeIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Edge               value_type;
  typedef typename Mesh::EdgeHandle         value_handle;

#if 1
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Edge&        reference;
  typedef const typename Mesh::Edge*        pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Edge&              reference;
  typedef typename Mesh::Edge*              pointer;
#endif



  /// Default constructor
  ConstFaceEdgeIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceEdgeIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  {  ; }


  /// Construct with mesh and start halfedge
  ConstFaceEdgeIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  {  ; }


  /// Copy constructor
  ConstFaceEdgeIterT(const ConstFaceEdgeIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// Assignment operator
  ConstFaceEdgeIterT& operator=(const ConstFaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceEdgeIterT(const FaceEdgeIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  {  ; }


  /// assign from non-const circulator
  ConstFaceEdgeIterT& operator=(const FaceEdgeIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceEdgeIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceEdgeIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceEdgeIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceEdgeIterT& operator++() { 
    assert(mesh_);
    heh_=mesh_->next_halfedge_handle(heh_);
    if(heh_ == start_) lap_counter_++;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceEdgeIterT& operator--() { 
    assert(mesh_);
    if(heh_ == start_) lap_counter_--;
    heh_=mesh_->prev_halfedge_handle(heh_);
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::EdgeHandle handle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::EdgeHandle() const {
    assert(mesh_);
    return mesh_->edge_handle(heh_); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class FaceFaceIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class FaceFaceIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Face               value_type;
  typedef typename Mesh::FaceHandle         value_handle;

#if 0
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Face&        reference;
  typedef const typename Mesh::Face*        pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Face&              reference;
  typedef typename Mesh::Face*              pointer;
#endif



  /// Default constructor
  FaceFaceIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  FaceFaceIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Construct with mesh and start halfedge
  FaceFaceIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Copy constructor
  FaceFaceIterT(const FaceFaceIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Assignment operator
  FaceFaceIterT& operator=(const FaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 0
  /// construct from non-const circulator type
  FaceFaceIterT(const FaceFaceIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// assign from non-const circulator
  FaceFaceIterT& operator=(const FaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceFaceIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const FaceFaceIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const FaceFaceIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  FaceFaceIterT& operator++() { 
    assert(mesh_);
    do { heh_=mesh_->next_halfedge_handle(heh_); if(heh_ == start_) lap_counter_++; } while ((*this) && (!handle().is_valid()));
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  FaceFaceIterT& operator--() { 
    assert(mesh_);
    do { if(heh_ == start_) lap_counter_--; heh_=mesh_->prev_halfedge_handle(heh_); } while ((*this) && (!handle().is_valid()));
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::FaceHandle handle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::FaceHandle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const {
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//== CLASS DEFINITION =========================================================

	      
/** \class ConstFaceFaceIterT CirculatorsT.hh <OpenMesh/Mesh/Iterators/CirculatorsT.hh>
    Circulator.
*/

template <class Mesh>
class ConstFaceFaceIterT
{
 public:


  //--- Typedefs ---

  typedef typename Mesh::HalfedgeHandle     HalfedgeHandle;

  typedef typename Mesh::Face               value_type;
  typedef typename Mesh::FaceHandle         value_handle;

#if 1
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef const Mesh&                       mesh_ref;
  typedef const Mesh*                       mesh_ptr;
  typedef const typename Mesh::Face&        reference;
  typedef const typename Mesh::Face*        pointer;
#else
  typedef std::bidirectional_iterator_tag   iterator_category;
  typedef std::ptrdiff_t                    difference_type;
  typedef Mesh&                             mesh_ref;
  typedef Mesh*                             mesh_ptr;
  typedef typename Mesh::Face&              reference;
  typedef typename Mesh::Face*              pointer;
#endif



  /// Default constructor
  ConstFaceFaceIterT() : mesh_(0), lap_counter_(false) {}


  /// Construct with mesh and a typename Mesh::FaceHandle
  ConstFaceFaceIterT(mesh_ref _mesh, typename Mesh::FaceHandle _start, bool _end = false) :
    mesh_(&_mesh), 
    start_(_mesh.halfedge_handle(_start)),
    heh_(start_),
    lap_counter_(_end)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Construct with mesh and start halfedge
  ConstFaceFaceIterT(mesh_ref _mesh, HalfedgeHandle _heh, bool _end = false) :
    mesh_(&_mesh),
    start_(_heh),
    heh_(_heh),
    lap_counter_(_end)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Copy constructor
  ConstFaceFaceIterT(const ConstFaceFaceIterT& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// Assignment operator
  ConstFaceFaceIterT& operator=(const ConstFaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }


#if 1
  /// construct from non-const circulator type
  ConstFaceFaceIterT(const FaceFaceIterT<Mesh>& _rhs) :
    mesh_(_rhs.mesh_),
    start_(_rhs.start_),
    heh_(_rhs.heh_),
    lap_counter_(_rhs.lap_counter_)
  { if (heh_.is_valid() && !handle().is_valid()) operator++();; }


  /// assign from non-const circulator
  ConstFaceFaceIterT& operator=(const FaceFaceIterT<Mesh>& _rhs)
  {
    mesh_   = _rhs.mesh_;
    start_  = _rhs.start_;
    heh_    = _rhs.heh_;
    lap_counter_ = _rhs.lap_counter_;
    return *this;
  }
#else
  friend class ConstFaceFaceIterT<Mesh>;
#endif  


  /// Equal ?
  bool operator==(const ConstFaceFaceIterT& _rhs) const {
    return ((mesh_   == _rhs.mesh_) &&
	    (start_  == _rhs.start_) &&
	    (heh_    == _rhs.heh_) &&
	    (lap_counter_ == _rhs.lap_counter_));
  }


  /// Not equal ?
  bool operator!=(const ConstFaceFaceIterT& _rhs) const {
    return !operator==(_rhs);
  }


  /// Pre-Increment (next cw target)
  ConstFaceFaceIterT& operator++() { 
    assert(mesh_);
    do { heh_=mesh_->next_halfedge_handle(heh_); if(heh_ == start_) lap_counter_++; } while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /// Pre-Decrement (next ccw target)
  ConstFaceFaceIterT& operator--() { 
    assert(mesh_);
    do { if(heh_ == start_) lap_counter_--; heh_=mesh_->prev_halfedge_handle(heh_); } while ((*this) && (!handle().is_valid()));;
    return *this;
  }


  /** Get the current halfedge. There are \c Vertex*Iters and \c
      Face*Iters.  For both the current state is defined by the
      current halfedge. This is what this method returns. 
  */
  HalfedgeHandle current_halfedge_handle() const {
    return heh_;
  }


  /// Return the handle of the current target.
  typename Mesh::FaceHandle handle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }


  /// Cast to the handle of the current target.
  operator typename Mesh::FaceHandle() const {
    assert(mesh_);
    return mesh_->face_handle(mesh_->opposite_halfedge_handle(heh_)); 
  }
    

  ///  Return a reference to the current target.
  reference operator*() const { 
    assert(mesh_);
    return mesh_->deref(handle());
  }


  /// Return a pointer to the current target.
  pointer operator->() const { 
    assert(mesh_);
    return &mesh_->deref(handle());
  }


  /** Returns whether the circulator is still valid.
      After one complete round around a vertex/face the circulator becomes
      invalid, i.e. this function will return \c false. Nevertheless you
      can continue circulating. This method just tells you whether you
      have completed the first round.
   */
  operator bool() const { 
    return heh_.is_valid() && ((start_ != heh_) || (lap_counter_ == 0));
  }


protected:

  mesh_ptr         mesh_;
  HalfedgeHandle   start_, heh_;
  int              lap_counter_;
};



//=============================================================================
} // namespace Iterators
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================

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
 *   $Revision: 460 $                                                         *
 *   $Date: 2011-11-16 10:45:08 +0100 (Mi, 16 Nov 2011) $                   *
 *                                                                           *
 \*===========================================================================*/

/** \file McDecimaterT.cc
 */

//=============================================================================
//
//  CLASS McDecimaterT - IMPLEMENTATION
//
//=============================================================================
#define OPENMESH_MULTIPLE_CHOICE_DECIMATER_DECIMATERT_CC

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Decimater/McDecimaterT.hh>

#include <vector>
#if defined(OM_CC_MIPS)
#  include <float.h>
#else
#  include <cfloat>
#endif
#ifdef USE_OPENMP
#include <omp.h>
#endif

//== NAMESPACE ===============================================================

namespace OpenMesh {
namespace Decimater {

//== IMPLEMENTATION ==========================================================

template<class Mesh>
McDecimaterT<Mesh>::McDecimaterT(Mesh& _mesh) :
  BaseDecimaterT<Mesh>(_mesh),
    mesh_(_mesh), randomSamples_(10) {

  // default properties
  mesh_.request_vertex_status();
  mesh_.request_halfedge_status();
  mesh_.request_edge_status();
  mesh_.request_face_status();
  mesh_.request_face_normals();

}

//-----------------------------------------------------------------------------

template<class Mesh>
McDecimaterT<Mesh>::~McDecimaterT() {
  // default properties
  mesh_.release_vertex_status();
  mesh_.release_edge_status();
  mesh_.release_halfedge_status();
  mesh_.release_face_status();
  mesh_.release_face_normals();

}

//-----------------------------------------------------------------------------
template<class Mesh>
size_t McDecimaterT<Mesh>::decimate(size_t _n_collapses) {

  if (!this->is_initialized())
    return 0;

  unsigned int n_collapses(0);

  bool collapsesUnchanged = false;
  // old n_collapses in order to check for convergence
  unsigned int oldCollapses = 0;
  // number of iterations where no new collapses where
  // performed in a row
  unsigned int noCollapses = 0;

  while ( n_collapses <  _n_collapses) {
    if (noCollapses > 20) {
      omlog() << "[McDecimater] : no collapses performed in over 20 iterations in a row\n";
      break;
    }

    // Optimal id and value will be collected during the random sampling
    typename Mesh::HalfedgeHandle bestHandle(-1);
    typename Mesh::HalfedgeHandle tmpHandle(-1);
    double bestEnergy = FLT_MAX;
    double energy = FLT_MAX;

    // Generate random samples for collapses
#ifdef USE_OPENMP
#pragma omp parallel for private(energy,tmpHandle) shared(bestEnergy,bestHandle)
#endif
    for ( int i = 0; i < (int)randomSamples_; ++i) {

      // Random halfedge handle
      tmpHandle = typename Mesh::HalfedgeHandle((static_cast<double>(rand()) / RAND_MAX) * (mesh_.n_halfedges()-1) );

      // if it is not deleted, we analyse it
      if ( ! mesh_.status(tmpHandle).deleted()  ) {

        CollapseInfo ci(mesh_, tmpHandle);

        // Check if legal we analyze the priority of this collapse operation
        if (this->is_collapse_legal(ci)) {
#ifdef USE_OPENMP
#pragma omp critical(energyUpdate)
          {
#endif
          energy = this->collapse_priority(ci);

          if (energy != ModBaseT<Mesh>::ILLEGAL_COLLAPSE) {
            // Check if the current samples energy is better than any energy before
            if ( energy < bestEnergy ) {
              bestEnergy = energy;
              bestHandle = tmpHandle;
            }
          }
#ifdef USE_OPENMP
          }
#endif
        } else {
          continue;
        }
      }

    }

    // Found the best energy?
    if ( bestEnergy != FLT_MAX ) {

      // setup collapse info
      CollapseInfo ci(mesh_, bestHandle);

      // check topological correctness AGAIN !
      if (!this->is_collapse_legal(ci))
        continue;

      // pre-processing
      this->preprocess_collapse(ci);

      // perform collapse
      mesh_.collapse(bestHandle);
      ++n_collapses;

      // store current collapses state
      oldCollapses = n_collapses;
      noCollapses = 0;
      collapsesUnchanged = false;

      // update triangle normals
      typename Mesh::VertexFaceIter vf_it = mesh_.vf_iter(ci.v1);
      for (; vf_it; ++vf_it)
        if (!mesh_.status(vf_it).deleted())
          mesh_.set_normal(vf_it, mesh_.calc_face_normal(vf_it.handle()));

      // post-process collapse
      this->postprocess_collapse(ci);

    } else {
      if (oldCollapses == n_collapses) {
        if (collapsesUnchanged == false) {
          noCollapses = 1;
          collapsesUnchanged = true;
        } else {
          noCollapses++;
        }
      }
    }

  }

  // DON'T do garbage collection here! It's up to the application.
  return n_collapses;
}

//-----------------------------------------------------------------------------

template<class Mesh>
size_t McDecimaterT<Mesh>::decimate_to_faces(size_t _nv, size_t _nf) {
  if (!this->is_initialized())
    return 0;

  // check if no vertex or face contraints were set
  if ( (_nv == 0) && (_nf == 1) )
    return decimate_constraints_only(1.0);

  unsigned int nv = mesh_.n_vertices();
  unsigned int nf = mesh_.n_faces();
  unsigned int n_collapses(0);


  bool collapsesUnchanged = false;
  // old n_collapses in order to check for convergence
  unsigned int oldCollapses = 0;
  // number of iterations where no new collapses where
  // performed in a row
  unsigned int noCollapses = 0;

  while ( (_nv < nv) && (_nf < nf) ) {
    if (noCollapses > 20) {
      omlog() << "[McDecimater] : no collapses performed in over 20 iterations in a row\n";
      break;
    }

    // Optimal id and value will be collected during the random sampling
    typename Mesh::HalfedgeHandle bestHandle(-1);
    typename Mesh::HalfedgeHandle tmpHandle(-1);
    double bestEnergy = FLT_MAX;
    double energy = FLT_MAX;


    // Generate random samples for collapses
#ifdef USE_OPENMP
#pragma omp parallel for private(energy,tmpHandle) shared(bestEnergy,bestHandle)
#endif
    for ( int i = 0; i < (int)randomSamples_; ++i) {

      // Random halfedge handle
      tmpHandle = typename Mesh::HalfedgeHandle((static_cast<double>(rand()) / RAND_MAX) * (mesh_.n_halfedges()-1) );

      // if it is not deleted, we analyse it
      if ( ! mesh_.status(tmpHandle).deleted()  ) {

        CollapseInfo ci(mesh_, tmpHandle);

        // Check if legal we analyze the priority of this collapse operation
        if (this->is_collapse_legal(ci)) {
#ifdef USE_OPENMP
#pragma omp critical(energyUpdate)
          {
#endif
          energy = this->collapse_priority(ci);

          if (energy != ModBaseT<Mesh>::ILLEGAL_COLLAPSE) {
            // Check if the current samples energy is better than any energy before
            if ( energy < bestEnergy ) {
              bestEnergy = energy;
              bestHandle = tmpHandle;
            }
            }
#ifdef USE_OPENMP
          }
#endif
        } else {
          continue;
        }
      }

    }

    // Found the best energy?
    if ( bestEnergy != FLT_MAX ) {

      // setup collapse info
      CollapseInfo ci(mesh_, bestHandle);

      // check topological correctness AGAIN !
      if (!this->is_collapse_legal(ci))
        continue;

      // adjust complexity in advance (need boundary status)

      // One vertex is killed by the collapse
      --nv;

      // If we are at a boundary, one face is lost,
      // otherwise two
      if (mesh_.is_boundary(ci.v0v1) || mesh_.is_boundary(ci.v1v0))
        --nf;
      else
        nf -= 2;


      // pre-processing
      this->preprocess_collapse(ci);

      // perform collapse
      mesh_.collapse(bestHandle);
      ++n_collapses;

      // store current collapses state
      oldCollapses = n_collapses;
      noCollapses = 0;
      collapsesUnchanged = false;

      // update triangle normals
      typename Mesh::VertexFaceIter vf_it = mesh_.vf_iter(ci.v1);
      for (; vf_it; ++vf_it)
        if (!mesh_.status(vf_it).deleted())
          mesh_.set_normal(vf_it, mesh_.calc_face_normal(vf_it.handle()));

      // post-process collapse
      this->postprocess_collapse(ci);

    } else {
      if (oldCollapses == n_collapses) {
        if (collapsesUnchanged == false) {
          noCollapses = 1;
          collapsesUnchanged = true;
        } else {
          noCollapses++;
        }
      }
    }

  }

  // DON'T do garbage collection here! It's up to the application.
  return n_collapses;
}

//-----------------------------------------------------------------------------

template<class Mesh>
size_t McDecimaterT<Mesh>::decimate_constraints_only(float _factor) {
  if (!this->is_initialized())
    return 0;

  if (_factor < 1.0)
    this->set_error_tolerance_factor(_factor);

  unsigned int n_collapses(0);
  unsigned int nv = mesh_.n_vertices();
  unsigned int nf = mesh_.n_faces();

  bool lastCollapseIllegal = false;
  // number of illegal collapses that occured in a row
  unsigned int illegalCollapses = 0;

  bool collapsesUnchanged = false;
  // old n_collapses in order to check for convergence
  unsigned int oldCollapses = 0;
  // number of iterations where no new collapses where
  // performed in a row
  unsigned int noCollapses = 0;


  while ( (noCollapses <= 20) && (illegalCollapses <= 10) && (nv > 0) && (nf > 1) ) {

    // Optimal id and value will be collected during the random sampling
    typename Mesh::HalfedgeHandle bestHandle(-1);
    typename Mesh::HalfedgeHandle tmpHandle(-1);
    double bestEnergy = FLT_MAX;
    double energy = FLT_MAX;

    // Generate random samples for collapses
#ifdef USE_OPENMP
#pragma omp parallel for private(energy,tmpHandle) shared(bestEnergy,bestHandle)
#endif
    for ( int i = 0; i < (int)randomSamples_; ++i) {

      // Random halfedge handle
      tmpHandle = typename Mesh::HalfedgeHandle((static_cast<double>(rand()) / RAND_MAX) * (mesh_.n_halfedges()-1) );

      // if it is not deleted, we analyse it
      if ( ! mesh_.status(tmpHandle).deleted()  ) {

        CollapseInfo ci(mesh_, tmpHandle);

        // Check if legal we analyze the priority of this collapse operation
        if (this->is_collapse_legal(ci)) {
#ifdef USE_OPENMP
#pragma omp critical(energyUpdate)
          {
#endif

          energy = this->collapse_priority(ci);

          if (energy == ModBaseT<Mesh>::ILLEGAL_COLLAPSE){
            if (lastCollapseIllegal) {
              illegalCollapses++;
            } else {
              illegalCollapses = 1;
              lastCollapseIllegal = true;
            }
          } else {
            illegalCollapses = 0;
            lastCollapseIllegal = false;

            // Check if the current samples energy is better than any energy before
            if ( energy < bestEnergy ) {
              bestEnergy = energy;
              bestHandle = tmpHandle;
            }
          }
#ifdef USE_OPENMP
          }
#endif
        } else {
          continue;
        }
      }

    }

    // Found the best energy?
    if ( bestEnergy != FLT_MAX ) {

      // setup collapse info
      CollapseInfo ci(mesh_, bestHandle);

      // check topological correctness AGAIN !
      if (!this->is_collapse_legal(ci))
        continue;

      // adjust complexity in advance (need boundary status)

      // One vertex is killed by the collapse
      --nv;

      // If we are at a boundary, one face is lost,
      // otherwise two
      if (mesh_.is_boundary(ci.v0v1) || mesh_.is_boundary(ci.v1v0))
        --nf;
      else
        nf -= 2;

      // pre-processing
      this->preprocess_collapse(ci);

      // perform collapse
      mesh_.collapse(bestHandle);
      ++n_collapses;

      // store current collapses state
      oldCollapses = n_collapses;
      noCollapses = 0;
      collapsesUnchanged = false;


      // update triangle normals
      typename Mesh::VertexFaceIter vf_it = mesh_.vf_iter(ci.v1);
      for (; vf_it; ++vf_it)
        if (!mesh_.status(vf_it).deleted())
          mesh_.set_normal(vf_it, mesh_.calc_face_normal(vf_it.handle()));

      // post-process collapse
      this->postprocess_collapse(ci);

    } else {
      if (oldCollapses == n_collapses) {
        if (collapsesUnchanged == false) {
          noCollapses = 1;
          collapsesUnchanged = true;
        } else {
          noCollapses++;
        }
      }
    }

  }

  if (_factor < 1.0)
    this->set_error_tolerance_factor(1.0);

  // DON'T do garbage collection here! It's up to the application.
  return n_collapses;

}

//=============================================================================
}// END_NS_MC_DECIMATER
} // END_NS_OPENMESH
//=============================================================================


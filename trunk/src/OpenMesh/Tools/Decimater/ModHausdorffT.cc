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
 *   $Revision: 448 $                                                        *
 *   $Date: 2011-11-04 13:59:37 +0100 (Fri, 04 Nov 2011) $                   *
 *                                                                           *
\*===========================================================================*/

/** \file ModHausdorffT.cc
 */


//=============================================================================
//
//  CLASS ModHausdorffT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_DECIMATER_MODHAUSDORFFT_C


//== INCLUDES =================================================================

#include "ModHausdorffT.hh"
#ifdef USE_OPENMP
#include <omp.h>
#endif


//== NAMESPACES ===============================================================

namespace OpenMesh  {
namespace Decimater {

//== IMPLEMENTATION ==========================================================

template <class MeshT>
typename ModHausdorffT<MeshT>::Scalar
ModHausdorffT<MeshT>::
distPointTriangleSquared( const Point& _p,
                          const Point& _v0,
                          const Point& _v1,
                          const Point& _v2,
                          Point& _nearestPoint )
{
  const Point v0v1 = _v1 - _v0;
  const Point v0v2 = _v2 - _v0;
  const Point n = v0v1 % v0v2; // not normalized !
  const double d = n.sqrnorm();


  // Check if the triangle is degenerated
  if (d < FLT_MIN && d > -FLT_MIN) {
//    std::cerr << "distPointTriangleSquared: Degenerated triangle !\n";
//    std::cerr << "Points are : " << _v0 << " " << _v1 << " " << _v2 << std::endl;
//    std::cerr << "d is " << d << std::endl;
    return -1.0;
  }
  const double invD = 1.0 / d;


  // these are not needed for every point, should still perform
  // better with many points against one triangle
  const Point v1v2 = _v2 - _v1;
  const double inv_v0v2_2 = 1.0 / v0v2.sqrnorm();
  const double inv_v0v1_2 = 1.0 / v0v1.sqrnorm();
  const double inv_v1v2_2 = 1.0 / v1v2.sqrnorm();


  Point v0p = _p - _v0;
  Point t = v0p % n;
  double  s01, s02, s12;
  const double a = (t | v0v2) * -invD;
  const double b = (t | v0v1) * invD;


  if (a < 0)
  {
    // Calculate the distance to an edge or a corner vertex
    s02 = ( v0v2 | v0p ) * inv_v0v2_2;
    if (s02 < 0.0)
    {
      s01 = ( v0v1 | v0p ) * inv_v0v1_2;
      if (s01 <= 0.0) {
        v0p = _v0;
      } else if (s01 >= 1.0) {
        v0p = _v1;
      } else {
        v0p = _v0 + v0v1 * s01;
      }
    } else if (s02 > 1.0) {
      s12 = ( v1v2 | ( _p - _v1 )) * inv_v1v2_2;
      if (s12 >= 1.0) {
        v0p = _v2;
      } else if (s12 <= 0.0) {
        v0p = _v1;
      } else {
        v0p = _v1 + v1v2 * s12;
      }
    } else {
      v0p = _v0 + v0v2 * s02;
    }
  } else if (b < 0.0) {
    // Calculate the distance to an edge or a corner vertex
    s01 = ( v0v1 | v0p ) * inv_v0v1_2;
    if (s01 < 0.0)
    {
  const Point n = v0v1 % v0v2; // not normalized !
      s02 = ( v0v2 |  v0p ) * inv_v0v2_2;
      if (s02 <= 0.0) {
        v0p = _v0;
      } else if (s02 >= 1.0) {
        v0p = _v2;
      } else {
        v0p = _v0 + v0v2 * s02;
      }
    } else if (s01 > 1.0) {
      s12 = ( v1v2 | ( _p - _v1 )) * inv_v1v2_2;
      if (s12 >= 1.0) {
        v0p = _v2;
      } else if (s12 <= 0.0) {
        v0p = _v1;
      } else {
        v0p = _v1 + v1v2 * s12;
      }
    } else {
      v0p = _v0 + v0v1 * s01;
    }
  } else if (a+b > 1.0) {
    // Calculate the distance to an edge or a corner vertex
    s12 = ( v1v2 | ( _p - _v1 )) * inv_v1v2_2;
    if (s12 >= 1.0) {
      s02 = ( v0v2 | v0p ) * inv_v0v2_2;
      if (s02 <= 0.0) {
        v0p = _v0;
      } else if (s02 >= 1.0) {
        v0p = _v2;
      } else {
        v0p = _v0 + v0v2*s02;
      }
    } else if (s12 <= 0.0) {
      s01 = ( v0v1 |  v0p ) * inv_v0v1_2;
      if (s01 <= 0.0) {
        v0p = _v0;
      } else if (s01 >= 1.0) {
        v0p = _v1;
      } else {
        v0p = _v0 + v0v1 * s01;
      }
    } else {
      v0p = _v1 + v1v2 * s12;
    }
  } else {
    // Calculate the distance to an interior point of the triangle
    _nearestPoint = _p - n*((n|v0p) * invD);
    return (_nearestPoint - _p).sqrnorm();
  }

  _nearestPoint = v0p;

  return (_nearestPoint - _p).sqrnorm();
}


template <class MeshT>
void
ModHausdorffT<MeshT>::
initialize()
{
  typename Mesh::FIter  f_it(mesh_.faces_begin()), f_end(mesh_.faces_end());

  for (; f_it!=f_end; ++f_it)
    mesh_.property(points_, f_it).clear();
}


//-----------------------------------------------------------------------------


template <class MeshT>
float
ModHausdorffT<MeshT>::
collapse_priority(const CollapseInfo& _ci)
{
  static Points                  points;  points.clear();
  std::vector<FaceHandle>        faces;   faces.reserve(20);
  typename Mesh::VertexFaceIter  vf_it;
  typename Mesh::FaceHandle      fh;
  typename Mesh::Scalar          sqr_tolerace = tolerance_*tolerance_;
  typename Mesh::Point           dummy;
  typename Mesh::CFVIter         fv_it;
  bool                           ok;

  // collect all points to be tested
  // collect all faces to be tested against
  for (vf_it=mesh_.vf_iter(_ci.v0); vf_it; ++vf_it) {
    fh = vf_it.handle();

    if (fh != _ci.fl && fh != _ci.fr)
      faces.push_back(fh);

    Points& pts = mesh_.property(points_, fh);
    std::copy(pts.begin(), pts.end(), std::back_inserter(points));
  }

  // add point to be removed
  points.push_back(_ci.p0);

  // setup iterators
  typename std::vector<FaceHandle>::iterator  fh_it, fh_end(faces.end());
  typename Points::const_iterator             p_it, p_end(points.end());


  // simulate collapse
  mesh_.set_point(_ci.v0, _ci.p1);

  // for each point: try to find a face such that error is < tolerance
  ok = true;

  for (p_it=points.begin(); ok && p_it!=p_end; ++p_it) {
    ok = false;

    for (fh_it=faces.begin(); !ok && fh_it!=fh_end; ++fh_it) {
      const Point& p0 = mesh_.point(fv_it=mesh_.cfv_iter(*fh_it));
      const Point& p1 = mesh_.point(++fv_it);
      const Point& p2 = mesh_.point(++fv_it);

      if (  distPointTriangleSquared(*p_it, p0, p1, p2, dummy) <= sqr_tolerace)
        ok = true;
    }
  }

  // undo simulation changes
  mesh_.set_point(_ci.v0, _ci.p0);

  return ( ok ? Base::LEGAL_COLLAPSE : Base::ILLEGAL_COLLAPSE );
}


//-----------------------------------------------------------------------------


template <class MeshT>
void
ModHausdorffT<MeshT>::
postprocess_collapse(const CollapseInfo& _ci)
{
  static Points                  points;
  typename Mesh::VertexFaceIter  vf_it;
  FaceHandle                     fh;
  std::vector<FaceHandle>        faces;


  // collect points & neighboring triangles

  points.clear(); // it's static !
  faces.reserve(20);


  // collect active faces and their points
  for (vf_it=mesh_.vf_iter(_ci.v1); vf_it; ++vf_it) {
    fh = vf_it.handle();
    faces.push_back(fh);

    Points& pts = mesh_.property(points_, fh);
    std::copy(pts.begin(), pts.end(), std::back_inserter(points));
    pts.clear();
  }
  if (faces.empty()) return; // should not happen anyway...


  // collect points of the 2 deleted faces
  if ((fh=_ci.fl).is_valid()) {
    Points& pts = mesh_.property(points_, fh);
    std::copy(pts.begin(), pts.end(), std::back_inserter(points));
    pts.clear();
  }
  if ((fh=_ci.fr).is_valid()) {
    Points& pts = mesh_.property(points_, fh);
    std::copy(pts.begin(), pts.end(), std::back_inserter(points));
    pts.clear();
  }


  // add the deleted point
  points.push_back(_ci.p0);




  // setup iterators
  typename std::vector<FaceHandle>::iterator  fh_it, fh_end(faces.end());
  typename Points::const_iterator             p_it, p_end(points.end());



  // re-distribute points
  Scalar                  emin, e;
  Point                   dummy;
  typename Mesh::CFVIter  fv_it;

  for (p_it=points.begin(); p_it!=p_end; ++p_it) {
    emin = FLT_MAX;

#ifdef USE_OPENMP
    int facesCount = faces.size();
#pragma omp parallel for private(e) shared(emin)
    for (int i = 0; i < facesCount; ++i) {
      const Point& p0 = mesh_.point(fv_it=mesh_.cfv_iter(faces[i]));
      const Point& p1 = mesh_.point(++fv_it);
      const Point& p2 = mesh_.point(++fv_it);

      e =  distPointTriangleSquared(*p_it, p0, p1, p2, dummy);
      if (e < emin) {
        emin = e;
        fh   = faces[i];
      }

    }
#else
    for (fh_it=faces.begin(); fh_it!=fh_end; ++fh_it) {
      const Point& p0 = mesh_.point(fv_it=mesh_.cfv_iter(*fh_it));
      const Point& p1 = mesh_.point(++fv_it);
      const Point& p2 = mesh_.point(++fv_it);

      e =  distPointTriangleSquared(*p_it, p0, p1, p2, dummy);
      if (e < emin) {
        emin = e;
        fh   = *fh_it;
      }

    }
#endif

    mesh_.property(points_, fh).push_back(*p_it);
  }
}


//-----------------------------------------------------------------------------


template <class MeshT>
typename ModHausdorffT<MeshT>::Scalar
ModHausdorffT<MeshT>::
compute_sqr_error(FaceHandle _fh, const Point& _p) const
{
  typename Mesh::CFVIter           fv_it = mesh_.cfv_iter(_fh);
  const Point&                     p0    = mesh_.point(fv_it);
  const Point&                     p1    = mesh_.point(++fv_it);
  const Point&                     p2    = mesh_.point(++fv_it);

  const Points&                    points = mesh_.property(points_, _fh);
  typename Points::const_iterator  p_it   = points.begin();
  typename Points::const_iterator  p_end  = points.end();

  Point  dummy;
  Scalar e;
  Scalar emax =  distPointTriangleSquared(_p, p0, p1, p2, dummy);


#ifdef USE_OPENMP
  int pointsCount = points.size();
#pragma omp parallel for private(e) shared(emax)
  for (int i = 0; i < pointsCount; ++i) {
    e =  distPointTriangleSquared(points[i], p0, p1, p2, dummy);
    if (e > emax)
      emax = e;
  }
#else
  for (; p_it!=p_end; ++p_it) {
    e =  distPointTriangleSquared(*p_it, p0, p1, p2, dummy);
    if (e > emax)
      emax = e;
  }
#endif

  return emax;
}


//=============================================================================
}
}
//=============================================================================

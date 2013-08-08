#ifndef UNITTESTS_TRIMESH_CIRCULATOR_CURRENT_HANDLE_REPLACEMENT_HH
#define UNITTESTS_TRIMESH_CIRCULATOR_CURRENT_HANDLE_REPLACEMENT_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

#include <iostream>

class OpenMeshTrimeshCirculatorCurrentHalfedgeHandleReplacement : public OpenMeshBase {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {
        }

        // This function is called after all tests are through
        virtual void TearDown() {

            // Do some final stuff with the member data here...
        }


    // Member already defined in OpenMeshBase
    //Mesh mesh_;
};

/*
 * ====================================================================
 * Define tests below
 * ====================================================================
 */


/*
 * duplicate dereferencing behaviour
 */
TEST_F(OpenMeshTrimeshCirculatorCurrentHalfedgeHandleReplacement, dereference) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[5];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(0,-1, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(2,-1, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[1]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  /* Test setup:
      0 ==== 2
      |\  0 /|
      | \  / |
      |2  1 3|
      | /  \ |
      |/  1 \|
      3 ==== 4 */
  // Starting vertex is 1->4

  // output from fh_it.current_halfedge_handle()
  size_t current_halfedge_handles[12] = {
    4,0,2,10,6,8,1,12,7,14,3,11
  };


  size_t i(0);
  for (Mesh::FaceIter f_it = mesh_.faces_begin(); f_it != mesh_.faces_end(); ++f_it) {
    for (Mesh::FaceHalfedgeIter fh_it = mesh_.fh_iter(*f_it); fh_it.is_valid(); ++fh_it, ++i) {
      EXPECT_EQ(current_halfedge_handles[i], fh_it->idx() ) << "halfedge handles don't match";
    }
  }
}

/*
 * duplicate vv_iter behaviour
 */
TEST_F(OpenMeshTrimeshCirculatorCurrentHalfedgeHandleReplacement, vv_iter) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[5];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(0,-1, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(2,-1, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[1]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  /* Test setup:
      0 ==== 2
      |\  0 /|
      | \  / |
      |2  1 3|
      | /  \ |
      |/  1 \|
      3 ==== 4 */
  // Starting vertex is 1->4

  // output from vv_it.current_halfedge_handle()
  size_t current_halfedge_handles[16] = {
    5,
    0,
    12,
    11,
    6,
    1,
    2,
    15,
    3,
    4,
    13,
    7,
    8,
    9,
    10,
    14
  };

  std::vector<Mesh::EdgeHandle> eh0;
  std::vector<Mesh::EdgeHandle> eh1;
  size_t i(0);
  for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it) {
    for (Mesh::VertexVertexIter vv_it = mesh_.vv_iter(*v_it); vv_it.is_valid(); ++vv_it, ++i)
      eh0.push_back(mesh_.edge_handle(Mesh::HalfedgeHandle(current_halfedge_handles[i])));
  }
  for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it) {
    for (Mesh::VertexOHalfedgeIter voh_it = mesh_.voh_iter(*v_it); voh_it.is_valid(); ++voh_it)
      eh1.push_back(mesh_.edge_handle(*voh_it));
  }

  EXPECT_EQ(eh0.size(), eh1.size()) << "size of vectors does not match";
  for (size_t i = 0; i < eh0.size(); ++i)
    EXPECT_EQ(eh0[i], eh1[i]) << "edge handles do not match";

}

/*
 * duplicate fe_iter behaviour
 */
TEST_F(OpenMeshTrimeshCirculatorCurrentHalfedgeHandleReplacement, fe_iter) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[5];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(0,-1, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(2,-1, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[1]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  /* Test setup:
      0 ==== 2
      |\  0 /|
      | \  / |
      |2  1 3|
      | /  \ |
      |/  1 \|
      3 ==== 4 */
  // Starting vertex is 1->4

  // output from fe_it.current_halfedge_handle()
  size_t current_halfedge_handles[12] = {
    4,0,2,10,6,8,1,12,7,14,3,11
  };

  size_t i(0);

  std::vector<Mesh::HalfedgeHandle> heh0;
  std::vector<Mesh::HalfedgeHandle> heh1;

  for (Mesh::FaceIter f_it = mesh_.faces_begin(); f_it != mesh_.faces_end(); ++f_it) {
    for (Mesh::FaceEdgeIter fe_it = mesh_.fe_iter(*f_it); fe_it.is_valid(); ++fe_it, ++i) {
      heh0.push_back(Mesh::HalfedgeHandle(current_halfedge_handles[i]));
    }
  }
  for (Mesh::FaceIter f_it = mesh_.faces_begin(); f_it != mesh_.faces_end(); ++f_it) {
    for (Mesh::FaceHalfedgeIter fh_it = mesh_.fh_iter(*f_it); fh_it.is_valid(); ++fh_it) {
      heh1.push_back(*fh_it);
    }
  }

  EXPECT_EQ(heh0.size(), heh1.size()) << "size of vectors does not match";
  for (size_t i = 0; i < heh0.size(); ++i)
    EXPECT_EQ(heh0[i], heh1[i]) << "halfedge handles do not match";

}

#endif // UNITTESTS_TRIMESH_CIRCULATOR_CURRENT_HANDLE_REPLACEMENT_HH

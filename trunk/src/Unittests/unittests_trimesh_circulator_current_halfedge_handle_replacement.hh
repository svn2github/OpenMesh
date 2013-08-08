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



  for (Mesh::FaceIter f_it = mesh_.faces_begin(); f_it != mesh_.faces_end(); ++f_it) {
    for (Mesh::FaceHalfedgeIter fh_it = mesh_.fh_iter(*f_it); fh_it.is_valid(); ++fh_it) {
      EXPECT_EQ(fh_it.current_halfedge_handle(), *fh_it ) << "halfedge handles don't match";
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


  std::vector<Mesh::EdgeHandle> eh0;
  std::vector<Mesh::EdgeHandle> eh1;
  for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it) {
    for (Mesh::VertexVertexIter vv_it = mesh_.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
      eh0.push_back(mesh_.edge_handle(vv_it.current_halfedge_handle()));
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


  std::vector<Mesh::HalfedgeHandle> heh0;
  std::vector<Mesh::HalfedgeHandle> heh1;

  for (Mesh::FaceIter f_it = mesh_.faces_begin(); f_it != mesh_.faces_end(); ++f_it) {
    for (Mesh::FaceEdgeIter fe_it = mesh_.fe_iter(*f_it); fe_it.is_valid(); ++fe_it) {
      heh0.push_back(fe_it.current_halfedge_handle());
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
/*
 * duplicate find_halfedge behaviour
 */
TEST_F(OpenMeshTrimeshCirculatorCurrentHalfedgeHandleReplacement, find_halfedge) {

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


  Mesh::HalfedgeHandle hh = mesh_.find_halfedge(vhandle[0], vhandle[1]);
  Mesh::HalfedgeHandle hh1;

  for (Mesh::VertexOHalfedgeIter voh_it = mesh_.voh_iter(vhandle[0]); voh_it.is_valid(); ++voh_it) {
    if (mesh_.to_vertex_handle(*voh_it) == vhandle[1]) {
      hh1 = *voh_it;
      break;
    }
  }

  EXPECT_EQ(hh, hh1 ) << "halfedge handles don't match";
}

#endif // UNITTESTS_TRIMESH_CIRCULATOR_CURRENT_HANDLE_REPLACEMENT_HH

#ifndef INCLUDE_UNITTESTS_TRIMESH_OTHERS_HH
#define INCLUDE_UNITTESTS_TRIMESH_OTHERS_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

#include <iostream>

class OpenMeshOthers : public OpenMeshBase {

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
 * Checking for feature edges based on angle
 */
TEST_F(OpenMeshOthers, IsEstimatedFeatureEdge) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(1, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(0, 0, 1));

  // Add four faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[0]);
  mesh_.add_face(face_vhandles);

  // ===============================================
  // Setup complete
  // ===============================================


  // Check one Request only vertex normals
  // Face normals are required for vertex and halfedge normals, so 
  // that prevent access to non existing properties are in place

  mesh_.request_vertex_normals();
  mesh_.request_halfedge_normals();
  mesh_.request_face_normals();

  // Automatically compute all normals
  // As only vertex normals are requested and no face normals, this will compute nothing.
  mesh_.update_normals();

  Mesh::HalfedgeIter he_it  = mesh_.halfedges_begin();

  EXPECT_TRUE(mesh_.is_estimated_feature_edge(he_it,0.0))          << "Wrong feature edge detection 0.0";

  EXPECT_TRUE(mesh_.is_estimated_feature_edge(he_it,0.125 * M_PI)) << "Wrong feature edge detection 0.125";

  EXPECT_TRUE(mesh_.is_estimated_feature_edge(he_it,0.25 * M_PI))  << "Wrong feature edge detection 0.25";

  EXPECT_TRUE(mesh_.is_estimated_feature_edge(he_it,0.375 * M_PI)) << "Wrong feature edge detection 0.375";

  EXPECT_TRUE(mesh_.is_estimated_feature_edge(he_it,0.5 * M_PI))   << "Wrong feature edge detection 0.5";

  EXPECT_FALSE(mesh_.is_estimated_feature_edge(he_it,0.625 * M_PI))<< "Wrong feature edge detection 0.625";

  EXPECT_FALSE(mesh_.is_estimated_feature_edge(he_it,0.75 * M_PI)) << "Wrong feature edge detection 0.75";

  EXPECT_FALSE(mesh_.is_estimated_feature_edge(he_it,0.875 * M_PI))<< "Wrong feature edge detection 0.875";

  EXPECT_FALSE(mesh_.is_estimated_feature_edge(he_it,1.0 * M_PI))  << "Wrong feature edge detection 1.0";

}

#endif // INCLUDE GUARD

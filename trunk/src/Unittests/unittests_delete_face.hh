#ifndef INCLUDE_UNITTESTS_OpenMeshDeleteFaceTriangleMesh_HH
#define INCLUDE_UNITTESTS_OpenMeshDeleteFaceTriangleMesh_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>
#include <iostream>

class OpenMeshDeleteFaceTriangleMesh : public OpenMeshBase {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {
            
            // Do some initial stuff with the member data here...
        }

        // This function is called after all tests are through
        virtual void TearDown() {

            // Do some final stuff with the member data here...
        }

    // Member already defined in OpenMeshBase
    //Mesh mesh_;  
};

class OpenMeshDeleteFacePolyMesh : public OpenMeshBasePoly {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {
            
            // Do some initial stuff with the member data here...
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

/* Adds a cube to a trimesh and then deletes half of the faces
 */
TEST_F(OpenMeshDeleteFaceTriangleMesh, DeleteHalfTriangleMeshCube) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[8];
  vhandle[0] = mesh_.add_vertex(Mesh::Point(-1, -1,  1));
  vhandle[1] = mesh_.add_vertex(Mesh::Point( 1, -1,  1));
  vhandle[2] = mesh_.add_vertex(Mesh::Point( 1,  1,  1));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(-1,  1,  1));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(-1, -1, -1));
  vhandle[5] = mesh_.add_vertex(Mesh::Point( 1, -1, -1));
  vhandle[6] = mesh_.add_vertex(Mesh::Point( 1,  1, -1));
  vhandle[7] = mesh_.add_vertex(Mesh::Point(-1,  1, -1));

  // Add six faces to form a cube
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  //=======================

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);
  face_vhandles.push_back(vhandle[5]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  //=======================

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  mesh_.add_face(face_vhandles);

  //=======================

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[5]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[6]);
  mesh_.add_face(face_vhandles);


  //=======================

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[6]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[6]);
  face_vhandles.push_back(vhandle[7]);
  mesh_.add_face(face_vhandles);

  //=======================

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[7]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);


  // Test setup:
  //
  //
  //    3 ======== 2
  //   /          /|
  //  /          / |      z
  // 0 ======== 1  |      |
  // |          |  |      |   y
  // |  7       |  6      |  /
  // |          | /       | /
  // |          |/        |/
  // 4 ======== 5         -------> x
  //

  // Check setup
  EXPECT_EQ(8u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(12u, mesh_.n_faces() )   << "Wrong number of faces";

  // =====================================================
  // Now we delete half of the mesh
  // =====================================================
  mesh_.request_face_status();
  mesh_.request_vertex_status();
  mesh_.request_halfedge_status();
  const unsigned int n_face_to_delete = mesh_->n_faces()/2;
  for(size_t i = 0; i < n_face_to_delete; i++)

	  mesh_.delete_face(mesh_.face_handle(i));

  // =====================================================
  // Check config afterwards
  // =====================================================

}

/* Adds a cube to a polymesh
 */
TEST_F(OpenMeshDeleteFacePolyMesh, DeleteteHalfPolyMeshCube) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[8];
  vhandle[0] = mesh_.add_vertex(PolyMesh::Point(-1, -1,  1));
  vhandle[1] = mesh_.add_vertex(PolyMesh::Point( 1, -1,  1));
  vhandle[2] = mesh_.add_vertex(PolyMesh::Point( 1,  1,  1));
  vhandle[3] = mesh_.add_vertex(PolyMesh::Point(-1,  1,  1));
  vhandle[4] = mesh_.add_vertex(PolyMesh::Point(-1, -1, -1));
  vhandle[5] = mesh_.add_vertex(PolyMesh::Point( 1, -1, -1));
  vhandle[6] = mesh_.add_vertex(PolyMesh::Point( 1,  1, -1));
  vhandle[7] = mesh_.add_vertex(PolyMesh::Point(-1,  1, -1));

  // Add six faces to form a cube
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);
 
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[6]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[6]);
  face_vhandles.push_back(vhandle[7]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);


  // Test setup:
  //
  //
  //    3 ======== 2
  //   /          /|
  //  /          / |      z
  // 0 ======== 1  |      |
  // |          |  |      |   y
  // |  7       |  6      |  /
  // |          | /       | /
  // |          |/        |/
  // 4 ======== 5         -------> x
  //

  // Check setup
  EXPECT_EQ(8u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(6u, mesh_.n_faces() )    << "Wrong number of faces";

  // =====================================================
  // Now we delete half of the mesh
  // =====================================================
  mesh_.request_face_status();
  mesh_.request_vertex_status();
  mesh_.request_halfedge_status();
  const unsigned int n_face_to_delete = mesh_->n_faces()/2;
  for(size_t i = 0; i < n_face_to_delete; i++)

	  mesh_.delete_face(mesh_.face_handle(i));

  // =====================================================
  // Check config afterwards
  // =====================================================



}


#endif // INCLUDE GUARD

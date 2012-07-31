#ifndef INCLUDE_UNITTESTS_OpenMeshAddFaceTriangleMesh_HH
#define INCLUDE_UNITTESTS_OpenMeshAddFaceTriangleMesh_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>
#include <iostream>

class OpenMeshAddFaceTriangleMesh : public OpenMeshBase {

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

class OpenMeshAddFacePolyMesh : public OpenMeshBasePoly {

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

/* Adds two triangles to a tri mesh
 */
TEST_F(OpenMeshAddFaceTriangleMesh, AddTrianglesToTrimesh) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(1, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(1, 0, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[0]);

  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  // Test setup:
  //  1 === 2
  //  |   / |    
  //  |  /  |
  //  | /   |
  //  0 === 3

  // Check setup
  EXPECT_EQ(4u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(2u, mesh_.n_faces() )    << "Wrong number of faces";

}

/* Adds a quad to a trimesh (should be triangulated afterwards)
 */
TEST_F(OpenMeshAddFaceTriangleMesh, AddQuadToTrimesh) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(1, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(1, 0, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);

  mesh_.add_face(face_vhandles);

  // Test setup:
  //  1 === 2
  //  |   / |    
  //  |  /  |
  //  | /   |
  //  0 === 3

  // Check setup
  EXPECT_EQ(4u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(2u, mesh_.n_faces() )    << "Wrong number of faces";

}

/* Adds a cube to a trimesh
 */
TEST_F(OpenMeshAddFaceTriangleMesh, CreateTriangleMeshCube) {

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

}


/* Adds a quad to a polymesh (should be a quad afterwards)
 */
TEST_F(OpenMeshAddFacePolyMesh, AddQuadToPolymesh) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(1, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(1, 0, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);

  mesh_.add_face(face_vhandles);

  // Test setup:
  //  1 === 2
  //  |     |    
  //  |     |
  //  |     |
  //  0 === 3

  // Check setup
  EXPECT_EQ(4u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(1u, mesh_.n_faces() )    << "Wrong number of faces";

}

/* Adds a cube to a polymesh
 */
TEST_F(OpenMeshAddFacePolyMesh, CreatePolyMeshCube) {

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

}


#endif // INCLUDE GUARD

#ifndef INCLUDE_UNITTESTS_TRIMESH_ITERATORS_HH
#define INCLUDE_UNITTESTS_TRIMESH_ITERATORS_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

#include <iostream>

class OpenMeshIterators : public OpenMeshBase {

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
 * Small VertexIterator Test
 */
TEST_F(OpenMeshIterators, VertexIter) {

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

  Mesh::VertexIter v_it  = mesh_.vertices_begin(); 
  Mesh::VertexIter v_end = mesh_.vertices_end(); 

  EXPECT_EQ(0, v_it.handle().idx()) << "Index wrong for vertex iterator vertices_begin()";
  ++v_it;
  EXPECT_EQ(1, v_it.handle().idx()) << "Index wrong in vertex iterator";
  ++v_it;
  EXPECT_EQ(2, v_it.handle().idx()) << "Index wrong in vertex iterator";
  ++v_it;
  EXPECT_EQ(3, v_it.handle().idx()) << "Index wrong in vertex iterator";
  ++v_it;
  EXPECT_EQ(4, v_it.handle().idx()) << "Index wrong in vertex iterator";

  // Check end iterator
  EXPECT_EQ(4, v_end.handle().idx()) << "Index wrong in vertex iterator for vertices_end()";    


  // Second iterator test to start at a specific position

}

/*
 * Small VertexIterator Test to check if we can start iterating at a specific handle
 */
TEST_F(OpenMeshIterators, VertexIterStartPosition) {

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

  Mesh::VertexIter v_it  = Mesh::VertexIter(mesh_,mesh_.vertex_handle(2));
  Mesh::VertexIter v_end = mesh_.vertices_end();

  EXPECT_EQ(2, v_it.handle().idx()) << "Index wrong in vertex iterator";
  ++v_it;
  EXPECT_EQ(3, v_it.handle().idx()) << "Index wrong in vertex iterator";
  ++v_it;
  EXPECT_EQ(4, v_it.handle().idx()) << "Index wrong in vertex iterator";

  // Check end iterator
  EXPECT_EQ(4, v_end.handle().idx()) << "Index wrong in vertex iterator for vertices_end()";


}

/*
 * Small EdgeIterator Test
 */
TEST_F(OpenMeshIterators, EdgeIter) {

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


  Mesh::EdgeIter e_it  = mesh_.edges_begin(); 
  Mesh::EdgeIter e_end = mesh_.edges_end(); 
  
  EXPECT_EQ(0, e_it.handle().idx()) << "Wrong start index in edge iterator";
  EXPECT_EQ(5, e_end.handle().idx()) << "Wrong end index in edge iterator";

  EXPECT_EQ(1, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() )   << "1: Wrong to vertex handle of halfedge 0";
  EXPECT_EQ(2, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() ) << "1: Wrong from vertex handle of halfedge 0";
  EXPECT_EQ(2, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() )   << "1: Wrong to vertex handle of halfedge 1";
  EXPECT_EQ(1, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() ) << "1: Wrong from vertex handle of halfedge 1";

  ++e_it;
  EXPECT_EQ(1, e_it.handle().idx()) << "Wrong index in edge iterator";

  EXPECT_EQ(0, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() )   << "2: Wrong to vertex handle of halfedge 0";
  EXPECT_EQ(1, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() ) << "2: Wrong from vertex handle of halfedge 0";
  EXPECT_EQ(1, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() )   << "2: Wrong to vertex handle of halfedge 1";
  EXPECT_EQ(0, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() ) << "2: Wrong from vertex handle of halfedge 1";


  ++e_it;
  EXPECT_EQ(2, e_it.handle().idx()) << "Wrong index in edge iterator";

  EXPECT_EQ(2, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() )   << "3: Wrong to vertex handle of halfedge 0";
  EXPECT_EQ(0, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() ) << "3: Wrong from vertex handle of halfedge 0";
  EXPECT_EQ(0, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() )   << "3: Wrong to vertex handle of halfedge 1";
  EXPECT_EQ(2, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() ) << "3: Wrong from vertex handle of halfedge 1";


  ++e_it;
  EXPECT_EQ(3, e_it.handle().idx()) << "Wrong index in edge iterator";

  EXPECT_EQ(3, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() )   << "4: Wrong to vertex handle of halfedge 0";
  EXPECT_EQ(0, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() ) << "4: Wrong from vertex handle of halfedge 0";
  EXPECT_EQ(0, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() )   << "4: Wrong to vertex handle of halfedge 1";
  EXPECT_EQ(3, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() ) << "4: Wrong from vertex handle of halfedge 1";


  ++e_it;
  EXPECT_EQ(4, e_it.handle().idx()) << "Wrong index in edge iterator";

  EXPECT_EQ(2, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() )   << "5: Wrong to vertex handle of halfedge 0";
  EXPECT_EQ(3, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,0)).idx() ) << "5: Wrong from vertex handle of halfedge 0";
  EXPECT_EQ(3, mesh_.to_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() )   << "5: Wrong to vertex handle of halfedge 1";
  EXPECT_EQ(2, mesh_.from_vertex_handle(mesh_.halfedge_handle(e_it,1)).idx() ) << "5: Wrong from vertex handle of halfedge 1";


}

/*
 * Test with a mesh with one deleted face
 */
TEST_F(OpenMeshIterators, FaceIterEmptyMeshOneDeletedFace) {

  mesh_.clear();

  // request delete_face capability
  mesh_.request_vertex_status();
  mesh_.request_edge_status();
  mesh_.request_face_status();

  // Add some vertices
  Mesh::VertexHandle vhandle[4];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(1, 1, 0));

  // Add two faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[0]);
  Mesh::FaceHandle fh = mesh_.add_face(face_vhandles);

  // Delete face but keep vertices
  bool const is_delete_isolated_vertex = false;
  mesh_.delete_face(fh, is_delete_isolated_vertex);

  // Test setup (Face deleted but vertices kept.
  //  1 === 2
  //  |   /
  //  |  /
  //  | /
  //  0

  Mesh::FaceIter f_it  = mesh_.faces_begin();
  Mesh::FaceIter f_end = mesh_.faces_end();

  EXPECT_EQ(0, f_it.handle().idx()) << "Wrong start index in FaceIterator";

  EXPECT_EQ(1, f_end.handle().idx()) << "Wrong end index in FaceIterator";

  ++f_it;
  EXPECT_EQ(1, f_it.handle().idx()) << "Wrong end index in FaceIterator after one step";
  EXPECT_TRUE(f_it == f_end ) << "Iterator not at end for FaceIterator after one step";

  Mesh::ConstFaceIter cf_it  = mesh_.faces_begin();
  Mesh::ConstFaceIter cf_end = mesh_.faces_end();

  EXPECT_EQ(0, cf_it.handle().idx()) << "Wrong start index in ConstFaceIterator";

  EXPECT_EQ(1, cf_end.handle().idx()) << "Wrong end index in ConstFaceIterator";

  ++cf_it;
  EXPECT_EQ(1, cf_it.handle().idx()) << "Wrong end index in ConstFaceIterator after one step";
  EXPECT_TRUE(cf_it == cf_end ) << "Iterator not at end for ConstFaceIterator after one step";


  // Same with skipping iterators:
  f_it  = mesh_.faces_sbegin();
  f_end = mesh_.faces_end();

  EXPECT_EQ(1, f_it.handle().idx()) << "Wrong start index in FaceIterator with skipping";

  EXPECT_EQ(1, f_end.handle().idx()) << "Wrong end index in FaceIterator with skipping";

  EXPECT_TRUE(f_it == f_end ) << "Iterator not at end for FaceIterator with skipping";

  // Same with skipping iterators:
  cf_it  = mesh_.faces_sbegin();
  cf_end = mesh_.faces_end();

  EXPECT_EQ(1, cf_it.handle().idx()) << "Wrong start index in ConstFaceIterator with skipping";

  EXPECT_EQ(1, cf_end.handle().idx()) << "Wrong end index in ConstFaceIterator with skipping";

  EXPECT_TRUE(cf_it == cf_end ) << "Iterator not at end for ConstFaceIterator with skipping";


  mesh_.release_vertex_status();
  mesh_.release_edge_status();
  mesh_.release_face_status();

}

#endif // INCLUDE GUARD

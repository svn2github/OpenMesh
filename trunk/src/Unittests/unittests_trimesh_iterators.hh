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

  Mesh::VertexIter v_it=mesh_.vertices_begin(); 
  Mesh::VertexIter  v_end=mesh_.vertices_end(); 


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
 * Small VertexFaceIterator Test with holes in it
 */
TEST_F(OpenMeshIterators, VertexFaceIterWithHoles) {

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

  /* Test setup:
      0 ==== 2
       \    /
        \  /
          1
        /  \
       /    \
      3 ==== 4 */

  // Iterate around vertex 1 at the middle (with holes in between)
  Mesh::VertexFaceIter vf_it  = mesh_.vf_begin(vhandle[1]); 
  Mesh::VertexFaceIter vf_end = mesh_.vf_end(vhandle[1]);
  EXPECT_EQ(0, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at initialization";
  EXPECT_TRUE(vf_it) << "Iterator invalid in VertexFaceIter at initialization";
  ++vf_it ;
  EXPECT_EQ(1, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at step 1";
  EXPECT_TRUE(vf_it) << "Iterator invalid in VertexFaceIter at step 1";
  ++vf_it ;
  EXPECT_EQ(-1, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at end";
  EXPECT_FALSE(vf_it) << "Iterator not invalid in VertexFaceIter at end";
  EXPECT_TRUE( vf_it == vf_end )  << "End iterator for VertexFaceIter not matching";

  // Iterate around vertex 1 at the middle (with holes in between) with const iterator
  Mesh::ConstVertexFaceIter cvf_it  = mesh_.cvf_begin(vhandle[1]);
  Mesh::ConstVertexFaceIter cvf_end = mesh_.cvf_end(vhandle[1]);
  EXPECT_EQ(0, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at initialization";
  EXPECT_TRUE(cvf_it) << "Iterator invalid in ConstVertexFaceIter at initialization";
  ++cvf_it ;
  EXPECT_EQ(1, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at step one";
  EXPECT_TRUE(cvf_it) << "Iterator invalid in ConstVertexFaceIter at step one";
  ++cvf_it ;
  EXPECT_EQ(-1, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at end";
  EXPECT_FALSE(cvf_it) << "Iterator not invalid in ConstVertexFaceIter at end";
  EXPECT_TRUE( cvf_it == cvf_end )  << "End iterator for ConstVertexFaceIter not matching";

}

/*
 * Small VertexFaceIterator Test without holes in it
 */
TEST_F(OpenMeshIterators, VertexFaceIterWithoutHoles) {

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

  Mesh::VertexFaceIter vfa_it  = mesh_.vf_begin(vhandle[1]);

  // Iterate around vertex 1 at the middle (with holes in between)
  Mesh::VertexFaceIter vf_it  = mesh_.vf_begin(vhandle[1]);
  Mesh::VertexFaceIter vf_end = mesh_.vf_end(vhandle[1]);
  EXPECT_EQ(3, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at initialization";
  EXPECT_TRUE(vf_it) << "Iterator invalid in VertexFaceIter at initialization";
  ++vf_it ;
  EXPECT_EQ(1, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at step 1";
  EXPECT_TRUE(vf_it) << "Iterator invalid in VertexFaceIter at step 1";
  ++vf_it ;
  EXPECT_EQ(2, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at step 2";
  EXPECT_TRUE(vf_it) << "Iterator invalid in VertexFaceIter at step 2";
  ++vf_it ;
  EXPECT_EQ(0, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at step 3";
  EXPECT_TRUE(vf_it) << "Iterator invalid in VertexFaceIter at step 3";
  ++vf_it ;
  EXPECT_EQ(3, vf_it.handle().idx() ) << "Index wrong in VertexFaceIter at end";
  EXPECT_FALSE(vf_it) << "Iterator not invalid in VertexFaceIter at end";
  EXPECT_TRUE( vf_it == vf_end )  << "End iterator for VertexFaceIter not matching";

  // Iterate around vertex 1 at the middle (with holes in between) with const iterator
  Mesh::ConstVertexFaceIter cvf_it  = mesh_.cvf_begin(vhandle[1]);
  Mesh::ConstVertexFaceIter cvf_end = mesh_.cvf_end(vhandle[1]);
  EXPECT_EQ(3, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at initialization";
  EXPECT_TRUE(cvf_it) << "Iterator invalid in ConstVertexFaceIter at initialization";
  ++cvf_it ;
  EXPECT_EQ(1, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at step 1";
  EXPECT_TRUE(cvf_it) << "Iterator invalid in ConstVertexFaceIter at step 1";
  ++cvf_it ;
  EXPECT_EQ(2, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at step 2";
  EXPECT_TRUE(cvf_it) << "Iterator invalid in ConstVertexFaceIter at step 2";
  ++cvf_it ;
  EXPECT_EQ(0, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at step 3";
  EXPECT_TRUE(cvf_it) << "Iterator invalid in ConstVertexFaceIter at step 3";
  ++cvf_it ;
  EXPECT_EQ(3, cvf_it.handle().idx() ) << "Index wrong in ConstVertexFaceIter at end";
  EXPECT_FALSE(cvf_it) << "Iterator not invalid in VertexFaceIter at end";
  EXPECT_TRUE( cvf_it == cvf_end )  << "End iterator for ConstVertexFaceIter not matching";


}

/*
 * Small FaceFaceIterator Test with holes in it
 */
TEST_F(OpenMeshIterators, FaceFaceIterWithHoles) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[5];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(3, 0, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(4, 1, 0));

  // Add three faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  /* Test setup:
   *
   * 0 ------ 2 ------ 4
   *  \      / \      /
   *   \  0 /   \  2 /
   *    \  /  1  \  /
   *     1 ------- 3
   */


  Mesh::FaceFaceIter ff_it  = mesh_.ff_begin(mesh_.face_handle(1));
  Mesh::FaceFaceIter ff_end = mesh_.ff_end(mesh_.face_handle(1));

  EXPECT_EQ(2, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at initialization";
  EXPECT_TRUE(ff_it) << "Iterator invalid in FaceFaceIter at initialization";
  ++ff_it;
  EXPECT_EQ(0, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at step 1";
  EXPECT_TRUE(ff_it) << "Iterator invalid in FaceFaceIter at step 1";
  ++ff_it;
  EXPECT_EQ(2, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at end";
  EXPECT_FALSE(ff_it) << "Iterator invalid in FaceFaceIter at end";
  EXPECT_TRUE( ff_it == ff_end )  << "End iterator for FaceFaceIter not matching";

  Mesh::ConstFaceFaceIter cff_it  = mesh_.cff_begin(mesh_.face_handle(1));
  Mesh::ConstFaceFaceIter cff_end = mesh_.cff_end(mesh_.face_handle(1));

  EXPECT_EQ(2, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at initialization";
  EXPECT_TRUE(cff_it) << "Iterator invalid in ConstFaceFaceIter at initialization";
  ++cff_it;
  EXPECT_EQ(0, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at step 1";
  EXPECT_TRUE(cff_it) << "Iterator invalid in ConstFaceFaceIter at step 1";
  ++cff_it;
  EXPECT_EQ(2, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at end";
  EXPECT_FALSE(cff_it) << "Iterator invalid in ConstFaceFaceIter at end";
  EXPECT_TRUE( cff_it == cff_end )  << "End iterator for ConstFaceFaceIter not matching";

}

/*
 * Small FaceFaceIterator Test with holes in it
 */
TEST_F(OpenMeshIterators, FaceFaceIterWithoutHoles) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[6];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(3, 0, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(4, 1, 0));
  vhandle[5] = mesh_.add_vertex(Mesh::Point(2,-1, 0));

  // Add three faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  mesh_.add_face(face_vhandles);

  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[3]);
  mesh_.add_face(face_vhandles);

  /* Test setup:
   *
   * 0 ------ 2 ------ 4
   *  \      / \      /
   *   \  0 /   \  2 /
   *    \  /  1  \  /
   *     1 ------- 3
   *      \       /
   *       \  3  /
   *        \   /
   *         \ /
   *          5
   */


  Mesh::FaceFaceIter ff_it  = mesh_.ff_begin(mesh_.face_handle(1));
  Mesh::FaceFaceIter ff_end = mesh_.ff_end(mesh_.face_handle(1));

  EXPECT_EQ(2, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at initialization";
  EXPECT_TRUE(ff_it) << "Iterator invalid in FaceFaceIter at initialization";
  ++ff_it;
  EXPECT_EQ(0, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at step 1";
  EXPECT_TRUE(ff_it) << "Iterator invalid in FaceFaceIter at step 1";
  ++ff_it;
  EXPECT_EQ(3, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at step 2";
  EXPECT_TRUE(ff_it) << "Iterator invalid in FaceFaceIter at step 2";
  ++ff_it;
  EXPECT_EQ(2, ff_it.handle().idx() ) << "Index wrong in FaceFaceIter at end";
  EXPECT_FALSE(ff_it) << "Iterator invalid in FaceFaceIter at end";
  EXPECT_TRUE( ff_it == ff_end )  << "End iterator for FaceFaceIter not matching";

  Mesh::ConstFaceFaceIter cff_it  = mesh_.cff_begin(mesh_.face_handle(1));
  Mesh::ConstFaceFaceIter cff_end = mesh_.cff_end(mesh_.face_handle(1));

  EXPECT_EQ(2, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at initialization";
  EXPECT_TRUE(cff_it) << "Iterator invalid in ConstFaceFaceIter at initialization";
  ++cff_it;
  EXPECT_EQ(0, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at step 1";
  EXPECT_TRUE(cff_it) << "Iterator invalid in ConstFaceFaceIter at step 1";
  ++cff_it;
  EXPECT_EQ(3, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at step 2";
  EXPECT_TRUE(cff_it) << "Iterator invalid in ConstFaceFaceIter at step 2";
  ++cff_it;
  EXPECT_EQ(2, cff_it.handle().idx() ) << "Index wrong in ConstFaceFaceIter at end";
  EXPECT_FALSE(cff_it) << "Iterator invalid in ConstFaceFaceIter at end";
  EXPECT_TRUE( cff_it == cff_end )  << "End iterator for ConstFaceFaceIter not matching";

}





#endif // INCLUDE GUARD

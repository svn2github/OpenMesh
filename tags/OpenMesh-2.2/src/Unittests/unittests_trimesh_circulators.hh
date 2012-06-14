#ifndef INCLUDE_UNITTESTS_TRIMESH_CIRCULATORS_HH
#define INCLUDE_UNITTESTS_TRIMESH_CIRCULATORS_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

#include <iostream>

class OpenMeshCirculators : public OpenMeshBase {

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
 * Small VertexFaceIterator Test with holes in it
 *
 * WARNING!!! Basically this is an illegal configuration!
 * But this way we can still detect if it breaks!
 */
TEST_F(OpenMeshCirculators, VertexFaceIterWithHoles) {

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
TEST_F(OpenMeshCirculators, VertexFaceIterWithoutHoles) {

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
 * Small VertexFaceIterator Test without holes in it
 */
TEST_F(OpenMeshCirculators, VertexOutgoingHalfedgeWithoutHoles) {

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


  // Iterate around vertex 1 at the middle (with holes in between)
  Mesh::VertexOHalfedgeIter voh_it  = mesh_.voh_begin(vhandle[1]);
  Mesh::VertexOHalfedgeIter voh_end = mesh_.voh_end(vhandle[1]);

  EXPECT_EQ(11, voh_it.handle().idx() ) << "Index wrong in VertexOHalfedgeIter begin at initialization";
  EXPECT_EQ(11, voh_end.handle().idx() ) << "Index wrong in VertexOHalfedgeIter end at initialization";
  EXPECT_EQ(3, mesh_.face_handle(voh_it.handle()).idx() ) << "Corresponding face Index wrong in VertexOHalfedgeIter begin at initialization";
  EXPECT_TRUE(voh_it) << "Iterator invalid in VertexOHalfedgeIter at initialization";

  ++voh_it ;

  EXPECT_EQ(6, voh_it.handle().idx() ) << "Index wrong in VertexOHalfedgeIter step 1";
  EXPECT_EQ(1, mesh_.face_handle(voh_it.handle()).idx() ) << "Corresponding face Index wrong in VertexOHalfedgeIter step 1";
  EXPECT_TRUE(voh_it) << "Iterator invalid in VertexOHalfedgeIter at step 1";

  ++voh_it ;

  EXPECT_EQ(1, voh_it.handle().idx() ) << "Index wrong in VertexOHalfedgeIter step 2";
  EXPECT_EQ(2, mesh_.face_handle(voh_it.handle()).idx() ) << "Corresponding face Index wrong in VertexOHalfedgeIter step 2";
  EXPECT_TRUE(voh_it) << "Iterator invalid in VertexOHalfedgeIter at step 2";

  ++voh_it ;

  EXPECT_EQ(2, voh_it.handle().idx() ) << "Index wrong in VertexOHalfedgeIter step 3";
  EXPECT_EQ(0, mesh_.face_handle(voh_it.handle()).idx() ) << "Corresponding face Index wrong in VertexOHalfedgeIter step 3";
  EXPECT_TRUE(voh_it) << "Iterator invalid in VertexOHalfedgeIter at step 3";

  ++voh_it ;

  EXPECT_EQ(11, voh_it.handle().idx() ) << "Index wrong in VertexOHalfedgeIter step 4";
  EXPECT_EQ(3, mesh_.face_handle(voh_it.handle()).idx() ) << "Corresponding face Index wrong in VertexOHalfedgeIter step 4";
  EXPECT_FALSE(voh_it) << "Iterator still valid in VertexOHalfedgeIter at step 4";
  EXPECT_TRUE( voh_it == voh_end ) << "Miss matched end iterator";

  ++voh_it ;

  EXPECT_EQ(6, voh_it.handle().idx() ) << "Index wrong in VertexOHalfedgeIter step 5";
  EXPECT_EQ(1, mesh_.face_handle(voh_it.handle()).idx() ) << "Corresponding face Index wrong in VertexOHalfedgeIter step 5";
  //EXPECT_FALSE(voh_it) << "Iterator still valid in VertexOHalfedgeIter at step 5";



  // Iterate around vertex 1 at the middle (with holes in between)
  Mesh::ConstVertexOHalfedgeIter cvoh_it  = mesh_.cvoh_begin(vhandle[1]);
  Mesh::ConstVertexOHalfedgeIter cvoh_end = mesh_.cvoh_end(vhandle[1]);

  EXPECT_EQ(11, cvoh_it.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter begin at initialization";
  EXPECT_EQ(11, cvoh_end.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter end at initialization";
  EXPECT_EQ(3, mesh_.face_handle(cvoh_it.handle()).idx() ) << "Corresponding face Index wrong in ConstVertexOHalfedgeIter begin at initialization";
  EXPECT_TRUE(cvoh_it) << "Iterator invalid in ConstVertexOHalfedgeIter at initialization";

  ++cvoh_it ;

  EXPECT_EQ(6, cvoh_it.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter step 1";
  EXPECT_EQ(1, mesh_.face_handle(cvoh_it.handle()).idx() ) << "Corresponding face Index wrong in ConstVertexOHalfedgeIter step 1";
  EXPECT_TRUE(cvoh_it) << "Iterator invalid in ConstVertexOHalfedgeIter at step 1";

  ++cvoh_it ;

  EXPECT_EQ(1, cvoh_it.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter step 2";
  EXPECT_EQ(2, mesh_.face_handle(cvoh_it.handle()).idx() ) << "Corresponding face Index wrong in ConstVertexOHalfedgeIter step 2";
  EXPECT_TRUE(cvoh_it) << "Iterator invalid in ConstVertexOHalfedgeIter at step 2";

  ++cvoh_it ;

  EXPECT_EQ(2, cvoh_it.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter step 3";
  EXPECT_EQ(0, mesh_.face_handle(cvoh_it.handle()).idx() ) << "Corresponding face Index wrong in ConstVertexOHalfedgeIter step 3";
  EXPECT_TRUE(cvoh_it) << "Iterator invalid in ConstVertexOHalfedgeIter at step 3";

  ++cvoh_it ;

  EXPECT_EQ(11, cvoh_it.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter step 4";
  EXPECT_EQ(3, mesh_.face_handle(cvoh_it.handle()).idx() ) << "Corresponding face Index wrong in ConstVertexOHalfedgeIter step 4";
  EXPECT_FALSE(cvoh_it) << "Iterator still valid in ConstVertexOHalfedgeIter at step 4";
  EXPECT_TRUE( cvoh_it == cvoh_end ) << "Miss matched end iterator";

  ++cvoh_it ;

  EXPECT_EQ(6, cvoh_it.handle().idx() ) << "Index wrong in ConstVertexOHalfedgeIter step 5";
  EXPECT_EQ(1, mesh_.face_handle(cvoh_it.handle()).idx() ) << "Corresponding face Index wrong in ConstVertexOHalfedgeIter step 5";
  //EXPECT_FALSE(cvoh_it) << "Iterator still valid in ConstVertexOHalfedgeIter at step 5";


}

/*
 * Small FaceFaceIterator Test with holes in it
 */
TEST_F(OpenMeshCirculators, FaceFaceIterWithHoles) {

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
TEST_F(OpenMeshCirculators, FaceFaceIterWithoutHoles) {

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

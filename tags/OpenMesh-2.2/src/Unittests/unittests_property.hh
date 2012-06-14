#ifndef INCLUDE_UNITTESTS_DECIMATER_HH
#define INCLUDE_UNITTESTS_DECIMATER_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>
#include <iostream>
class OpenMeshProperties : public OpenMeshBase {

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

/* Creates a double property and checks if it works
 */
TEST_F(OpenMeshProperties, VertexPropertyCheckDouble) {

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
  EXPECT_EQ(4, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(2, mesh_.n_faces() )    << "Wrong number of faces";

  // Add a double vertex property
  OpenMesh::VPropHandleT<double> doubleHandle;

  EXPECT_FALSE( mesh_.get_property_handle(doubleHandle,"doubleProp") );

  mesh_.add_property(doubleHandle,"doubleProp");

  EXPECT_TRUE(mesh_.get_property_handle(doubleHandle,"doubleProp"));


  // Fill property
  double index = 0.0; 

  for ( Mesh::VertexIter v_it = mesh_.vertices_begin() ; v_it != mesh_.vertices_end(); ++v_it ) {
    mesh_.property(doubleHandle,v_it) = index;
    index += 1.0;
  }

  // Check if it is ok.
  Mesh::VertexIter v_it = mesh_.vertices_begin();
  EXPECT_EQ( mesh_.property(doubleHandle,v_it) , 0.0 ) << "Invalid double value for vertex 0";
  ++v_it;

  EXPECT_EQ( mesh_.property(doubleHandle,v_it) , 1.0 ) << "Invalid double value for vertex 1";
  ++v_it;

  EXPECT_EQ( mesh_.property(doubleHandle,v_it) , 2.0 ) << "Invalid double value for vertex 2";
  ++v_it;

  EXPECT_EQ( mesh_.property(doubleHandle,v_it) , 3.0 ) << "Invalid double value for vertex 3";

  // Try to get the stl iterators:
  std::vector<double>::iterator it=mesh_.property(doubleHandle).data_vector().begin();
  std::vector<double>::iterator end=mesh_.property(doubleHandle).data_vector().end();

  EXPECT_EQ( *it , 0.0 ) << "Invalid double value for vertex 0";
  ++it;

  EXPECT_EQ( *it , 1.0 ) << "Invalid double value for vertex 1";
  ++it;

  EXPECT_EQ( *it , 2.0 ) << "Invalid double value for vertex 2";
  ++it;

  EXPECT_EQ( *it , 3.0 ) << "Invalid double value for vertex 3";
  ++it;

  EXPECT_EQ( it, end ) << "End iterator not mathing!";

}

/* Creates a bool property and checks if it works
 */
TEST_F(OpenMeshProperties, VertexPropertyCheckBool) {

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
  EXPECT_EQ(4, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(2, mesh_.n_faces() )    << "Wrong number of faces";

  // Add a double vertex property
  OpenMesh::VPropHandleT<bool> boolHandle;

  EXPECT_FALSE( mesh_.get_property_handle(boolHandle,"boolProp") );

  mesh_.add_property(boolHandle,"boolProp");

  EXPECT_TRUE(mesh_.get_property_handle(boolHandle,"boolProp"));

  // Fill property
  bool current = true;

  for ( Mesh::VertexIter v_it = mesh_.vertices_begin() ; v_it != mesh_.vertices_end(); ++v_it ) {
    mesh_.property(boolHandle,v_it) = current;
    current = !current;
  }

  // Check if it is ok.
  Mesh::VertexIter v_it = mesh_.vertices_begin();
  EXPECT_TRUE( mesh_.property(boolHandle,v_it) ) << "Invalid bool value for vertex 0";
  ++v_it;

  EXPECT_FALSE( mesh_.property(boolHandle,v_it) ) << "Invalid bool value for vertex 1";
  ++v_it;

  EXPECT_TRUE( mesh_.property(boolHandle,v_it) ) << "Invalid bool value for vertex 2";
  ++v_it;

  EXPECT_FALSE( mesh_.property(boolHandle,v_it) ) << "Invalid bool value for vertex 3";

  // Try to get the stl iterators:
  std::vector<bool>::iterator it=mesh_.property(boolHandle).data_vector().begin();
  std::vector<bool>::iterator end=mesh_.property(boolHandle).data_vector().end();

  EXPECT_TRUE( *it ) << "Invalid bool value for vertex 0";
  ++it;

  EXPECT_FALSE( *it ) << "Invalid bool value for vertex 1";
  ++it;
  
  EXPECT_TRUE( *it ) << "Invalid bool value for vertex 2";
  ++it;
  
  EXPECT_FALSE( *it ) << "Invalid bool value for vertex 3";
  ++it;

  EXPECT_EQ( it, end ) << "End iterator not mathing!";

}

#endif // INCLUDE GUARD

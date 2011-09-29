#ifndef INCLUDE_UNITTESTS_TRIMESH_ITERATORS_HH
#define INCLUDE_UNITTESTS_TRIMESH_ITERATORS_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>

#include <iostream>

class OpenMeshIterators : public OpenMeshBase {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {
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

    unsigned int i = 0;


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

    unsigned int i = 0;


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


#endif // INCLUDE GUARD

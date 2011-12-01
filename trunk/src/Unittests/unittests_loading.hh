#ifndef INCLUDE_UNITTESTS_LOADING_HH
#define INCLUDE_UNITTESTS_LOADING_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>


class OpenMeshLoader : public OpenMeshBase {

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

/*
 * Just load a simple mesh file in obj format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshLoader, LoadSimpleOFFFile) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube1.off");

    EXPECT_TRUE(ok);

    EXPECT_EQ(7526, mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}


/*
 * Just load a point file in ply format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshLoader, LoadSimplePointPLYFileWithBadEncoding) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "pointCloudBadEncoding.ply");

    EXPECT_TRUE(ok) << "Unable to load pointCloudBadEncoding.ply";

    EXPECT_EQ(10, mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(0, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(0, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}

/*
 * Just load a point file in ply format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshLoader, LoadSimplePointPLYFileWithGoodEncoding) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "pointCloudGoodEncoding.ply");

    EXPECT_TRUE(ok) << "Unable to load pointCloudGoodEncoding.ply";

    EXPECT_EQ(10, mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(0, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(0, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}


/*
 * Just load a obj file of a cube
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJ) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal.obj");

    EXPECT_TRUE(ok) << "Unable to load cube-minimal.obj";

    EXPECT_EQ(8, mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}


/*
 * Just load a obj file of a cube with vertex colors defined directly after the vertex definitions
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJWithVertexColorsAfterVertices) {

    mesh_.clear();

    mesh_.request_vertex_colors();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-vertex-colors-after-vertex-definition.obj");

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-vertex-colors-after-vertex-definition.obj";

    EXPECT_EQ(8,   mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18,  mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12,  mesh_.n_faces()) << "The number of loaded faces is not correct!";
   
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[0] ) << "Wrong vertex color at vertex 0 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[1] ) << "Wrong vertex color at vertex 0 component 1";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[2] ) << "Wrong vertex color at vertex 0 component 2";

    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(3))[0] ) << "Wrong vertex color at vertex 3 component 0";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(3))[1] ) << "Wrong vertex color at vertex 3 component 1";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(3))[2] ) << "Wrong vertex color at vertex 3 component 2";

    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(4))[0] ) << "Wrong vertex color at vertex 4 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(4))[1] ) << "Wrong vertex color at vertex 4 component 1";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(4))[2] ) << "Wrong vertex color at vertex 4 component 2";

    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[0] ) << "Wrong vertex color at vertex 7 component 0";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[1] ) << "Wrong vertex color at vertex 7 component 1";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[2] ) << "Wrong vertex color at vertex 7 component 2";

    mesh_.release_vertex_colors();
}

/*
 * Just load a obj file of a cube with vertex colors defined as separate lines
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJWithVertexColorsAsVCLines) {

    mesh_.clear();

    mesh_.request_vertex_colors();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-vertex-colors-as-vc-lines.obj");

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-vertex-colors-as-vc-lines.obj";

    EXPECT_EQ(8,   mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18,  mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12,  mesh_.n_faces()) << "The number of loaded faces is not correct!";

    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[0] ) << "Wrong vertex color at vertex 0 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[1] ) << "Wrong vertex color at vertex 0 component 1";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[2] ) << "Wrong vertex color at vertex 0 component 2";

    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(3))[0] ) << "Wrong vertex color at vertex 3 component 0";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(3))[1] ) << "Wrong vertex color at vertex 3 component 1";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(3))[2] ) << "Wrong vertex color at vertex 3 component 2";

    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(4))[0] ) << "Wrong vertex color at vertex 4 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(4))[1] ) << "Wrong vertex color at vertex 4 component 1";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(4))[2] ) << "Wrong vertex color at vertex 4 component 2";

    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[0] ) << "Wrong vertex color at vertex 7 component 0";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[1] ) << "Wrong vertex color at vertex 7 component 1";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[2] ) << "Wrong vertex color at vertex 7 component 2";

    mesh_.release_vertex_colors();

}






#endif // INCLUDE GUARD

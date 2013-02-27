#ifndef INCLUDE_UNITTESTS_READ_WRITE_OFF_HH
#define INCLUDE_UNITTESTS_READ_WRITE_OFF_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>


class OpenMeshReadWriteOFF : public OpenMeshBase {

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
TEST_F(OpenMeshReadWriteOFF, LoadSimpleOFFFile) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube1.off");

    EXPECT_TRUE(ok);

    EXPECT_EQ(7526u , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572u, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048u, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}


TEST_F(OpenMeshReadWriteOFF, WriteAndReadVertexColorsToAndFromOFFFile) {

    mesh_.clear();

    mesh_.request_vertex_colors();

    mesh_.add_vertex( Mesh::Point(0,0,1) );
    mesh_.add_vertex( Mesh::Point(0,1,0) );
    mesh_.add_vertex( Mesh::Point(0,1,1) );
    mesh_.add_vertex( Mesh::Point(1,0,1) );

    // using the default color type Vec3uc from DefaultTraits in Traits.hh
    Mesh::Color testColor(255, 128, 64);

    // setting colors (different from black)
    for (Mesh::VertexIter vit = mesh_.vertices_begin(), vitend = mesh_.vertices_end(); vit != vitend; ++vit)
      mesh_.set_color(vit, testColor);

    // check if the colors are correctly setted
    int count = 0;
    for (Mesh::VertexIter vit = mesh_.vertices_begin(), vitend = mesh_.vertices_end(); vit != vitend; ++vit) {
      Mesh::Color color = mesh_.color(vit);
      if ( color[0] != testColor[0] || color[1] != testColor[1] || color[2] != testColor[2] )
        ++ count;
    }

    EXPECT_EQ(0, count) << "Vertices have the wrong color!";

    // write the mesh_
    OpenMesh::IO::Options opt(OpenMesh::IO::Options::VertexColor);
    OpenMesh::IO::write_mesh(mesh_, "temp.off", opt);
    OpenMesh::IO::read_mesh(mesh_, "temp.off", opt);

    // check if vertices still have the same color
    count = 0;
    for (Mesh::VertexIter vit = mesh_.vertices_begin(), vitend = mesh_.vertices_end(); vit != vitend; ++vit) {
      Mesh::Color color = mesh_.color(vit);
      if ( color[0] != testColor[0] || color[1] != testColor[1] || color[2] != testColor[2] )
        ++ count;
    }

    EXPECT_EQ(0, count) << "Vertices should have the same color after writing and reading the OFF file!";

    mesh_.release_vertex_colors();
}


#endif // INCLUDE GUARD

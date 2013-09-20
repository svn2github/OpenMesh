
#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>


namespace {

class OpenMeshReadWriteSTL : public OpenMeshBase {

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
 * Just load a simple mesh file in stla format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshReadWriteSTL, LoadSimpleSTLFile) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube1.stl");

    EXPECT_TRUE(ok);

    EXPECT_EQ(7526u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048u , mesh_.n_faces()) << "The number of loaded faces is not correct!";
}



/*
 * Just load a simple mesh file in stlb format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshReadWriteSTL, LoadSimpleSTLBinaryFile) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube1Binary.stl");

    EXPECT_TRUE(ok);

    EXPECT_EQ(7526u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048u , mesh_.n_faces()) << "The number of loaded faces is not correct!";
}

}

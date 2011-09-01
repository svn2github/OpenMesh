#ifndef INCLUDE_UNITTESTS_LOADING_CC
#define INCLUDE_UNITTESTS_LOADING_CC

#include <gtest/gtest.h>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

struct CustomTraits : public OpenMesh::DefaultTraits {
};

typedef OpenMesh::TriMesh_ArrayKernelT<CustomTraits> Mesh;

/*
 * Simple test setting.
 */

class OpenMeshLoader : public testing::Test {

    protected:

        // This function is called before each test is run
        virtual void SetUp() {
            
            // Do some initial stuff with the member data here...
        }

        // This function is called after all tests are through
        virtual void TearDown() {

            // Do some final stuff with the member data here...
        }

    // This member will be accessible in all tests
    Mesh mesh_;  
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

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube1.off");

    EXPECT_TRUE(ok);

    EXPECT_EQ(7526, mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}

#endif // INCLUDE GUARD

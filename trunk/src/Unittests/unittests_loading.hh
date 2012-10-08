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

    EXPECT_EQ(7526u , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572u, mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048u, mesh_.n_faces()) << "The number of loaded faces is not correct!";
}



/*
 * Just load a simple mesh file in stla format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshLoader, LoadSimpleSTLFile) {

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
TEST_F(OpenMeshLoader, LoadSimpleSTLBinaryFile) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube1Binary.stl");

    EXPECT_TRUE(ok);

    EXPECT_EQ(7526u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(22572u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(15048u , mesh_.n_faces()) << "The number of loaded faces is not correct!";
}



/*
 * Just load a point file in ply format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshLoader, LoadSimplePointPLYFileWithBadEncoding) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "pointCloudBadEncoding.ply");

    EXPECT_TRUE(ok) << "Unable to load pointCloudBadEncoding.ply";

    EXPECT_EQ(10u , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(0u  , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(0u  , mesh_.n_faces()) << "The number of loaded faces is not correct!";
}

/*
 * Just load a point file in ply format and count whether
 * the right number of entities has been loaded.
 */
TEST_F(OpenMeshLoader, LoadSimplePointPLYFileWithGoodEncoding) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "pointCloudGoodEncoding.ply");

    EXPECT_TRUE(ok) << "Unable to load pointCloudGoodEncoding.ply";

    EXPECT_EQ(10u , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(0u  , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(0u  , mesh_.n_faces()) << "The number of loaded faces is not correct!";
}


/*
 * Just load a obj file of a cube
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJ) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal.obj");

    EXPECT_TRUE(ok) << "Unable to load cube-minimal.obj";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces()) << "The number of loaded faces is not correct!";
}

/*
 * Just load a obj file of a cube and checks the halfedge and vertex normals
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJCheckHalfEdgeAndVertexNormals) {

  mesh_.clear();

  mesh_.request_halfedge_normals();
  mesh_.request_vertex_normals();

  OpenMesh::IO::Options options;
  options += OpenMesh::IO::Options::VertexNormal;

  std::string file_name = "cube-minimal.obj";

  bool ok = OpenMesh::IO::read_mesh(mesh_, file_name,options);

  EXPECT_TRUE(ok) << file_name;

  EXPECT_EQ(8u  ,   mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
  EXPECT_EQ(18u ,  mesh_.n_edges()) << "The number of loaded edges is not correct!";
  EXPECT_EQ(12u ,  mesh_.n_faces()) << "The number of loaded faces is not correct!";
  EXPECT_EQ(36u , mesh_.n_halfedges()) << "The number of loaded halfedges is not correct!";

  ///////////////////////////////////////////////
  //check vertex normals
  EXPECT_EQ(0,   mesh_.normal(mesh_.vertex_handle(0))[0] ) << "Wrong vertex normal at vertex 0 component 0";
  EXPECT_EQ(-1,   mesh_.normal(mesh_.vertex_handle(0))[1] ) << "Wrong vertex normal at vertex 0 component 1";
  EXPECT_EQ(0,   mesh_.normal(mesh_.vertex_handle(0))[2] ) << "Wrong vertex normal at vertex 0 component 2";

  EXPECT_EQ(0,   mesh_.normal(mesh_.vertex_handle(3))[0] ) << "Wrong vertex normal at vertex 3 component 0";
  EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(3))[1] ) << "Wrong vertex normal at vertex 3 component 1";
  EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(3))[2] ) << "Wrong vertex normal at vertex 3 component 2";

  EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(4))[0] ) << "Wrong vertex normal at vertex 4 component 0";
  EXPECT_EQ(-1,   mesh_.normal(mesh_.vertex_handle(4))[1] ) << "Wrong vertex normal at vertex 4 component 1";
  EXPECT_EQ(0,   mesh_.normal(mesh_.vertex_handle(4))[2] ) << "Wrong vertex normal at vertex 4 component 2";

  EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(7))[0] ) << "Wrong vertex normal at vertex 7 component 0";
  EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(7))[1] ) << "Wrong vertex normal at vertex 7 component 1";
  EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(7))[2] ) << "Wrong vertex normal at vertex 7 component 2";

  ///////////////////////////////////////////////
  //check halfedge normals
  EXPECT_EQ(0,   mesh_.normal(mesh_.halfedge_handle(0))[0] ) << "Wrong halfedge normal at halfedge 0 component 0";
  EXPECT_EQ(0,   mesh_.normal(mesh_.halfedge_handle(0))[1] ) << "Wrong halfedge normal at halfedge 0 component 1";
  EXPECT_EQ(-1,   mesh_.normal(mesh_.halfedge_handle(0))[2] ) << "Wrong halfedge normal at halfedge 0 component 2";

  EXPECT_EQ(-1,   mesh_.normal(mesh_.halfedge_handle(10))[0] ) << "Wrong halfedge normal at halfedge 10 component 0";
  EXPECT_EQ(0, mesh_.normal(mesh_.halfedge_handle(10))[1] ) << "Wrong halfedge normal at halfedge 10 component 1";
  EXPECT_EQ(0, mesh_.normal(mesh_.halfedge_handle(10))[2] ) << "Wrong halfedge normal at halfedge 10 component 2";

  EXPECT_EQ(0, mesh_.normal(mesh_.halfedge_handle(19))[0] ) << "Wrong halfedge normal at halfedge 19 component 0";
  EXPECT_EQ(1,   mesh_.normal(mesh_.halfedge_handle(19))[1] ) << "Wrong halfedge normal at halfedge 19 component 1";
  EXPECT_EQ(0,   mesh_.normal(mesh_.halfedge_handle(19))[2] ) << "Wrong halfedge normal at halfedge 19 component 2";

  EXPECT_EQ(1, mesh_.normal(mesh_.halfedge_handle(24))[0] ) << "Wrong halfedge normal at halfedge 24 component 0";
  EXPECT_EQ(0,   mesh_.normal(mesh_.halfedge_handle(24))[1] ) << "Wrong halfedge normal at halfedge 24 component 1";
  EXPECT_EQ(0,   mesh_.normal(mesh_.halfedge_handle(24))[2] ) << "Wrong halfedge normal at halfedge 24 component 2";

  EXPECT_EQ(0, mesh_.normal(mesh_.halfedge_handle(30))[0] ) << "Wrong halfedge normal at halfedge 30 component 0";
  EXPECT_EQ(-1,   mesh_.normal(mesh_.halfedge_handle(30))[1] ) << "Wrong halfedge normal at halfedge 30 component 1";
  EXPECT_EQ(0,   mesh_.normal(mesh_.halfedge_handle(30))[2] ) << "Wrong halfedge normal at halfedge 30 component 2";

  EXPECT_EQ(0, mesh_.normal(mesh_.halfedge_handle(35))[0] ) << "Wrong halfedge normal at halfedge 35 component 0";
  EXPECT_EQ(0, mesh_.normal(mesh_.halfedge_handle(35))[1] ) << "Wrong halfedge normal at halfedge 35 component 1";
  EXPECT_EQ(1, mesh_.normal(mesh_.halfedge_handle(35))[2] ) << "Wrong halfedge normal at halfedge 35 component 2";

  mesh_.release_vertex_normals();
  mesh_.release_halfedge_normals();

}

/*
 * Just load a obj file and set vertex color option before loading
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJForceVertexColorsAlthoughNotAvailable) {

  mesh_.clear();

  mesh_.request_vertex_colors();

  std::string file_name = "cube-minimal.obj";

  OpenMesh::IO::Options options;
  options += OpenMesh::IO::Options::VertexColor;

  bool ok = OpenMesh::IO::read_mesh(mesh_, file_name,options);

  EXPECT_TRUE(ok) << file_name;

  EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
  EXPECT_EQ(18u , mesh_.n_edges())     << "The number of loaded edges is not correct!";
  EXPECT_EQ(12u , mesh_.n_faces())     << "The number of loaded faces is not correct!";
  EXPECT_EQ(36u , mesh_.n_halfedges())  << "The number of loaded halfedges is not correct!";

}


/*
 * Just load a obj file of a cube and checks the halfedge texCoords
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJCheckTexCoords) {

  mesh_.clear();

  mesh_.request_halfedge_texcoords2D();

  OpenMesh::IO::Options options;
  options += OpenMesh::IO::Options::FaceTexCoord;

  std::string file_name = "cube-minimal-texCoords.obj";

  bool ok = OpenMesh::IO::read_mesh(mesh_, file_name,options);

  EXPECT_TRUE(ok) << file_name;

  EXPECT_EQ(1, mesh_.texcoord2D(mesh_.halfedge_handle(0))[0] ) << "Wrong texCoord at halfedge 0 component 0";
  EXPECT_EQ(1, mesh_.texcoord2D(mesh_.halfedge_handle(0))[1] ) << "Wrong texCoord at halfedge 0 component 1";

  EXPECT_EQ(3, mesh_.texcoord2D(mesh_.halfedge_handle(10))[0] ) << "Wrong texCoord at halfedge 1 component 0";
  EXPECT_EQ(3, mesh_.texcoord2D(mesh_.halfedge_handle(10))[1] ) << "Wrong texCoord at halfedge 1 component 1";

  EXPECT_EQ(6, mesh_.texcoord2D(mesh_.halfedge_handle(19))[0] ) << "Wrong texCoord at halfedge 4 component 0";
  EXPECT_EQ(6, mesh_.texcoord2D(mesh_.halfedge_handle(19))[1] ) << "Wrong texCoord at halfedge 4 component 1";

  EXPECT_EQ(7, mesh_.texcoord2D(mesh_.halfedge_handle(24))[0] ) << "Wrong texCoord at halfedge 7 component 0";
  EXPECT_EQ(7, mesh_.texcoord2D(mesh_.halfedge_handle(24))[1] ) << "Wrong texCoord at halfedge 7 component 1";

  EXPECT_EQ(9, mesh_.texcoord2D(mesh_.halfedge_handle(30))[0] ) << "Wrong texCoord at halfedge 9 component 0";
  EXPECT_EQ(9, mesh_.texcoord2D(mesh_.halfedge_handle(30))[1] ) << "Wrong texCoord at halfedge 9 component 1";

  EXPECT_EQ(12, mesh_.texcoord2D(mesh_.halfedge_handle(35))[0] ) << "Wrong texCoord at halfedge 11 component 0";
  EXPECT_EQ(12, mesh_.texcoord2D(mesh_.halfedge_handle(35))[1] ) << "Wrong texCoord at halfedge 11 component 1";

  mesh_.release_halfedge_texcoords2D();
}


/*
 * Just load a obj file of a cube with vertex colors defined directly after the vertex definitions
 */
TEST_F(OpenMeshLoader, LoadSimpleOBJWithVertexColorsAfterVertices) {

    mesh_.clear();

    mesh_.request_vertex_colors();

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexColor;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-vertex-colors-after-vertex-definition.obj",options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-vertex-colors-after-vertex-definition.obj";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges())    << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces())    << "The number of loaded faces is not correct!";

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

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexColor;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-vertex-colors-as-vc-lines.obj",options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-vertex-colors-as-vc-lines.obj";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces()) << "The number of loaded faces is not correct!";

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
 * Just load a ply
 */
TEST_F(OpenMeshLoader, LoadSimplePLY) {

    mesh_.clear();

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal.ply");

    EXPECT_TRUE(ok) << "Unable to load cube-minimal.ply";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces()) << "The number of loaded faces is not correct!";

}

/*
 * Just load a ply file and set vertex color option before loading
 */
TEST_F(OpenMeshLoader, LoadSimplePLYForceVertexColorsAlthoughNotAvailable) {

  mesh_.clear();

  mesh_.request_vertex_colors();

  std::string file_name = "cube-minimal.ply";

  OpenMesh::IO::Options options;
  options += OpenMesh::IO::Options::VertexColor;

  bool ok = OpenMesh::IO::read_mesh(mesh_, file_name,options);

  EXPECT_TRUE(ok) << file_name;

  EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
  EXPECT_EQ(18u , mesh_.n_edges())     << "The number of loaded edges is not correct!";
  EXPECT_EQ(12u , mesh_.n_faces())     << "The number of loaded faces is not correct!";
  EXPECT_EQ(36u , mesh_.n_halfedges())  << "The number of loaded halfedges is not correct!";

  EXPECT_FALSE(options.vertex_has_normal()) << "Wrong user options are returned!";
  EXPECT_FALSE(options.vertex_has_texcoord()) << "Wrong user options are returned!";
  EXPECT_FALSE(options.vertex_has_color()) << "Wrong user options are returned!";
}

/*
 * Just load a ply file of a cube with vertex colors
 */
TEST_F(OpenMeshLoader, LoadSimplePLYWithVertexColors) {

    mesh_.clear();

    mesh_.request_vertex_colors();

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexColor;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-vertexColors.ply",options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-vertexColors.ply";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges())    << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces())    << "The number of loaded faces is not correct!";

    EXPECT_EQ(255,   mesh_.color(mesh_.vertex_handle(0))[0] ) << "Wrong vertex color at vertex 0 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[1] ) << "Wrong vertex color at vertex 0 component 1";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[2] ) << "Wrong vertex color at vertex 0 component 2";

    EXPECT_EQ(255,   mesh_.color(mesh_.vertex_handle(3))[0] ) << "Wrong vertex color at vertex 3 component 0";
    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(3))[1] ) << "Wrong vertex color at vertex 3 component 1";
    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(3))[2] ) << "Wrong vertex color at vertex 3 component 2";

    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(4))[0] ) << "Wrong vertex color at vertex 4 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(4))[1] ) << "Wrong vertex color at vertex 4 component 1";
    EXPECT_EQ(255,   mesh_.color(mesh_.vertex_handle(4))[2] ) << "Wrong vertex color at vertex 4 component 2";

    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(7))[0] ) << "Wrong vertex color at vertex 7 component 0";
    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(7))[1] ) << "Wrong vertex color at vertex 7 component 1";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[2] ) << "Wrong vertex color at vertex 7 component 2";

    EXPECT_FALSE(options.vertex_has_normal()) << "Wrong user options are returned!";
    EXPECT_FALSE(options.vertex_has_texcoord()) << "Wrong user options are returned!";
    EXPECT_TRUE(options.vertex_has_color()) << "Wrong user options are returned!";

    mesh_.release_vertex_colors();
}

/*
 * Just load a ply file of a cube with vertex texCoords
 */
TEST_F(OpenMeshLoader, LoadSimplePLYWithTexCoords) {

    mesh_.clear();

    mesh_.request_vertex_texcoords2D();

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexTexCoord;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-texCoords.ply",options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-texCoords.ply";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges())    << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces())    << "The number of loaded faces is not correct!";

    EXPECT_EQ(10,   mesh_.texcoord2D(mesh_.vertex_handle(0))[0] ) << "Wrong vertex color at vertex 0 component 0";
    EXPECT_EQ(10,   mesh_.texcoord2D(mesh_.vertex_handle(0))[1] ) << "Wrong vertex color at vertex 0 component 1";

    EXPECT_EQ(6,   mesh_.texcoord2D(mesh_.vertex_handle(2))[0] ) << "Wrong vertex color at vertex 2 component 0";
    EXPECT_EQ(6,   mesh_.texcoord2D(mesh_.vertex_handle(2))[1] ) << "Wrong vertex color at vertex 2 component 1";

    EXPECT_EQ(9,   mesh_.texcoord2D(mesh_.vertex_handle(4))[0] ) << "Wrong vertex color at vertex 4 component 0";
    EXPECT_EQ(9,   mesh_.texcoord2D(mesh_.vertex_handle(4))[1] ) << "Wrong vertex color at vertex 4 component 1";

    EXPECT_EQ(12,   mesh_.texcoord2D(mesh_.vertex_handle(7))[0] ) << "Wrong vertex color at vertex 7 component 0";
    EXPECT_EQ(12,   mesh_.texcoord2D(mesh_.vertex_handle(7))[1] ) << "Wrong vertex color at vertex 7 component 1";


    EXPECT_FALSE(options.vertex_has_normal()) << "Wrong user options are returned!";
    EXPECT_TRUE(options.vertex_has_texcoord()) << "Wrong user options are returned!";
    EXPECT_FALSE(options.vertex_has_color()) << "Wrong user options are returned!";

    mesh_.release_vertex_texcoords2D();
}

/*
 * Just load a ply with normals, ascii mode
 */
TEST_F(OpenMeshLoader, LoadSimplePLYWithNormals) {

    mesh_.clear();

    mesh_.request_vertex_normals();

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexNormal;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-normals.ply", options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-normals.ply";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges()) << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces()) << "The number of loaded faces is not correct!";

    EXPECT_TRUE(options.vertex_has_normal()) << "Wrong user options are returned!";
    EXPECT_FALSE(options.vertex_has_texcoord()) << "Wrong user options are returned!";
    EXPECT_FALSE(options.vertex_has_color()) << "Wrong user options are returned!";


    EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(0))[0] ) << "Wrong normal at vertex 0 component 0";
    EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(0))[1] ) << "Wrong normal at vertex 0 component 1";
    EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(0))[2] ) << "Wrong normal at vertex 0 component 2";

    EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(3))[0] ) << "Wrong normal at vertex 3 component 0";
    EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(3))[1] ) << "Wrong normal at vertex 3 component 1";
    EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(3))[2] ) << "Wrong normal at vertex 3 component 2";

    EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(4))[0] ) << "Wrong normal at vertex 4 component 0";
    EXPECT_EQ(0, mesh_.normal(mesh_.vertex_handle(4))[1] ) << "Wrong normal at vertex 4 component 1";
    EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(4))[2] ) << "Wrong normal at vertex 4 component 2";

    EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(7))[0] ) << "Wrong normal at vertex 7 component 0";
    EXPECT_EQ(1, mesh_.normal(mesh_.vertex_handle(7))[1] ) << "Wrong normal at vertex 7 component 1";
    EXPECT_EQ(2, mesh_.normal(mesh_.vertex_handle(7))[2] ) << "Wrong normal at vertex 7 component 2";

    mesh_.release_vertex_normals();

}


/*
 * Just load an om file of a cube with vertex texCoords
 */
TEST_F(OpenMeshLoader, LoadSimpleOMWithTexCoords) {

    mesh_.clear();

    mesh_.request_vertex_texcoords2D();

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexTexCoord;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-texCoords.om",options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-texCoords.om";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges())    << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces())    << "The number of loaded faces is not correct!";

    EXPECT_EQ(10,   mesh_.texcoord2D(mesh_.vertex_handle(0))[0] ) << "Wrong vertex color at vertex 0 component 0";
    EXPECT_EQ(10,   mesh_.texcoord2D(mesh_.vertex_handle(0))[1] ) << "Wrong vertex color at vertex 0 component 1";

    EXPECT_EQ(6,   mesh_.texcoord2D(mesh_.vertex_handle(2))[0] ) << "Wrong vertex color at vertex 2 component 0";
    EXPECT_EQ(6,   mesh_.texcoord2D(mesh_.vertex_handle(2))[1] ) << "Wrong vertex color at vertex 2 component 1";

    EXPECT_EQ(9,   mesh_.texcoord2D(mesh_.vertex_handle(4))[0] ) << "Wrong vertex color at vertex 4 component 0";
    EXPECT_EQ(9,   mesh_.texcoord2D(mesh_.vertex_handle(4))[1] ) << "Wrong vertex color at vertex 4 component 1";

    EXPECT_EQ(12,   mesh_.texcoord2D(mesh_.vertex_handle(7))[0] ) << "Wrong vertex color at vertex 7 component 0";
    EXPECT_EQ(12,   mesh_.texcoord2D(mesh_.vertex_handle(7))[1] ) << "Wrong vertex color at vertex 7 component 1";


    EXPECT_FALSE(options.vertex_has_normal()) << "Wrong user options are returned!";
    EXPECT_TRUE(options.vertex_has_texcoord()) << "Wrong user options are returned!";
    EXPECT_FALSE(options.vertex_has_color()) << "Wrong user options are returned!";

    mesh_.release_vertex_texcoords2D();
}

/*
 * Just load an om file of a cube with vertex colors
 */
TEST_F(OpenMeshLoader, LoadSimpleOMWithVertexColors) {

    mesh_.clear();

    mesh_.request_vertex_colors();

    OpenMesh::IO::Options options;
    options += OpenMesh::IO::Options::VertexColor;

    bool ok = OpenMesh::IO::read_mesh(mesh_, "cube-minimal-vertexColors.om",options);

    EXPECT_TRUE(ok) << "Unable to load cube-minimal-vertexColors.om";

    EXPECT_EQ(8u  , mesh_.n_vertices()) << "The number of loaded vertices is not correct!";
    EXPECT_EQ(18u , mesh_.n_edges())    << "The number of loaded edges is not correct!";
    EXPECT_EQ(12u , mesh_.n_faces())    << "The number of loaded faces is not correct!";

    EXPECT_EQ(255,   mesh_.color(mesh_.vertex_handle(0))[0] ) << "Wrong vertex color at vertex 0 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[1] ) << "Wrong vertex color at vertex 0 component 1";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(0))[2] ) << "Wrong vertex color at vertex 0 component 2";

    EXPECT_EQ(255,   mesh_.color(mesh_.vertex_handle(3))[0] ) << "Wrong vertex color at vertex 3 component 0";
    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(3))[1] ) << "Wrong vertex color at vertex 3 component 1";
    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(3))[2] ) << "Wrong vertex color at vertex 3 component 2";

    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(4))[0] ) << "Wrong vertex color at vertex 4 component 0";
    EXPECT_EQ(0,   mesh_.color(mesh_.vertex_handle(4))[1] ) << "Wrong vertex color at vertex 4 component 1";
    EXPECT_EQ(255,   mesh_.color(mesh_.vertex_handle(4))[2] ) << "Wrong vertex color at vertex 4 component 2";

    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(7))[0] ) << "Wrong vertex color at vertex 7 component 0";
    EXPECT_EQ(0, mesh_.color(mesh_.vertex_handle(7))[1] ) << "Wrong vertex color at vertex 7 component 1";
    EXPECT_EQ(255, mesh_.color(mesh_.vertex_handle(7))[2] ) << "Wrong vertex color at vertex 7 component 2";

    EXPECT_FALSE(options.vertex_has_normal()) << "Wrong user options are returned!";
    EXPECT_FALSE(options.vertex_has_texcoord()) << "Wrong user options are returned!";
    EXPECT_TRUE(options.vertex_has_color()) << "Wrong user options are returned!";

    mesh_.release_vertex_colors();
}

#endif // INCLUDE GUARD

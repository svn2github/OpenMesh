#ifndef INCLUDE_UNITTESTS_SUBIVIDER_HH
#define INCLUDE_UNITTESTS_SUBIVIDER_HH

#include <gtest/gtest.h>
#include <Unittests/unittests_common.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/Sqrt3T.hh>
#include <OpenMesh/Tools/Subdivider/Adaptive/Composite/CompositeT.hh>
#include <OpenMesh/Tools/Subdivider/Adaptive/Composite/RulesT.hh>

class OpenMeshSubdivider_Poly : public OpenMeshBasePoly {

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

class OpenMeshSubdivider_Triangle : public OpenMeshBase {

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
 */
TEST_F(OpenMeshSubdivider_Triangle, Subdivider_Loop) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[9];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(0, 2, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(1, 1, 0));
  vhandle[5] = mesh_.add_vertex(Mesh::Point(1, 2, 0));
  vhandle[6] = mesh_.add_vertex(Mesh::Point(2, 0, 0));
  vhandle[7] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[8] = mesh_.add_vertex(Mesh::Point(2, 2, 0));

  // Add eight faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[3]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[4]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[7]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[8]);
  face_vhandles.push_back(vhandle[7]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[8]);

  mesh_.add_face(face_vhandles);

  // Test setup:
  //  6 === 7 === 8
  //  |   / |   / |
  //  |  /  |  /  |
  //  | /   | /   |
  //  3 === 4 === 5
  //  |   / | \   |
  //  |  /  |  \  |
  //  | /   |   \ |
  //  0 === 1 === 2

  // Initialize subdivider
  OpenMesh::Subdivider::Uniform::Sqrt3T<Mesh> sqrt3;

  // Check setup
  EXPECT_EQ(9u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(8u, mesh_.n_faces() )    << "Wrong number of faces";

  // Execute 3 subdivision steps
  sqrt3.attach(mesh_);
  sqrt3( 3 );
  sqrt3.detach();

  // Check setup
  EXPECT_EQ(121u, mesh_.n_vertices() ) << "Wrong number of vertices after subdivision with sqrt3";
  EXPECT_EQ(216u, mesh_.n_faces() )    << "Wrong number of faces after subdivision with sqrt3";

}

/*
 * ====================================================================
 * Define tests below
 * ====================================================================
 */

/*
 */
TEST_F(OpenMeshSubdivider_Poly, Subdivider_CatmullClark) {

  mesh_.clear();

  // Add some vertices
  Mesh::VertexHandle vhandle[9];

  vhandle[0] = mesh_.add_vertex(Mesh::Point(0, 0, 0));
  vhandle[1] = mesh_.add_vertex(Mesh::Point(0, 1, 0));
  vhandle[2] = mesh_.add_vertex(Mesh::Point(0, 2, 0));
  vhandle[3] = mesh_.add_vertex(Mesh::Point(1, 0, 0));
  vhandle[4] = mesh_.add_vertex(Mesh::Point(1, 1, 0));
  vhandle[5] = mesh_.add_vertex(Mesh::Point(1, 2, 0));
  vhandle[6] = mesh_.add_vertex(Mesh::Point(2, 0, 0));
  vhandle[7] = mesh_.add_vertex(Mesh::Point(2, 1, 0));
  vhandle[8] = mesh_.add_vertex(Mesh::Point(2, 2, 0));

  // Add four faces
  std::vector<Mesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[3]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[8]);
  face_vhandles.push_back(vhandle[7]);

  mesh_.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);

  mesh_.add_face(face_vhandles);

  // Test setup:
  //  6 === 7 === 8
  //  |     |     |
  //  |     |     |
  //  |     |     |
  //  3 === 4 === 5
  //  |     |     |
  //  |     |     |
  //  |     |     |
  //  0 === 1 === 2

  // Initialize subdivider
  OpenMesh::Subdivider::Uniform::CatmullClarkT<PolyMesh> catmull;

  // Check setup
  EXPECT_EQ(9u, mesh_.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(4u, mesh_.n_faces() )    << "Wrong number of faces";

  // Execute 3 subdivision steps
  catmull.attach(mesh_);
  catmull( 3 );
  catmull.detach();

  EXPECT_EQ(289u, mesh_.n_vertices() ) << "Wrong number of vertices after subdivision with catmull clark";
  EXPECT_EQ(256u, mesh_.n_faces() )    << "Wrong number of faces after subdivision with catmull clark";

}

struct MeshTraits : public OpenMesh::Subdivider::Adaptive::CompositeTraits {
  typedef OpenMesh::Vec3f Point;
  typedef OpenMesh::Vec3f Normal;

  VertexAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal);
  EdgeAttributes(OpenMesh::Attributes::Status);
  FaceAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal);
};

typedef OpenMesh::TriMesh_ArrayKernelT<MeshTraits> MyMesh;

TEST_F(OpenMeshSubdivider_Triangle, AdaptiveCompositeRefineVertex) {

  MyMesh mesh;
  mesh.request_vertex_status();
  mesh.request_edge_status();
  mesh.request_face_status();
  mesh.request_vertex_normals();
  mesh.request_face_normals();

  // Add some vertices
  MyMesh::VertexHandle vhandle[9];

  vhandle[0] = mesh.add_vertex(MyMesh::Point(0, 0, 0));
  vhandle[1] = mesh.add_vertex(MyMesh::Point(0, 1, 0));
  vhandle[2] = mesh.add_vertex(MyMesh::Point(0, 2, 0));
  vhandle[3] = mesh.add_vertex(MyMesh::Point(1, 0, 0));
  vhandle[4] = mesh.add_vertex(MyMesh::Point(1, 1, 0));
  vhandle[5] = mesh.add_vertex(MyMesh::Point(1, 2, 0));
  vhandle[6] = mesh.add_vertex(MyMesh::Point(2, 0, 0));
  vhandle[7] = mesh.add_vertex(MyMesh::Point(2, 1, 0));
  vhandle[8] = mesh.add_vertex(MyMesh::Point(2, 2, 0));

  // Add eight faces
  std::vector<MyMesh::VertexHandle> face_vhandles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[3]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[4]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[7]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[8]);
  face_vhandles.push_back(vhandle[7]);

  mesh.add_face(face_vhandles);
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[8]);

  MyMesh::FaceHandle fh = mesh.add_face(face_vhandles);

  //// Test setup:
  ////  6 === 7 === 8
  ////  |   / |   / |
  ////  |  /  |  /  |
  ////  | /   | /   |
  ////  3 === 4 === 5
  ////  |   / | \   |
  ////  |  /  |  \  |
  ////  | /   |   \ |
  ////  0 === 1 === 2

  // Initialize subdivider
  OpenMesh::Subdivider::Adaptive::CompositeT<MyMesh> subdivider(mesh);

  subdivider.add<OpenMesh::Subdivider::Adaptive::Tvv3<MyMesh> >();
  subdivider.add<OpenMesh::Subdivider::Adaptive::VF<MyMesh> >();
  subdivider.add<OpenMesh::Subdivider::Adaptive::FF<MyMesh> >();
  subdivider.add<OpenMesh::Subdivider::Adaptive::FVc<MyMesh> >();

  subdivider.initialize();

  // Check setup
  EXPECT_EQ(9u, mesh.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(8u, mesh.n_faces() )    << "Wrong number of faces";

  // execute adaptive composite subdivision
  subdivider.refine(vhandle[4]);


  // Check setup
  EXPECT_EQ(17u, mesh.n_vertices() ) << "Wrong number of vertices after subdivision with sqrt3";
  EXPECT_EQ(24u, mesh.n_faces() )    << "Wrong number of faces after subdivision with sqrt3";

}

TEST_F(OpenMeshSubdivider_Triangle, AdaptiveCompositeRefineFace) {

  MyMesh mesh;
  mesh.request_vertex_status();
  mesh.request_edge_status();
  mesh.request_face_status();
  mesh.request_vertex_normals();
  mesh.request_face_normals();

  // Add some vertices
  MyMesh::VertexHandle vhandle[9];

  vhandle[0] = mesh.add_vertex(MyMesh::Point(0, 0, 0));
  vhandle[1] = mesh.add_vertex(MyMesh::Point(0, 1, 0));
  vhandle[2] = mesh.add_vertex(MyMesh::Point(0, 2, 0));
  vhandle[3] = mesh.add_vertex(MyMesh::Point(1, 0, 0));
  vhandle[4] = mesh.add_vertex(MyMesh::Point(1, 1, 0));
  vhandle[5] = mesh.add_vertex(MyMesh::Point(1, 2, 0));
  vhandle[6] = mesh.add_vertex(MyMesh::Point(2, 0, 0));
  vhandle[7] = mesh.add_vertex(MyMesh::Point(2, 1, 0));
  vhandle[8] = mesh.add_vertex(MyMesh::Point(2, 2, 0));

  // Add eight faces
  std::vector<MyMesh::VertexHandle> face_vhandles;
  std::vector<MyMesh::FaceHandle> face_handles;

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[3]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[4]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[4]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[7]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[8]);
  face_vhandles.push_back(vhandle[7]);

  face_handles.push_back(mesh.add_face(face_vhandles));
  face_vhandles.clear();

  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[8]);

  face_handles.push_back(mesh.add_face(face_vhandles));

  //// Test setup:
  ////  6 === 7 === 8
  ////  |   / |   / |
  ////  |  /  |  /  |
  ////  | /   | /   |
  ////  3 === 4 === 5
  ////  |   / | \   |
  ////  |  /  |  \  |
  ////  | /   |   \ |
  ////  0 === 1 === 2

  // Initialize subdivider
  OpenMesh::Subdivider::Adaptive::CompositeT<MyMesh> subdivider(mesh);

  subdivider.add<OpenMesh::Subdivider::Adaptive::Tvv3<MyMesh> >();
  subdivider.add<OpenMesh::Subdivider::Adaptive::VF<MyMesh> >();
  subdivider.add<OpenMesh::Subdivider::Adaptive::FF<MyMesh> >();
  subdivider.add<OpenMesh::Subdivider::Adaptive::FVc<MyMesh> >();

  subdivider.initialize();

  // Check setup
  EXPECT_EQ(9u, mesh.n_vertices() ) << "Wrong number of vertices";
  EXPECT_EQ(8u, mesh.n_faces() )    << "Wrong number of faces";

  // execute adaptive composite subdivision
  std::vector<MyMesh::FaceHandle>::iterator it, end;
  it = face_handles.begin();
  end = face_handles.end();
  for (; it != end; ++it)
    subdivider.refine(*it);


  // Check setup
  EXPECT_EQ(245u, mesh.n_vertices() ) << "Wrong number of vertices after subdivision with sqrt3";
  EXPECT_EQ(458u, mesh.n_faces() )    << "Wrong number of faces after subdivision with sqrt3";

}

#endif // INCLUDE GUARD

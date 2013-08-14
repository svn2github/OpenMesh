#include <gtest/gtest.h>

#include "unittests_common.hh"
#include "unittests_property.hh"
#include "unittests_read_write_OBJ.hh"
#include "unittests_read_write_OFF.hh"
#include "unittests_read_write_OM.hh"
#include "unittests_read_write_PLY.hh"
#include "unittests_read_write_STL.hh"
#include "unittests_trimesh_iterators.hh"
#include "unittests_trimesh_collapse.hh"
#include "unittests_decimater.hh"
#include "unittests_mc_decimater.hh"
#include "unittests_mesh_dual.hh"
#include "unittests_mixed_decimater.hh"
#include "unittests_subdivider_uniform.hh"
#include "unittests_subdivider_adaptive.hh"
#include "unittests_trimesh_normal_calculations.hh"
#include "unittests_trimesh_others.hh"
#include "unittests_add_face.hh"
#include "unittests_delete_face.hh"
#include "unittests_faceless_mesh.hh"
#include "unittests_trimesh_garbage_collection.hh"
#include "unittests_randomNumberGenerator.hh"
#include "unittests_split_copy.hh"
#include "unittests_vector_type.hh"
#include "unittests_boundary.hh"

// Circulators on triangular meshes
#include "unittests_trimesh_circulator_face_edge.hh"
#include "unittests_trimesh_circulator_face_face.hh"
#include "unittests_trimesh_circulator_face_halfedge.hh"
#include "unittests_trimesh_circulator_face_vertex.hh"
#include "unittests_trimesh_circulator_vertex_edge.hh"
#include "unittests_trimesh_circulator_vertex_face.hh"
#include "unittests_trimesh_circulator_vertex_ihalfedge.hh"
#include "unittests_trimesh_circulator_vertex_ihalfedge.hh"
#include "unittests_trimesh_circulator_vertex_ohalfedge.hh"
#include "unittests_trimesh_circulator_vertex_vertex.hh"
#include "unittests_trimesh_circulator_current_halfedge_handle_replacement.hh"

int main(int _argc, char** _argv) {

    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}

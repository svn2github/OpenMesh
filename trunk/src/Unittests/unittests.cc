#include <gtest/gtest.h>

#include "unittests_common.hh"
#include "unittests_property.hh"
#include "unittests_loading.hh"
#include "unittests_trimesh_iterators.hh"
#include "unittests_trimesh_collapse.hh"
#include "unittests_trimesh_circulators.hh"
#include "unittests_decimater.hh"
#include "unittests_mc_decimater.hh"
#include "unittests_mixed_decimater.hh"
#include "unittests_subdivider.hh"
#include "unittests_trimesh_normal_calculations.hh"
#include "unittests_trimesh_others.hh"
#include "unittests_add_face.hh"
#include "unittests_faceless_mesh.hh"
#include "unittests_trimesh_garbage_collection.hh"
#include "unittests_randomNumberGenerator.hh"
#include "unittests_split_copy.hh"

int main(int _argc, char** _argv) {

    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}

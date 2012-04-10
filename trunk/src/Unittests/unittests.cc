#include <gtest/gtest.h>

#include "unittests_common.hh"
#include "unittests_property.hh"
#include "unittests_loading.hh"
#include "unittests_trimesh_iterators.hh"
#include "unittests_trimesh_collapse.hh"
#include "unittests_trimesh_circulators.hh"
#include "unittests_decimater.hh"
#include "unittests_trimesh_normal_calculations.hh"

int main(int _argc, char** _argv) {

    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}

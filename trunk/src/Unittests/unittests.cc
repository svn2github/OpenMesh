#include <gtest/gtest.h>

#include "unittests_common.hh"
#include "unittests_loading.hh"
#include "unittests_trimesh_iterators.hh"
#include "unittests_trimesh_circulators.hh"
#include "unittests_decimater.hh"

int main(int _argc, char** _argv) {

    testing::InitGoogleTest(&_argc, _argv);
    return RUN_ALL_TESTS();
}

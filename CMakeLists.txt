# Copyright 2017 Cory Sherman
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# The cgs library is header only, and does not require any compilation.
# This CMakeLists.txt builds cgs tests.

cmake_minimum_required(VERSION 3.2.2 FATAL_ERROR)

project(cgs C CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Weffc++ -Wno-language-extension-token -Wno-missing-braces")
endif()

enable_testing()

include_directories(include)

set(CGS_HEADERS
    "include/cgs.hpp"

    "include/cgs/algorithm.hpp"
    "include/cgs/assert.hpp"
    "include/cgs/macro.hpp"
    "include/cgs/math.hpp"
    "include/cgs/meta.hpp"
    "include/cgs/optimize.hpp"
    "include/cgs/unowned_ptr.hpp"

)

set(CGS_TEST_SOURCE

    "test/algorithm.cpp"
    "test/assert_abort.cpp"
    "test/assert.cpp"
    "test/assert_debug.cpp"
    "test/assert_release.cpp"
    "test/assert_throw.cpp"
    "test/assert_undefined.cpp"
    "test/math.cpp"
    "test/meta.cpp"
    "test/unowned_ptr.cpp"

)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # disable warning for assume with side effect. We're intentionally testing assume with side effects.
    set_source_files_properties("test/assert.cpp"           PROPERTIES COMPILE_FLAGS -Wno-assume)
    set_source_files_properties("test/assert_release.cpp"   PROPERTIES COMPILE_FLAGS -Wno-assume)
    set_source_files_properties("test/assert_undefined.cpp" PROPERTIES COMPILE_FLAGS -Wno-assume)
endif()

# main test
add_executable(cgs-test
    ${CGS_HEADERS}
    ${CGS_TEST_SOURCE}
)
add_test(NAME cgs-test COMMAND cgs-test)

# cmake added c++17 support in version 3.8
if(CMAKE_VERSION VERSION_LESS 3.8)
    if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
    endif()
else()
    set_property(TARGET ${PROJECT_NAME}-test PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${PROJECT_NAME}-test PROPERTY CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    # GTest library does not comply with effective C++
    set_target_properties(${PROJECT_NAME}-test PROPERTIES COMPILE_FLAGS -Wno-effc++)
endif()

# Download and compile Googletest library
include(ExternalProject)
set(gtest_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/gtest-install")
ExternalProject_Add(gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${gtest_INSTALL_PREFIX}
)
set(gtest_INCLUDE_DIRS "${gtest_INSTALL_PREFIX}/include")
set(gtest_LIBRARIES
    "${gtest_INSTALL_PREFIX}/lib/libgtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
    "${gtest_INSTALL_PREFIX}/lib/libgtest_main${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
add_dependencies(${PROJECT_NAME}-test gtest)
include_directories(${gtest_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}-test ${gtest_LIBRARIES})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}-test ${CMAKE_THREAD_LIBS_INIT})

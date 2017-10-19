/*
   Copyright 2017 Cory Sherman

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "gtest/gtest.h"

#undef NDEBUG
#include "cgs/assert.hpp"

#include <regex>

TEST(Assert, DebugAbort)
{
    cgs_assert(2 + 2 == 4);
    EXPECT_DEATH(cgs_assert(2 + 2 == 5), R"(Assertion failed \(2 \+ 2 == 5\) at .*/test/assert_debug\.cpp:26)");
}
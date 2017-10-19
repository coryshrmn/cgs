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

#define CGS_ASSERT_THROW
#include "cgs/assert.hpp"

#include <regex>

TEST(Assert, Throw)
{
    try {
        cgs_assert(2 + 2 == 5);
        EXPECT_FALSE(true);
    }
    catch(const std::logic_error& e) {
        std::string reString { R"(Assertion failed \(2 \+ 2 == 5\) at .*/test/assert_throw\.cpp:26)" };
        std::regex re { reString };
        EXPECT_TRUE(std::regex_match(e.what(), re))
            << "  Actual: \"" <<  e.what() << "\"\n"
            << "Expected: \"" << reString << "\"\n";
    }
}

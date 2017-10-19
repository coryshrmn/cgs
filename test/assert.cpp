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

#include "cgs/assert.hpp"

#include <regex>

TEST(Assert, ManualAbort)
{
    cgs_assert_abort(2 + 2 == 4);
    EXPECT_DEATH(cgs_assert_abort(2 + 2 == 5), R"(Assertion failed \(2 \+ 2 == 5\) at .*/test/assert\.cpp:25)");
}

static int saved = 0;
static bool save(int value) {
    saved = value;
    return false;
}

TEST(Assert, ManualIgnore)
{
    cgs_assert_ignore(true);
    EXPECT_EQ(saved, 0);
    cgs_assert_ignore(save(1));
    EXPECT_EQ(saved, 0);
}

TEST(Assert, ManualThrow)
{
    cgs_assert_throw(2 + 2 == 4);
    try {
        cgs_assert_throw(2 + 2 == 5);
        EXPECT_FALSE(true);
    }
    catch(const std::logic_error& e) {
        std::string reString { R"(Assertion failed \(2 \+ 2 == 5\) at .*/test/assert\.cpp:46)" };
        std::regex re { reString };
        EXPECT_TRUE(std::regex_match(e.what(), re))
            << "  Actual: \"" <<  e.what() << "\"\n"
            << "Expected: \"" << reString << "\"\n";
    }
}

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

#define CGS_ASSERT_IGNORE
#include "cgs/assert.hpp"

static int saved = 0;
static bool save(int value) {
    saved = value;
    return false;
}

TEST(Assert, NotEvaluated)
{
    
    EXPECT_EQ(saved, 0);
    cgs_assert(save(1));
    EXPECT_EQ(saved, 0);
}

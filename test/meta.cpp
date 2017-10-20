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

#include "cgs/meta.hpp"
using cgs::is_constexpr;

constexpr int safe_v() { return 0; }
constexpr int safe_p(void*) { return 0; }
constexpr int safe_fi(float, int) { return 0; }

// constexpr if i != 0
constexpr int safe_if_nonzero(int i)
{
    if(i == 0) {
        throw i;
    }
    return i;
}

// constexpr if i == 0
constexpr int safe_if_zero(int i)
{
    if(i != 0) {
        throw i;
    }
    return i;
}

int unsafe_v()
{
    throw 0;
}

int unsafe_i(int)
{
    throw 0;
}

TEST(Meta, IsConstexpr)
{
    EXPECT_TRUE( (is_constexpr<safe_v>()) );
    EXPECT_TRUE( (is_constexpr<safe_p, std::nullptr_t>()) );
    EXPECT_TRUE( (is_constexpr<safe_fi, float, int>()) );

    EXPECT_FALSE( (is_constexpr<unsafe_v>()) );

    // "strange" results: is_constexpr only considers default-initialized parameter values
    EXPECT_TRUE( (is_constexpr<safe_if_zero, int>()) );
    EXPECT_FALSE( (is_constexpr<safe_if_nonzero, int>()) );

    // can also check at compile time
    static_assert(is_constexpr<safe_fi, float, int>());
    static_assert(!is_constexpr<unsafe_v>());
}

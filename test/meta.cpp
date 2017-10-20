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

int unsafe_i(int i)
{
    throw 0;
}

TEST(Meta, IsConstexpr)
{
    EXPECT_TRUE( (is_constexpr<safe_v>()) );
    EXPECT_TRUE( (is_constexpr<safe_p>(nullptr)) );
    EXPECT_TRUE( (is_constexpr<safe_fi>(0.0f, 0)) );

    EXPECT_FALSE( (is_constexpr<unsafe_v>()) );
    EXPECT_FALSE( (is_constexpr<safe_if_nonzero>(0)) );

    // can also check at compile time
    static_assert(is_constexpr<safe_fi>(0.0f, 0));
    static_assert(!is_constexpr<unsafe_v>());
}

TEST(Meta, IsConstexprNonDefault)
{
    // false results for is_constexpr, when the parameter values effect constexpr-ness
    //
    // We pass default constructed arguments,
    // so we are testing if unsafe_inot(0), which IS constexpr,
    // even though unsafe_inot is not generally constexpr
    // see GitHub issue #1
    //
    // I don't think it is possible to test if a function is generally constexpr.
    //
    // It might be possible check if Func is constexpr with provided values.
    // I haven't figured it out yet.

    EXPECT_TRUE( (is_constexpr<safe_if_nonzero>(5)) );
    EXPECT_FALSE( (is_constexpr<safe_if_zero>(5)) );
}

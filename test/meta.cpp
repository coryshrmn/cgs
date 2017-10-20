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

#include <cstdlib> // rand

constexpr bool isFive(int i)
{
    return i == 5;
}

bool isFiveOrRand(int i)
{
    return i == 5 || (i == std::rand() % 5);
}

TEST(Meta, IsConstexpr)
{
    EXPECT_TRUE( (is_constexpr<decltype(isFive), &isFive>(5)) );
    EXPECT_TRUE( (is_constexpr<decltype(isFive), &isFive>(6)) );

    EXPECT_FALSE( (is_constexpr<decltype(isFiveOrRand), &isFiveOrRand>(5)) );
    EXPECT_FALSE( (is_constexpr<decltype(isFiveOrRand), &isFiveOrRand>(6)) );
}

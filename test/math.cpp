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

#define CGS_VIOLATE_THROW
#include "cgs/math.hpp"
using cgs::lerp;
using cgs::clamp;
using cgs::is_between;
using cgs::div_down;
using cgs::mod_down;
using cgs::divmod_down;

TEST(Math, LerpHalf)
{
    constexpr float a = 500.0f;
    constexpr float b = 1000.0f;
    constexpr float half = lerp(a, b, 0.5f);
    static_assert(half == 750.0f);
    EXPECT_EQ(half, 750.0f);
}

TEST(Math, LerpNaN)
{
    constexpr float a = std::numeric_limits<float>::quiet_NaN();
    constexpr float b = 1.0f;
    constexpr float half = lerp(a, b, 0.5f);
    // static check for NaN or Infinities
    static_assert(half != half);
    EXPECT_TRUE(std::isnan(half));
}

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
using cgs::isnan;
using cgs::detail::isnan_nobuiltin;

TEST(Math, LerpConstexpr)
{
    constexpr float a = 500.0f;
    constexpr float b = 1000.0f;
    constexpr float half = lerp(a, b, 0.5f);
    EXPECT_EQ(half, 750.0f);
}

TEST(Math, LerpInside)
{
    constexpr float a = 500.0f;
    constexpr float b = 1000.0f;
    EXPECT_EQ(lerp(a, b, 0.25f), 625.0f);
    EXPECT_EQ(lerp(a, b, 0.75f), 875.0f);
}

TEST(Math, LerpEdge)
{
    constexpr float a = 500.0f;
    constexpr float b = 1000.0f;
    EXPECT_EQ(lerp(a, b, 0.0f), a);
    EXPECT_EQ(lerp(a, b, 1.0f), b);
}

TEST(Math, LerpOutside)
{
    constexpr float a = 500.0f;
    constexpr float b = 1000.0f;
    EXPECT_EQ(lerp(a, b, -1.0f), 0.0f);
    EXPECT_EQ(lerp(a, b, 1.5f), 1250.0f);
}

TEST(Math, LerpNaN)
{
    constexpr float a = std::numeric_limits<float>::quiet_NaN();
    constexpr float b = 1.0f;
    constexpr float half = lerp(a, b, 0.5f);
    constexpr float bbNaN = lerp(b, b, a);
    EXPECT_TRUE(std::isnan(half));
    EXPECT_TRUE(std::isnan(bbNaN));
    EXPECT_TRUE(std::isnan(lerp(b, a, 0.5f)));
}

TEST(Math, LerpInfinities)
{
    constexpr float inf = std::numeric_limits<float>::infinity();

    // you can lerp in infinity either direction
    EXPECT_FLOAT_EQ(lerp(0.0f, 1.0f, -inf), -inf);
    EXPECT_FLOAT_EQ(lerp(0.0f, 1.0f, inf), inf);

    // lerping from infinity doesn't make any sense, always return quiet nan
    constexpr float nan = lerp(0.0f, inf, 1.0f);
    EXPECT_TRUE(std::isnan(nan));
    EXPECT_TRUE(std::isnan(lerp(0.0f, inf, 1.0f)));
    EXPECT_TRUE(std::isnan(lerp(0.0f, inf, -1.0f)));
    EXPECT_TRUE(std::isnan(lerp(1.0f, inf, 0.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, 0.0f, 0.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, 0.0f, 1.0f)));
    EXPECT_TRUE(std::isnan(lerp(inf, inf, inf)));
}

TEST(Math, LerpDouble)
{
    constexpr double a = 500.0;
    constexpr double b = 1000.0;
    constexpr double half = lerp(a, b, 0.5);
    EXPECT_EQ(half, 750.0);
}

TEST(Math, Clamp)
{
    static_assert(clamp(0.0, 0.0, 1.0) == 0.0);
    static_assert(clamp(0.0f, 0.0f, 1.0f) == 0.0f);
    static_assert(clamp(0, 0, 1) == 0);

    static_assert(clamp(-1.0, 0.0, 1.0) == 0.0);
    static_assert(clamp(-1, 0, 1) == 0);
    static_assert(clamp(2, 0, 1) == 1);

    static_assert(clamp(-INFINITY, 0.0f, 1.0f) == 0.0f);
    static_assert(clamp(INFINITY, 0.0f, 1.0f) == 1.0f);

    static_assert(clamp(-1000.0f, -INFINITY, 0.0f) == -1000.0f);
    static_assert(clamp(-1000.0f, 0.0f, INFINITY) == 0.0f);
}

TEST(Math, IsBetween)
{
    static_assert(is_between(0.0f, 0.0f, 1.0f));
    static_assert(is_between(0, 0, 1));
    static_assert(is_between(1, 0, 1));

    static_assert(!is_between(1, 1, 0));
    static_assert(!is_between(2, 0, 1));
    static_assert(!is_between(2, 0, 1));
}

TEST(Math, IsNaN)
{
#define is(x) static_assert(isnan(x)); static_assert(isnan_nobuiltin(x))
#define isnot(x) static_assert(!isnan(x)); static_assert(!isnan_nobuiltin(x))

    is(NAN);
    is(std::numeric_limits<float>::quiet_NaN());
    is(std::numeric_limits<float>::signaling_NaN());
    is(std::numeric_limits<double>::quiet_NaN());
    is(std::numeric_limits<double>::signaling_NaN());
    is(std::numeric_limits<long double>::quiet_NaN());
    is(std::numeric_limits<long double>::signaling_NaN());

    isnot(0.0f);
    isnot(0.0);
    isnot(static_cast<long double>(0));
    isnot(1.0f);
    isnot(1.0);
    isnot(static_cast<long double>(1.0));
    isnot(-1.0f);
    isnot(-1.0);
    isnot(static_cast<long double>(-1.0));
    isnot(INFINITY);
    isnot(-INFINITY);
    isnot(std::numeric_limits<float>::infinity());
    isnot(-std::numeric_limits<float>::infinity());
    isnot(std::numeric_limits<double>::infinity());
    isnot(-std::numeric_limits<double>::infinity());
    isnot(std::numeric_limits<long double>::infinity());
    isnot(-std::numeric_limits<long double>::infinity());
}

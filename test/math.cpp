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
using cgs::div;
using cgs::div_round_mode;
using cgs::mod;
using cgs::divmod;
using cgs::isnan;
using cgs::detail::isnan_nobuiltin;
using cgs::isfinite;
using cgs::detail::isfinite_nobuiltin;

TEST(Math, LerpConstexpr)
{
    constexpr float a = 500.0f;
    constexpr float b = 1000.0f;
    constexpr float half = lerp(a, b, 0.5f);
    static_assert(half == 750.0f);
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
    const float half = lerp(a, b, 0.5f);
    const float bbNaN = lerp(b, b, a);
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
    EXPECT_TRUE(std::isnan(lerp(0.0f, inf, 1.0f)));
    EXPECT_TRUE(std::isnan(lerp(0.0f, inf, -1.0f)));
    EXPECT_TRUE(std::isnan(lerp(1.0f, inf, 0.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, 0.0f, 0.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, 0.0f, 1.0f)));
    EXPECT_TRUE(std::isnan(lerp(inf, inf, inf)));
    EXPECT_TRUE(std::isnan(lerp(-inf, inf, -1.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, inf, 0.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, inf, 1.0f)));
    EXPECT_TRUE(std::isnan(lerp(-inf, inf, 2.0f)));
}

TEST(Math, LerpDouble)
{
    constexpr double a = 500.0;
    constexpr double b = 1000.0;
    const double half = lerp(a, b, 0.5);
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

#if 0 // static assertions
#define expect_dm(mode, n, d, expected_q, expected_r) \
    static_assert(div<mode>(n, d) == expected_q); \
    static_assert(mod<mode>(n, d) == expected_r); \
    static_assert(divmod<mode>(n, d).first == expected_q); \
    static_assert(divmod<mode>(n, d).second == expected_r)
#else // runtime expectations
#define expect_dm(mode, n, d, expected_q, expected_r) \
    EXPECT_EQ(div<mode>(n, d), expected_q); \
    EXPECT_EQ(mod<mode>(n, d), expected_r); \
    EXPECT_EQ(divmod<mode>(n, d).first, expected_q); \
    EXPECT_EQ(divmod<mode>(n, d).second, expected_r)
#endif

TEST(Math, DivModTrunc)
{
    // default rounding mode in modern C/C++

    // quotient copies sign from dividend,
    // remainder copies sign from divisor

    expect_dm(div_round_mode::trunc, -43,  10, -4, -3);
    expect_dm(div_round_mode::trunc,  43,  10,  4,  3);

    expect_dm(div_round_mode::trunc, -43, -10,  4, -3);
    expect_dm(div_round_mode::trunc,  43, -10, -4,  3);

    expect_dm(div_round_mode::trunc, -99, 11, -9, 0);
}

TEST(Math, DivModFloor)
{
    // n == d * q + r
    // quotient is floor(n / d)

    expect_dm(div_round_mode::floor, -43,  10, -5,  7);
    expect_dm(div_round_mode::floor,  43,  10,  4,  3);

    expect_dm(div_round_mode::floor,  43, -10, -5, -7);
    expect_dm(div_round_mode::floor, -43, -10,  4, -3);

    expect_dm(div_round_mode::floor, -99, 11, -9, 0);
}

TEST(Math, DivModEuclid)
{
    // n == d * q + r
    // remainder is always positive

    expect_dm(div_round_mode::euclid, -43,  10, -5,  7);
    expect_dm(div_round_mode::euclid,  43,  10,  4,  3);

    expect_dm(div_round_mode::euclid,  43, -10, -4,  3);
    expect_dm(div_round_mode::euclid, -43, -10,  5,  7);

    expect_dm(div_round_mode::euclid, -99, 11, -9, 0);
}

TEST(Math, IsNaN)
{
#define ISNAN(x) static_assert(cgs::isnan(x)); static_assert(isnan_nobuiltin(x)); EXPECT_TRUE(std::isnan(x))
#define ISNOTNAN(x) static_assert(!cgs::isnan(x)); static_assert(!isnan_nobuiltin(x)); EXPECT_FALSE(std::isnan(x))

    ISNAN(NAN);
    ISNAN(std::numeric_limits<float>::quiet_NaN());
    ISNAN(std::numeric_limits<float>::signaling_NaN());
    ISNAN(std::numeric_limits<double>::quiet_NaN());
    ISNAN(std::numeric_limits<double>::signaling_NaN());
    ISNAN(std::numeric_limits<long double>::quiet_NaN());
    ISNAN(std::numeric_limits<long double>::signaling_NaN());

    ISNOTNAN(0.0f);
    ISNOTNAN(0.0);
    ISNOTNAN(static_cast<long double>(0));
    ISNOTNAN(1.0f);
    ISNOTNAN(1.0);
    ISNOTNAN(static_cast<long double>(1.0));
    ISNOTNAN(-1.0f);
    ISNOTNAN(-1.0);
    ISNOTNAN(static_cast<long double>(-1.0));
    ISNOTNAN(INFINITY);
    ISNOTNAN(-INFINITY);
    ISNOTNAN(std::numeric_limits<float>::infinity());
    ISNOTNAN(-std::numeric_limits<float>::infinity());
    ISNOTNAN(std::numeric_limits<double>::infinity());
    ISNOTNAN(-std::numeric_limits<double>::infinity());
    ISNOTNAN(std::numeric_limits<long double>::infinity());
    ISNOTNAN(-std::numeric_limits<long double>::infinity());
}

TEST(Math, IsFinite)
{
#define ISFINITE(x) static_assert(cgs::isfinite(x)); static_assert(isfinite_nobuiltin(x)); EXPECT_TRUE(std::isfinite(x))
#define ISNOTFINITE(x) static_assert(!cgs::isfinite(x)); static_assert(!isfinite_nobuiltin(x)); EXPECT_FALSE(std::isfinite(x))

    ISFINITE(0.0f);
    ISFINITE(0.0);
    ISFINITE(static_cast<long double>(0));
    ISFINITE(1.0f);
    ISFINITE(1.0);
    ISFINITE(static_cast<long double>(1.0));
    ISFINITE(-1.0f);
    ISFINITE(-1.0);
    ISFINITE(static_cast<long double>(-1.0));

    ISNOTFINITE(NAN);
    ISNOTFINITE(std::numeric_limits<float>::quiet_NaN());
    ISNOTFINITE(std::numeric_limits<float>::signaling_NaN());
    ISNOTFINITE(std::numeric_limits<double>::quiet_NaN());
    ISNOTFINITE(std::numeric_limits<double>::signaling_NaN());
    ISNOTFINITE(std::numeric_limits<long double>::quiet_NaN());
    ISNOTFINITE(std::numeric_limits<long double>::signaling_NaN());

    ISNOTFINITE(INFINITY);
    ISNOTFINITE(-INFINITY);
    ISNOTFINITE(std::numeric_limits<float>::infinity());
    ISNOTFINITE(-std::numeric_limits<float>::infinity());
    ISNOTFINITE(std::numeric_limits<double>::infinity());
    ISNOTFINITE(-std::numeric_limits<double>::infinity());
    ISNOTFINITE(std::numeric_limits<long double>::infinity());
    ISNOTFINITE(-std::numeric_limits<long double>::infinity());
}

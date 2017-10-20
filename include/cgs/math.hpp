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
#ifndef CGS_MATH_HPP
#define CGS_MATH_HPP

#include "cgs/assert.hpp"
#include "cgs/meta.hpp" // is_constexpr

#include <utility> // pair
#include <cmath> // isnan, abs, tons of non constexpr stuff :(

namespace cgs
{

namespace detail
{

template <typename T>
constexpr bool isnan_nobuiltin(T value)
{
    return value != value;
}

} // namespace detail


template <typename T, typename F>
constexpr T lerp(const T& a, const T& b, F amount)
{
    // TODO which is faster?
    //return cgs_likely(std::isfinite(b - a))
    return cgs_likely(std::isfinite(a) && std::isfinite(b))
        ? a + (b - a) * amount
        : std::numeric_limits<T>::quiet_NaN();
}

template <typename T>
constexpr const T& clamp(const T& val, const T& min, const T& max)
{
    cgs_assert(min <= max);
    return val < max
        ? (val > min
            ? val
            : min)
        : max;
}

template <typename T>
constexpr bool is_between(const T& val, const T& min, const T& max)
{
    // min may be greater than max,
    // in which case val is never between them
    return min <= val && val <= max;
}

template <typename Int>
constexpr Int div_down(Int n, Int d)
{
    Int quotient = n / d;
    return (d * quotient <= n)
        ?  quotient
        : quotient - 1;
}

template <typename Int>
constexpr Int mod_down(Int n, Int d)
{
    Int remainder = n % d;
    return remainder >= 0
        ? remainder
        : remainder + d;
}

template <typename Int>
constexpr std::pair<Int, Int> divmod_down(Int n, Int d)
{
    Int quotient = div_down(n, d);
    Int remainder = n - d * quotient;
    return { quotient, remainder };
}

template <typename T>
constexpr bool isnan(T value)
{
    // std::isnan is not yet required to be constexpr
    constexpr auto standard = static_cast<bool(*)(T)>(std::isnan);

    // use standard if it is constexpr
    if constexpr(is_constexpr<standard, T>()) {
        return standard(value);
    }
    else {
        return detail::isnan_nobuiltin(value);
    }
}

} // namespace cgs

#endif // CGS_MATH_HPP

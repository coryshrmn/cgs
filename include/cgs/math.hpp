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
#include "cgs/meta/constexpr.hpp"

#include <cmath> // isnan, abs, tons of non constexpr stuff :(
#include <limits> // numeric_limits
#include <type_traits>
#include <utility> // pair

namespace cgs
{

template <typename T>
constexpr bool is_between(const T& val, const T& min, const T& max)
{
    // min may be greater than max,
    // in which case val is never between them
    return min <= val && val <= max;
}

template <typename T>
inline constexpr bool is_arithmetic_v = std::is_arithmetic<T>::value;

template <typename T>
inline constexpr bool is_integral_v = std::is_integral<T>::value;

template <typename T>
inline constexpr bool is_floating_point_v = std::is_floating_point<T>::value;

template <typename T>
inline constexpr bool is_signed_v = std::is_signed<T>::value;

namespace detail
{

template <typename T>
constexpr bool isnan_nobuiltin(T value)
{
    return value != value;
}

template <typename T>
constexpr std::enable_if_t<is_arithmetic_v<T>,
    bool> isfinite_nobuiltin(T value)
{
    if constexpr(is_integral_v<T>) {
        return true;
    }
    else {
        return is_between(value, -std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    }
}

} // namespace detail

template <typename T>
constexpr bool isnan(T value)
{

#if _GLIBCXX_HAVE_OBSOLETE_ISNAN \
  && !_GLIBCXX_NO_OBSOLETE_ISINF_ISNAN_DYNAMIC
// Workaround for old glibcxx:
// `std::isnan` is defined incorrectly,
// which prevents `is_constexpr<std::isnan>` from compiling.
#define CGS_BUILTIN_NAN_INVALID
#endif

#ifndef CGS_BUILTIN_NAN_INVALID
    // std::isnan is not yet required to be constexpr
    constexpr auto standard = static_cast<bool(*)(T)>(std::isnan);

    // use standard if it is constexpr
    if constexpr(is_constexpr<standard, T>()) {
        return standard(value);
    }
#endif

    return detail::isnan_nobuiltin(value);
}

template <typename T>
constexpr bool isfinite(T value)
{
    // std::isfinite is not yet required to be constexpr
    constexpr auto standard = static_cast<bool(*)(T)>(std::isfinite);

    // use standard if it is constexpr
    if constexpr(is_constexpr<standard, T>()) {
        return standard(value);
    }
    else {
        return detail::isfinite_nobuiltin(value);
    }
}

template <typename T>
constexpr T abs(T value)
{
    // std::abs is not yet required to be constexpr
    constexpr auto standard = static_cast<T(*)(T)>(std::abs);

    if constexpr(is_constexpr<standard, T>()) {
        return standard(value);
    }
    else {
        return value < T{} ? -value : value;
    }
}

template <typename T, typename F>
constexpr T lerp(const T& a, const T& b, F amount)
{
    // TODO which is faster?
    //return cgs_likely(cgs::isfinite(b - a))
    return cgs_likely(cgs::isfinite(a) && cgs::isfinite(b))
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

// see https://en.wikipedia.org/wiki/Modulo_operation#/media/File:Divmod.svg
enum class div_round_mode
{
    trunc,
    floor,
    euclid,
};

template <div_round_mode RoundMode, typename Int, typename = enable_if_t<is_integral_v<Int>>>
constexpr Int div(Int n, Int d)
{
    cgs_assert(d != 0);

    if constexpr(RoundMode == div_round_mode::trunc || !is_signed_v<Int>) {
        return n / d;
    }
    else if constexpr(RoundMode == div_round_mode::floor) {
        if((n < 0) != (d < 0)) {
            n -= d - 1;
        }
        return n / d;
    }
    else if constexpr(RoundMode == div_round_mode::euclid) {
        if(n < 0) {
            n -= cgs::abs(d) - 1;
        }
        return n / d;
    }
}

template <div_round_mode RoundMode, typename Int, typename = enable_if_t<is_integral_v<Int>>>
constexpr Int mod(Int n, Int d)
{
    cgs_assert(d != 0);

    const Int builtin = n % d;

    if constexpr(RoundMode == div_round_mode::trunc || !is_signed_v<Int>) {
        return builtin;
    }
    else if constexpr(RoundMode == div_round_mode::floor) {
        if((n < 0) != (d < 0) && builtin != 0) {
            return builtin + d;
        }
        return builtin;
    }
    else if constexpr(RoundMode == div_round_mode::euclid) {
        if(n < 0 && builtin != 0) {
            return builtin + cgs::abs(d);
        }
        return builtin;
    }
}

template <div_round_mode RoundMode, typename Int, typename = enable_if_t<is_integral_v<Int>>>
std::pair<Int, Int> divmod(Int n, Int d)
{
    return { div<RoundMode>(n, d), mod<RoundMode>(n, d) };
}

template <div_round_mode RoundMode, typename Int>
constexpr Int div_trunc(Int n, Int d)
{
    return div<div_round_mode::trunc>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr Int div_floor(Int n, Int d)
{
    return div<div_round_mode::floor>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr Int div_euclid(Int n, Int d)
{
    return div<div_round_mode::euclid>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr Int mod_trunc(Int n, Int d)
{
    return mod<div_round_mode::trunc>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr Int mod_floor(Int n, Int d)
{
    return mod<div_round_mode::floor>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr Int mod_euclid(Int n, Int d)
{
    return mod<div_round_mode::euclid>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr std::pair<Int, Int> divmod_trunc(Int n, Int d)
{
    return divmod<div_round_mode::trunc>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr std::pair<Int, Int> divmod_floor(Int n, Int d)
{
    return divmod<div_round_mode::floor>(n, d);
}

template <div_round_mode RoundMode, typename Int>
constexpr std::pair<Int, Int> divmod_euclid(Int n, Int d)
{
    return divmod<div_round_mode::euclid>(n, d);
}

} // namespace cgs

#endif // CGS_MATH_HPP

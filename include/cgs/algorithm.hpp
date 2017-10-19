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
#ifndef CGS_ALGORITHM_HPP
#define CGS_ALGORITHM_HPP

#include <type_traits>
#include <utility> // declval

// Similar to standard <algorithm> and <numeric>,
// with constexpr.

// We occasionally use a different parameter order than the standard.
// For example. transform_reduce takes the initial value last, so it can be defaulted {}.
// Also switched unary and binary operations because transform comes before reduce, intuitively.

namespace cgs
{

template <typename OutputIt, typename Sentinal, typename T>
constexpr void fill(OutputIt first, Sentinal last, const T& value)
{
    for(; first != last; ++first) {
        *first = value;
    }
}

template <typename InputIt, typename Sentinal, typename UnaryOp, typename BinaryOp,
          typename T = std::remove_reference_t<
              decltype(
                std::declval<UnaryOp>()(*std::declval<InputIt>())
              )
          >>
constexpr T transform_reduce(InputIt first, Sentinal last, UnaryOp unaryOp, BinaryOp binaryOp, T init = {})
{
    for(; first != last; ++first) {
        init = binaryOp(init, unaryOp(*first));
    }
    return init;
}

// std::min and std::max have various overloads,
// making them tedious to use compositionally (need to static_cast)
// we add min2 and max2, to help.

template <typename T>
constexpr const T& min2(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <typename T>
constexpr const T& max2(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

} // namespace cgs

#endif // CGS_ALGORITHM_HPP

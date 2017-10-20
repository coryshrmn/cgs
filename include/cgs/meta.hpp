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
#ifndef CGS_META_HPP
#define CGS_META_HPP

#include <type_traits>

namespace cgs
{

/**
 * @brief ignore the value of T, return integral type
 */
template <typename T>
constexpr bool ignore(T&&)
{
    return true;
}

namespace detail
{

template <
    typename FunctionType,
    std::decay_t<FunctionType> Function,
    typename... Args,
    bool = ignore(Function(Args{}...))
>
constexpr bool constexpr_check(bool&&)
{
    return true;
}

template <
    typename FunctionType,
    std::decay_t<FunctionType> Function,
    typename... Args
>
constexpr bool constexpr_check(const bool&&)
{
    return false;
}

} // namespace detail

/**
 * @brief Is this function constexpr with these argument types?
 */
template <
    typename FunctionType,
    std::decay_t<FunctionType> Function,
    typename... Args
>
constexpr bool is_constexpr(Args&&...)
{
    return detail::constexpr_check<FunctionType, Function, Args...>(false);
}

} // namespace cgs

#endif // CGS_META_HPP

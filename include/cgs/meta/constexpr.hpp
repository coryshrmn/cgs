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
#ifndef CGS_META_CONSTEXPR_HPP
#define CGS_META_CONSTEXPR_HPP

#include "cgs/meta/ignore.hpp"
#include "cgs/meta/invocable.hpp"

#include <type_traits>

namespace cgs
{

namespace detail
{

template <auto Func, typename... Args, bool = ignore(Func(Args{}...)) >
constexpr bool constexpr_check(bool&&)
{
    return true;
}

template <auto Func, typename... Args>
constexpr bool constexpr_check(const bool&&)
{
    return false;
}

} // namespace detail

/**
 * @brief Can Func be invoked at compile time, with value-initialized parameter values?
 *
 * We want to use enable_if<is_invocable>,
 * but `is_invocable` is not ubiquitous.
 *
 * The actual implementation for `is_invocable` is a bit messy (WIP in "cgs/meta/invocable.hpp"
 * So we use decltype(Func(declval<Args>()...)) instead.
 * It's close enough for my use cases, not as robust as is_invocable
 */
//template <auto Func, typename... Args, typename = std::enable_if_t<std::is_invocable_v<decltype(Func), Args...>> >
template <auto Func, typename... Args, typename = decltype(Func(Args{}...))>
constexpr bool is_constexpr()
{
    return detail::constexpr_check<Func, Args...>(false);
}

} // namespace cgs

#endif // CGS_META_CONSTEXPR_HPP

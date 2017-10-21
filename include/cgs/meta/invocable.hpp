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
#ifndef CGS_META_INVOCABLE_HPP
#define CGS_META_INVOCABLE_HPP

// disable, this is all a work in progress, may not compile
# if 0

#include <functional> // reference_wrapper
#include <type_traits>
#include <utility> // declval

// not my code, copied from cppreference
// hopefilly mine will be cleaner than this, but I'm using this as a reference.
namespace cppreference
{

namespace detail {
template <class T>
struct is_reference_wrapper : std::false_type {};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

template <class Base, class T, class Derived, class... Args>
auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
 -> typename std::enable_if<std::is_function<T>::value &&
                     std::is_base_of<Base, typename std::decay<Derived>::type>::value,
    decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>::type;

template <class Base, class T, class RefWrap, class... Args>
auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
 -> typename std::enable_if<std::is_function<T>::value &&
                     is_reference_wrapper<typename std::decay<RefWrap>::type>::value,
    decltype((ref.get().*pmf)(std::forward<Args>(args)...))>::type;

template <class Base, class T, class Pointer, class... Args>
auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
 -> typename std::enable_if<std::is_function<T>::value &&
                     !is_reference_wrapper<typename std::decay<Pointer>::type>::value &&
                     !std::is_base_of<Base, typename std::decay<Pointer>::type>::value,
    decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>::type;

template <class Base, class T, class Derived>
auto INVOKE(T Base::*pmd, Derived&& ref)
 -> typename std::enable_if<!std::is_function<T>::value &&
                     std::is_base_of<Base, typename std::decay<Derived>::type>::value,
    decltype(std::forward<Derived>(ref).*pmd)>::type;

template <class Base, class T, class RefWrap>
auto INVOKE(T Base::*pmd, RefWrap&& ref)
 -> typename std::enable_if<!std::is_function<T>::value &&
                     is_reference_wrapper<typename std::decay<RefWrap>::type>::value,
    decltype(ref.get().*pmd)>::type;

template <class Base, class T, class Pointer>
auto INVOKE(T Base::*pmd, Pointer&& ptr)
 -> typename std::enable_if<!std::is_function<T>::value &&
                     !is_reference_wrapper<typename std::decay<Pointer>::type>::value &&
                     !std::is_base_of<Base, typename std::decay<Pointer>::type>::value,
    decltype((*std::forward<Pointer>(ptr)).*pmd)>::type;

template <class F, class... Args>
auto INVOKE(F&& f, Args&&... args)
 -> typename std::enable_if<!std::is_member_pointer<typename std::decay<F>::type>::value,
    decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::type;
} // namespace detail

// Conforming C++14 implementation (is also a valid C++11 implementation):
namespace detail {
template <typename AlwaysVoid, typename, typename...>
struct invoke_result { };
template <typename F, typename...Args>
struct invoke_result<decltype(void(detail::INVOKE(std::declval<F>(), std::declval<Args>()...))),
                 F, Args...> {
    using type = decltype(detail::INVOKE(std::declval<F>(), std::declval<Args>()...));
};
} // namespace detail

template <class F, class... ArgTypes>
struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};

} // namespace cppreference



namespace cgs
{

namespace detail
{

template <typename T>
struct is_reference_wrapper : std::false_type { };

template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type { };

template <typename T>
inline constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

// invoke member function
template <typename Base, typename T, typename Derived, typename... Args>
auto invoke(T Base::*member_func, Derived&& derived, Args&&... args)
    -> std::enable_if_t<
        std::is_function_v<T> && std::is_base_of_v<Base, std::decay_t<Derived>>,
        decltype( (std::forward<Derived>(derived).*member_func)(std::forward<Args>(args)...) )
    >;

// invoke member function through reference_wrapper
template <typename Base, typename T, typename ReferenceWrapper, typename... Args>
auto invoke(T Base::*member_func, ReferenceWrapper&& wrapper, Args&&... args)
    -> std::enable_if_t<
        std::is_function_v<T> && is_reference_wrapper_v<std::decay_t<ReferenceWrapper>>,
        decltype( (wrapper.get().*member_func)(std::forward<Args>(args)...) )
    >;

template <typename T>
constexpr bool is_member_pointer_v = std::is_member_pointer<T>::value;

template <typename T>
using decay_t = typename std::decay<T>::type;

template <bool Enabled, typename T = void>
using enable_if_t = typename std::enable_if<Enabled, T>::value;

// invoke ordinary function or functor
template <typename Func, typename... Args>
auto invoke(Func&& func, Args&&... args)
    -> enable_if_t<
        !is_member_pointer_v(decay_t<Func>),
        decltype(std::forward<Func>(func)(std::forward<Args>(args)...))
    >;

template <typename Void, typename, typename...>
struct invoke_result_impl { };

template <typename Func, typename Args...>
struct invoke_result_impl<
    decltype(void(invoke(std::declval<Func>(), std::declval<Args>()...))),
    Func,
    Args...
>
{
    using type = decltype(invoke(std::declval<Func>(), std::declval<Args>()...));
};

} // namespace detail

template <typename Func, typename... Args>
struct invoke_result : detail::invoce_result_impl<void, Func, Args...> { };

template <typename Func, typename... Args>
using invoke_result_t = typename invoke_result<Fun, Args...>::type;

template <typename Func, typename... Args>
inline constexpr bool is_invocable_v = is_invocable<Func, Args...>::value;

} // namespace cgs

#endif // 0

#endif // CGS_META_INVOCABLE_HPP


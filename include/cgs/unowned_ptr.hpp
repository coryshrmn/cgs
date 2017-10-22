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
#ifndef CGS_UNOWNED_PTR_HPP
#define CGS_UNOWNED_PTR_HPP

#include "cgs/assert.hpp"
#include "cgs/meta.hpp"

namespace cgs
{

template <typename T>
class unowned_ptr
{
private:

    T* _p {};

public:

    constexpr unowned_ptr() noexcept = default;

    constexpr /* implicit */ unowned_ptr(T* p) noexcept
        : _p(p)
    { }

    template < typename U, typename = enable_if_t<is_convertible_v<U*, T*>> >
    constexpr /* implicit */ unowned_ptr(const unowned_ptr<U>& source) noexcept
        : _p(source.or_null())
    { }

    /**
     * @brief Use the pointer, must not be null.
     */
    constexpr T& operator*() const
    {
        cgs_assert(_p);
        return *_p;
    }

    /**
     * @brief Use the pointer, must not be null.
     */
    constexpr T* operator->() const
    {
        cgs_assert(_p);
        return _p;
    }

    /**
     * @brief Use the pointer to call a function, must not be null.
     */
    template <typename... Args>
    constexpr auto operator()(Args&&... args) const
    {
        cgs_assert(_p);
        return _p(std::forward<Args>(args)...);
    }

    /**
     * @brief Explicitly get the pointer, even if it is null.
     */
    constexpr T* or_null() const noexcept
    {
        return _p;
    }

    /**
     * @brief Is this not null?
     */
    constexpr operator bool() const noexcept
    {
        return static_cast<bool>(_p);
    }

    /**
     * @brief Is this null?
     */
    constexpr bool operator!() const noexcept
    {
        return !_p;
    }

    /**
     * @brief Compares pointers, not values.
     */
    constexpr bool operator==(const unowned_ptr& rhs) const noexcept
    {
        return _p == rhs._p;
    }

    /**
     * @brief Compares pointers, not values.
     */
    constexpr bool operator!=(const unowned_ptr& rhs) const noexcept
    {
        return _p != rhs._p;
    }
};

} //namespace cgs

#endif // CGS_UNOWNED_PTR_HPP

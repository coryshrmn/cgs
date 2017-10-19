#ifndef CGS_UNOWNED_PTR_HPP
#define CGS_UNOWNED_PTR_HPP

#include "cgs/assert.hpp"

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

    constexpr unowned_ptr& operator=(T* p)
    {
        _p = p;
        return *this;
    }

    constexpr T& operator*() const
    {
        cgs_assert(_p);
        return *_p;
    }

    constexpr T* operator->() const
    {
        cgs_assert(_p);
        return _p;
    }

    constexpr T* or_null() const
    {
        return _p;
    }

    constexpr operator bool() const noexcept
    {
        return static_cast<bool>(_p);
    }

    constexpr bool operator!() const noexcept
    {
        return !_p;
    }

    constexpr bool operator==(const unowned_ptr& rhs) const noexcept
    {
        return _p == rhs._p;
    }

    constexpr bool operator!=(const unowned_ptr& rhs) const noexcept
    {
        return _p != rhs._p;
    }

    template <typename... Args>
    auto operator()(Args&&... args)
    {
        cgs_assert(_p);
        return _p(std::forward<Args>(args)...);
    }
};

} //namespace cgs

#endif // CGS_UNOWNED_PTR_HPP

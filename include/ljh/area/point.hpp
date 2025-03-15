
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// point.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++23
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include <cstddef>
#include <format>

#if __has_include(<glm/fwd.hpp>)
#include <glm/fwd.hpp>
#endif

namespace ljh
{
    template<typename T>
    struct basic_size;

    template<typename T>
    struct basic_point
    {
        using value_type  = T;
        using length_type = std::size_t;

        value_type x = 0;
        value_type y = 0;

        static constexpr length_type length() noexcept
        {
            return 2;
        }

        constexpr basic_point() noexcept                     = default;
        constexpr basic_point(basic_point const& v) noexcept = default;
        constexpr basic_point(basic_point&& v) noexcept      = default;

        constexpr basic_point(T x, T y) noexcept;

        template<typename X = value_type, typename Y = value_type>
        constexpr basic_point(X x, Y y) noexcept;

        constexpr basic_point& operator=(basic_point const& v) noexcept = default;
        constexpr basic_point& operator=(basic_point&& v) noexcept      = default;

        constexpr ~basic_point() noexcept = default;

        template<typename U>
        constexpr explicit basic_point(basic_point<U> const& v) noexcept;
        template<typename U>
        constexpr basic_point& operator=(basic_point<U> const& v) noexcept;

#ifdef GLM_VERSION
        template<typename U, glm::qualifier P>
        constexpr explicit basic_point(glm::vec<2, U, P> const& point) noexcept;
        template<typename U, glm::qualifier P>
        constexpr explicit operator glm::vec<2, U, P>() const noexcept;
#endif

        constexpr value_type&       operator[](length_type i) noexcept;
        constexpr value_type const& operator[](length_type i) const noexcept;

        constexpr bool operator==(basic_point const& rhs) const noexcept;
        constexpr bool operator!=(basic_point const& rhs) const noexcept = default;

        constexpr basic_point operator+(basic_point const& rhs) const noexcept;
        constexpr basic_point operator-(basic_point const& rhs) const noexcept;
        constexpr basic_point operator/(basic_point const& rhs) const noexcept;
        constexpr basic_point operator*(basic_point const& rhs) const noexcept;

        constexpr basic_point& operator+=(basic_point const& rhs) noexcept;
        constexpr basic_point& operator-=(basic_point const& rhs) noexcept;
        constexpr basic_point& operator/=(basic_point const& rhs) noexcept;
        constexpr basic_point& operator*=(basic_point const& rhs) noexcept;

        constexpr basic_point  operator/(basic_size<T> const& rhs) const noexcept;
        constexpr basic_point& operator/=(basic_size<T> const& rhs) noexcept;

        template<size_t I, typename S>
        constexpr decltype(auto) get(this S&& self) noexcept;
    };

    using point  = basic_point<float>;
    using ipoint = basic_point<int>;
} // namespace ljh

namespace std
{
    template<typename T>
    struct tuple_size<ljh::basic_point<T>> : integral_constant<size_t, 2>
    {};

    template<size_t I, typename T>
    struct tuple_element<I, ljh::basic_point<T>>
    {
        using type = T;
    };

    template<typename T, typename C>
    struct formatter<ljh::basic_point<T>, C>
    {
        template<typename PC>
        constexpr PC::iterator parse(PC& ctx)
        {
            return ctx.begin();
        }

        template<typename FC>
        FC::iterator format(ljh::basic_point<T> const& value, FC& ctx) const
        {
            return std::format_to(ctx.out(), "{},{}", value.x, value.y);
        }
    };
} // namespace std

namespace ljh
{
    template<typename T>
    inline constexpr basic_point<T>::basic_point(T x, T y) noexcept
        : x(x)
        , y(y)
    {}

    template<typename T>
    template<typename X, typename Y>
    inline constexpr basic_point<T>::basic_point(X x, Y y) noexcept
        : x(static_cast<value_type>(x))
        , y(static_cast<value_type>(y))
    {}

    template<typename T>
    template<typename U>
    inline constexpr basic_point<T>::basic_point(basic_point<U> const& v) noexcept
        : x(static_cast<value_type>(v.x))
        , y(static_cast<value_type>(v.y))
    {}

    template<typename T>
    template<typename U>
    inline constexpr basic_point<T>& basic_point<T>::operator=(basic_point<U> const& v) noexcept
    {
        x = static_cast<value_type>(v.x);
        y = static_cast<value_type>(v.y);
        return *this;
    }

#ifdef GLM_VERSION
    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr basic_point<T>::basic_point(glm::vec<2, U, P> const& point) noexcept
        : x(static_cast<value_type>(point.x))
        , y(static_cast<value_type>(point.y))
    {}

    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr basic_point<T>::operator glm::vec<2, U, P>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }
#endif

    template<typename T>
    inline constexpr basic_point<T>::value_type& basic_point<T>::operator[](length_type i) noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        }
    }

    template<typename T>
    inline constexpr basic_point<T>::value_type const& basic_point<T>::operator[](length_type i) const noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        case 0: return x;
        case 1: return y;
        }
    }

    template<typename T>
    inline constexpr bool basic_point<T>::operator==(basic_point<T> const& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y;
    }

    template<typename T>
    inline constexpr basic_point<T> basic_point<T>::operator+(basic_point const& rhs) const noexcept
    {
        return {x + rhs.x, y + rhs.y};
    }

    template<typename T>
    inline constexpr basic_point<T> basic_point<T>::operator-(basic_point const& rhs) const noexcept
    {
        return {x - rhs.x, y - rhs.y};
    }

    template<typename T>
    inline constexpr basic_point<T> basic_point<T>::operator/(basic_point const& rhs) const noexcept
    {
        return {x / rhs.x, y / rhs.y};
    }

    template<typename T>
    inline constexpr basic_point<T> basic_point<T>::operator*(basic_point const& rhs) const noexcept
    {
        return {x * rhs.x, y * rhs.y};
    }

    template<typename T>
    inline constexpr basic_point<T>& basic_point<T>::operator+=(basic_point const& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr basic_point<T>& basic_point<T>::operator-=(basic_point const& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr basic_point<T>& basic_point<T>::operator/=(basic_point const& rhs) noexcept
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr basic_point<T>& basic_point<T>::operator*=(basic_point const& rhs) noexcept
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr basic_point<T> basic_point<T>::operator/(basic_size<T> const& rhs) const noexcept
    {
        return {x / rhs.w, y / rhs.h};
    }

    template<typename T>
    inline constexpr basic_point<T>& basic_point<T>::operator/=(basic_size<T> const& rhs) noexcept
    {
        x /= rhs.w;
        y /= rhs.h;
        return *this;
    }

    template<typename T>
    template<size_t I, typename S>
    inline constexpr decltype(auto) basic_point<T>::get(this S&& self) noexcept
    {
        if constexpr (I == 0)
            return self.x;
        else if constexpr (I == 1)
            return self.y;
    }
} // namespace ljh

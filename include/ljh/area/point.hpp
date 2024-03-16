
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// point.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include <cstddef>

#if __has_include(<glm/fwd.hpp>)
#include <glm/fwd.hpp>
#endif

namespace ljh
{
    template<typename T>
    struct tsize;

    template<typename T>
    struct tpoint
    {
        using value_type  = T;
        using length_type = std::size_t;

        value_type x = 0;
        value_type y = 0;

        static constexpr length_type length() noexcept
        {
            return 2;
        }

        constexpr tpoint() noexcept                = default;
        constexpr tpoint(tpoint const& v) noexcept = default;
        constexpr tpoint(tpoint&& v) noexcept      = default;

        template<typename X = value_type, typename Y = value_type>
        constexpr tpoint(X x, Y y) noexcept;

        constexpr tpoint& operator=(tpoint const& v) noexcept = default;
        constexpr tpoint& operator=(tpoint&& v) noexcept      = default;

        constexpr ~tpoint() noexcept = default;

        template<typename U>
        constexpr explicit tpoint(tpoint<U> const& v) noexcept;
        template<typename U>
        constexpr tpoint& operator=(tpoint<U> const& v) noexcept;

#ifdef GLM_VERSION
        template<typename U, glm::qualifier P>
        constexpr explicit tpoint(glm::vec<2, U, P> const& point) noexcept;
        template<typename U, glm::qualifier P>
        constexpr explicit operator glm::vec<2, U, P>() const noexcept;
#endif

        constexpr value_type&       operator[](length_type i) noexcept;
        constexpr value_type const& operator[](length_type i) const noexcept;

        constexpr bool operator==(tpoint const& rhs) const noexcept;
        constexpr bool operator!=(tpoint const& rhs) const noexcept = default;

        constexpr tpoint operator+(tpoint const& rhs) const noexcept;
        constexpr tpoint operator-(tpoint const& rhs) const noexcept;
        constexpr tpoint operator/(tpoint const& rhs) const noexcept;
        constexpr tpoint operator*(tpoint const& rhs) const noexcept;

        constexpr tpoint& operator+=(tpoint const& rhs) noexcept;
        constexpr tpoint& operator-=(tpoint const& rhs) noexcept;
        constexpr tpoint& operator/=(tpoint const& rhs) noexcept;
        constexpr tpoint& operator*=(tpoint const& rhs) noexcept;

        constexpr tpoint  operator/(tsize<T> const& rhs) const noexcept;
        constexpr tpoint& operator/=(tsize<T> const& rhs) noexcept;
    };

    using point  = tpoint<float>;
    using ipoint = tpoint<int>;

    template<typename T>
    template<typename X, typename Y>
    inline constexpr tpoint<T>::tpoint(X x, Y y) noexcept
        : x(static_cast<value_type>(x))
        , y(static_cast<value_type>(y))
    {}

    template<typename T>
    template<typename U>
    inline constexpr tpoint<T>::tpoint(tpoint<U> const& v) noexcept
        : x(static_cast<value_type>(v.x))
        , y(static_cast<value_type>(v.y))
    {}

    template<typename T>
    template<typename U>
    inline constexpr tpoint<T>& tpoint<T>::operator=(tpoint<U> const& v) noexcept
    {
        x = static_cast<value_type>(v.x);
        y = static_cast<value_type>(v.y);
        return *this;
    }

#ifdef GLM_VERSION
    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr tpoint<T>::tpoint(glm::vec<2, U, P> const& point) noexcept
        : x(static_cast<value_type>(point.x))
        , y(static_cast<value_type>(point.y))
    {}

    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr tpoint<T>::operator glm::vec<2, U, P>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }
#endif

    template<typename T>
    inline constexpr tpoint<T>::value_type& tpoint<T>::operator[](typename tpoint<T>::length_type i) noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        default:
        case 0: return x;
        case 1: return y;
        }
    }

    template<typename T>
    inline constexpr tpoint<T>::value_type const& tpoint<T>::operator[](typename tpoint<T>::length_type i) const noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        default:
        case 0: return x;
        case 1: return y;
        }
    }

    template<typename T>
    inline constexpr bool tpoint<T>::operator==(tpoint<T> const& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y;
    }

    template<typename T>
    inline constexpr tpoint<T> tpoint<T>::operator+(tpoint const& rhs) const noexcept
    {
        return {x + rhs.x, y + rhs.y};
    }

    template<typename T>
    inline constexpr tpoint<T> tpoint<T>::operator-(tpoint const& rhs) const noexcept
    {
        return {x - rhs.x, y - rhs.y};
    }

    template<typename T>
    inline constexpr tpoint<T> tpoint<T>::operator/(tpoint const& rhs) const noexcept
    {
        return {x / rhs.x, y / rhs.y};
    }

    template<typename T>
    inline constexpr tpoint<T> tpoint<T>::operator*(tpoint const& rhs) const noexcept
    {
        return {x * rhs.x, y * rhs.y};
    }

    template<typename T>
    inline constexpr tpoint<T>& tpoint<T>::operator+=(tpoint const& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr tpoint<T>& tpoint<T>::operator-=(tpoint const& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr tpoint<T>& tpoint<T>::operator/=(tpoint const& rhs) noexcept
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr tpoint<T>& tpoint<T>::operator*=(tpoint const& rhs) noexcept
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    template<typename T>
    constexpr tpoint<T> tpoint<T>::operator/(tsize<T> const& rhs) const noexcept
    {
        return {x / rhs.w, y / rhs.h};
    }

    template<typename T>
    constexpr tpoint<T>& tpoint<T>::operator/=(tsize<T> const& rhs) noexcept
    {
        x /= rhs.w;
        y /= rhs.h;
        return *this;
    }
} // namespace ljh

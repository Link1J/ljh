
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// size.hpp - v1.0
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
    struct basic_size
    {
        using value_type  = T;
        using length_type = size_t;

        value_type w = 0;
        value_type h = 0;

        static constexpr length_type length() noexcept
        {
            return 2;
        }

        constexpr basic_size() noexcept                    = default;
        constexpr basic_size(basic_size const& v) noexcept = default;
        constexpr basic_size(basic_size&& v) noexcept      = default;

        constexpr basic_size(T w, T h) noexcept;

        template<typename W = value_type, typename H = value_type>
        constexpr basic_size(W w, H h) noexcept;

        constexpr basic_size& operator=(basic_size const& v) noexcept = default;
        constexpr basic_size& operator=(basic_size&& v) noexcept      = default;

        constexpr ~basic_size() noexcept = default;

        template<typename U>
        constexpr explicit basic_size(basic_size<U> const& v) noexcept;
        template<typename U>
        constexpr basic_size& operator=(basic_size<U> const& v) noexcept;

#ifdef GLM_VERSION
        template<typename U, glm::qualifier P>
        constexpr explicit basic_size(glm::vec<2, U, P> const& size) noexcept;
        template<typename U, glm::qualifier P>
        constexpr explicit operator glm::vec<2, U, P>() const noexcept;
#endif

        constexpr value_type&       operator[](length_type i) noexcept;
        constexpr value_type const& operator[](length_type i) const noexcept;

        constexpr bool operator==(basic_size const& rhs) const noexcept;
        constexpr bool operator!=(basic_size const& rhs) const noexcept = default;

        constexpr basic_size operator+(basic_size const& rhs) const noexcept;
        constexpr basic_size operator-(basic_size const& rhs) const noexcept;
        constexpr basic_size operator/(basic_size const& rhs) const noexcept;
        constexpr basic_size operator*(basic_size const& rhs) const noexcept;

        constexpr basic_size& operator+=(basic_size const& rhs) noexcept;
        constexpr basic_size& operator-=(basic_size const& rhs) noexcept;
        constexpr basic_size& operator/=(basic_size const& rhs) noexcept;
        constexpr basic_size& operator*=(basic_size const& rhs) noexcept;

        template<size_t I, typename S>
        constexpr decltype(auto) get(this S&& self) noexcept;
    };

    using size  = basic_size<float>;
    using isize = basic_size<int>;
} // namespace ljh

namespace std
{
    template<typename T>
    struct tuple_size<ljh::basic_size<T>> : integral_constant<size_t, 2>
    {};

    template<size_t I, typename T>
    struct tuple_element<I, ljh::basic_size<T>>
    {
        using type = T;
    };

    template<typename T, typename C>
    struct formatter<ljh::basic_size<T>, C>
    {
        template<typename PC>
        constexpr PC::iterator parse(PC& ctx)
        {
            return ctx.begin();
        }

        template<typename FC>
        FC::iterator format(ljh::basic_size<T> const& value, FC& ctx) const
        {
            return std::format_to(ctx.out(), "{}x{}", value.w, value.h);
        }
    };
} // namespace std

namespace ljh
{
    template<typename T>
    inline constexpr basic_size<T>::basic_size(T w, T h) noexcept
        : w(w)
        , h(h)
    {}

    template<typename T>
    template<typename W, typename H>
    inline constexpr basic_size<T>::basic_size(W w, H h) noexcept
        : w(static_cast<value_type>(w))
        , h(static_cast<value_type>(h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr basic_size<T>::basic_size(basic_size<U> const& v) noexcept
        : w(static_cast<value_type>(v.w))
        , h(static_cast<value_type>(v.h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr basic_size<T>& basic_size<T>::operator=(basic_size<U> const& v) noexcept
    {
        w = static_cast<value_type>(v.w);
        h = static_cast<value_type>(v.h);
        return *this;
    }

#ifdef GLM_VERSION
    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr basic_size<T>::basic_size(glm::vec<2, U, P> const& size) noexcept
        : w(static_cast<value_type>(size.x))
        , h(static_cast<value_type>(size.y))
    {}

    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr basic_size<T>::operator glm::vec<2, U, P>() const noexcept
    {
        return {static_cast<U>(w), static_cast<U>(h)};
    }
#endif

    template<typename T>
    inline constexpr basic_size<T>::value_type& basic_size<T>::operator[](typename basic_size<T>::length_type i) noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        default:
        case 0: return w;
        case 1: return h;
        }
    }

    template<typename T>
    inline constexpr basic_size<T>::value_type const& basic_size<T>::operator[](typename basic_size<T>::length_type i) const noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        default:
        case 0: return w;
        case 1: return h;
        }
    }

    template<typename T>
    inline constexpr bool basic_size<T>::operator==(basic_size<T> const& rhs) const noexcept
    {
        return w == rhs.w && h == rhs.h;
    }

    template<typename T>
    inline constexpr basic_size<T> basic_size<T>::operator+(basic_size const& rhs) const noexcept
    {
        return {w + rhs.w, h + rhs.h};
    }

    template<typename T>
    inline constexpr basic_size<T> basic_size<T>::operator-(basic_size const& rhs) const noexcept
    {
        return {w - rhs.w, h - rhs.h};
    }

    template<typename T>
    inline constexpr basic_size<T> basic_size<T>::operator/(basic_size const& rhs) const noexcept
    {
        return {w / rhs.w, h / rhs.h};
    }

    template<typename T>
    inline constexpr basic_size<T> basic_size<T>::operator*(basic_size const& rhs) const noexcept
    {
        return {w * rhs.w, h * rhs.h};
    }

    template<typename T>
    inline constexpr basic_size<T>& basic_size<T>::operator+=(basic_size const& rhs) noexcept
    {
        w += rhs.w;
        h += rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr basic_size<T>& basic_size<T>::operator-=(basic_size const& rhs) noexcept
    {
        w -= rhs.w;
        h -= rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr basic_size<T>& basic_size<T>::operator/=(basic_size const& rhs) noexcept
    {
        w /= rhs.w;
        h /= rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr basic_size<T>& basic_size<T>::operator*=(basic_size const& rhs) noexcept
    {
        w *= rhs.w;
        h *= rhs.h;
        return *this;
    }

    template<typename T>
    template<size_t I, typename S>
    inline constexpr decltype(auto) basic_size<T>::get(this S&& self) noexcept
    {
        if constexpr (I == 0)
            return self.w;
        else if constexpr (I == 1)
            return self.h;
    }
} // namespace ljh

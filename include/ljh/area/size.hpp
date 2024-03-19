
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
    struct tsize
    {
        using value_type  = T;
        using length_type = size_t;

        value_type w = 0;
        value_type h = 0;

        static constexpr length_type length() noexcept
        {
            return 2;
        }

        constexpr tsize() noexcept               = default;
        constexpr tsize(tsize const& v) noexcept = default;
        constexpr tsize(tsize&& v) noexcept      = default;

        constexpr tsize(T w, T h) noexcept;

        template<typename W = value_type, typename H = value_type>
        constexpr tsize(W w, H h) noexcept;

        constexpr tsize& operator=(tsize const& v) noexcept = default;
        constexpr tsize& operator=(tsize&& v) noexcept      = default;

        constexpr ~tsize() noexcept = default;

        template<typename U>
        constexpr explicit tsize(tsize<U> const& v) noexcept;
        template<typename U>
        constexpr tsize& operator=(tsize<U> const& v) noexcept;

#ifdef GLM_VERSION
        template<typename U, glm::qualifier P>
        constexpr explicit tsize(glm::vec<2, U, P> const& size) noexcept;
        template<typename U, glm::qualifier P>
        constexpr explicit operator glm::vec<2, U, P>() const noexcept;
#endif

        constexpr value_type&       operator[](length_type i) noexcept;
        constexpr value_type const& operator[](length_type i) const noexcept;

        constexpr bool operator==(tsize const& rhs) const noexcept;
        constexpr bool operator!=(tsize const& rhs) const noexcept = default;

        constexpr tsize operator+(tsize const& rhs) const noexcept;
        constexpr tsize operator-(tsize const& rhs) const noexcept;
        constexpr tsize operator/(tsize const& rhs) const noexcept;
        constexpr tsize operator*(tsize const& rhs) const noexcept;

        constexpr tsize& operator+=(tsize const& rhs) noexcept;
        constexpr tsize& operator-=(tsize const& rhs) noexcept;
        constexpr tsize& operator/=(tsize const& rhs) noexcept;
        constexpr tsize& operator*=(tsize const& rhs) noexcept;

        template<size_t I, typename S>
        constexpr decltype(auto) get(this S&& self) noexcept;
    };

    using size  = tsize<float>;
    using isize = tsize<int>;
} // namespace ljh

namespace std
{
    template<typename T>
    struct tuple_size<ljh::tsize<T>> : integral_constant<size_t, 2>
    {};

    template<size_t I, typename T>
    struct tuple_element<I, ljh::tsize<T>>
    {
        using type = T;
    };

    template<typename T, typename C>
    struct formatter<ljh::tsize<T>, C>
    {
        template<typename PC>
        constexpr PC::iterator parse(PC& ctx)
        {
            return ctx.begin();
        }

        template<typename FC>
        FC::iterator format(ljh::tsize<T> const& value, FC& ctx) const
        {
            return std::format_to(ctx.out(), "{}x{}", value.w, value.h);
        }
    };
} // namespace std

namespace ljh
{
    template<typename T>
    inline constexpr tsize<T>::tsize(T w, T h) noexcept
        : w(w)
        , h(h)
    {}

    template<typename T>
    template<typename W, typename H>
    inline constexpr tsize<T>::tsize(W w, H h) noexcept
        : w(static_cast<value_type>(w))
        , h(static_cast<value_type>(h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr tsize<T>::tsize(tsize<U> const& v) noexcept
        : w(static_cast<value_type>(v.w))
        , h(static_cast<value_type>(v.h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr tsize<T>& tsize<T>::operator=(tsize<U> const& v) noexcept
    {
        w = static_cast<value_type>(v.w);
        h = static_cast<value_type>(v.h);
        return *this;
    }

#ifdef GLM_VERSION
    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr tsize<T>::tsize(glm::vec<2, U, P> const& size) noexcept
        : w(static_cast<value_type>(size.x))
        , h(static_cast<value_type>(size.y))
    {}

    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr tsize<T>::operator glm::vec<2, U, P>() const noexcept
    {
        return {static_cast<U>(w), static_cast<U>(h)};
    }
#endif

    template<typename T>
    inline constexpr tsize<T>::value_type& tsize<T>::operator[](typename tsize<T>::length_type i) noexcept
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
    inline constexpr tsize<T>::value_type const& tsize<T>::operator[](typename tsize<T>::length_type i) const noexcept
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
    inline constexpr bool tsize<T>::operator==(tsize<T> const& rhs) const noexcept
    {
        return w == rhs.w && h == rhs.h;
    }

    template<typename T>
    inline constexpr tsize<T> tsize<T>::operator+(tsize const& rhs) const noexcept
    {
        return {w + rhs.w, h + rhs.h};
    }

    template<typename T>
    inline constexpr tsize<T> tsize<T>::operator-(tsize const& rhs) const noexcept
    {
        return {w - rhs.w, h - rhs.h};
    }

    template<typename T>
    inline constexpr tsize<T> tsize<T>::operator/(tsize const& rhs) const noexcept
    {
        return {w / rhs.w, h / rhs.h};
    }

    template<typename T>
    inline constexpr tsize<T> tsize<T>::operator*(tsize const& rhs) const noexcept
    {
        return {w * rhs.w, h * rhs.h};
    }

    template<typename T>
    inline constexpr tsize<T>& tsize<T>::operator+=(tsize const& rhs) noexcept
    {
        w += rhs.w;
        h += rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr tsize<T>& tsize<T>::operator-=(tsize const& rhs) noexcept
    {
        w -= rhs.w;
        h -= rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr tsize<T>& tsize<T>::operator/=(tsize const& rhs) noexcept
    {
        w /= rhs.w;
        h /= rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr tsize<T>& tsize<T>::operator*=(tsize const& rhs) noexcept
    {
        w *= rhs.w;
        h *= rhs.h;
        return *this;
    }

    template<typename T>
    template<size_t I, typename S>
    inline constexpr decltype(auto) tsize<T>::get(this S&& self) noexcept
    {
        if constexpr (I == 0)
            return self.w;
        else if constexpr (I == 1)
            return self.h;
    }
} // namespace ljh

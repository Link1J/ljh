
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// rect.hpp - v1.0
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
#include "../cpp_version.hpp"
#include "../type_traits.hpp"
#include "point.hpp"
#include "size.hpp"

#include <utility>
#include <algorithm>
#include <format>
#include <cassert>

namespace ljh
{
    LJH_MODULE_MATH_EXPORT template<typename T>
    struct basic_rect
    {
        using value_type  = T;
        using length_type = std::size_t;
        using point_type  = basic_point<T>;
        using size_type   = basic_size<T>;

        value_type x = 0;
        value_type y = 0;
        value_type w = 0;
        value_type h = 0;

        static constexpr length_type length() noexcept
        {
            return 4;
        }

        constexpr basic_rect() noexcept                    = default;
        constexpr basic_rect(basic_rect const& v) noexcept = default;
        constexpr basic_rect(basic_rect&& v) noexcept      = default;

        constexpr basic_rect(T x, T y, T w, T h) noexcept;
        constexpr basic_rect(basic_point<T> const& pos, basic_size<T> const& size) noexcept;

        template<typename X = value_type, typename Y = value_type, typename W = value_type, typename H = value_type>
        constexpr basic_rect(X x, Y y, W w, H h) noexcept;
        template<typename P = value_type, typename S = value_type>
        constexpr basic_rect(basic_point<P> const& pos, basic_size<S> const& size) noexcept;
        template<typename P1, typename P2>
        constexpr basic_rect(basic_point<P1> const& top_left, basic_point<P2> const& bottom_right) noexcept;

        constexpr basic_rect& operator=(basic_rect const& v) noexcept = default;
        constexpr basic_rect& operator=(basic_rect&& v) noexcept      = default;

        constexpr ~basic_rect() noexcept = default;

        template<typename U>
        constexpr explicit operator basic_point<U>() const noexcept;
        template<typename U>
        constexpr explicit operator basic_size<U>() const noexcept;

        template<typename U>
        constexpr explicit basic_rect(basic_rect<U> const& v) noexcept;
        template<typename U>
        constexpr basic_rect& operator=(basic_rect<U> const& v) noexcept;

#ifdef GLM_VERSION
        template<typename U, glm::qualifier P>
        constexpr explicit basic_rect(glm::vec<4, U, P> const& pos_size) noexcept;
        template<typename U, glm::qualifier P>
        constexpr explicit operator glm::vec<4, U, P>() const noexcept;
#endif

        constexpr value_type&       operator[](length_type i) noexcept;
        constexpr value_type const& operator[](length_type i) const noexcept;

        constexpr value_type top() const noexcept;
        constexpr value_type bottom() const noexcept;
        constexpr value_type left() const noexcept;
        constexpr value_type right() const noexcept;

        template<typename S>
        constexpr auto&& position(this S&& self) noexcept;
        template<typename S>
        constexpr auto&& size(this S&& self) noexcept;

        constexpr bool operator==(basic_rect const& rhs) const noexcept;
        constexpr bool operator!=(basic_rect const& rhs) const noexcept = default;

        constexpr bool contains(point_type const& point) const noexcept;
        constexpr bool contains(basic_rect const& rhs) const noexcept;

        constexpr bool intersects(point_type const& point) const noexcept;
        constexpr bool intersects(basic_rect const& rhs) const noexcept;

        constexpr basic_rect operator|(basic_rect const& rhs) const noexcept;
        constexpr basic_rect operator&(basic_rect const& rhs) const noexcept;

        constexpr basic_rect& operator|=(basic_rect const& rhs) noexcept;
        constexpr basic_rect& operator&=(basic_rect const& rhs) noexcept;

        constexpr basic_rect operator+(point_type const& rhs) const noexcept;
        constexpr basic_rect operator+(size_type const& rhs) const noexcept;
        constexpr basic_rect operator-(point_type const& rhs) const noexcept;
        constexpr basic_rect operator-(size_type const& rhs) const noexcept;

        constexpr basic_rect& operator+=(point_type const& rhs) noexcept;
        constexpr basic_rect& operator+=(size_type const& rhs) noexcept;
        constexpr basic_rect& operator-=(point_type const& rhs) noexcept;
        constexpr basic_rect& operator-=(size_type const& rhs) noexcept;

        template<size_t I, typename S>
        constexpr auto&& get(this S&& self) noexcept;
    };

    using rect  = basic_rect<float>;
    using irect = basic_rect<int>;
} // namespace ljh

namespace std
{
    template<typename T>
    struct tuple_size<ljh::basic_rect<T>> : integral_constant<size_t, 2>
    {};

    template<typename T>
    struct tuple_element<0, ljh::basic_rect<T>>
    {
        using type = typename ljh::basic_rect<T>::point_type;
    };

    template<typename T>
    struct tuple_element<1, ljh::basic_rect<T>>
    {
        using type = typename ljh::basic_rect<T>::size_type;
    };

    template<typename T, typename C>
    struct formatter<ljh::basic_rect<T>, C>
    {
        template<typename PC>
        constexpr PC::iterator parse(PC& ctx)
        {
            return ctx.begin();
        }

        template<typename FC>
        FC::iterator format(ljh::basic_rect<T> const& value, FC& ctx) const
        {
#if !defined(LJH_COMPILER_MSVC) // Currently ICE on MSVC
            auto&& [pos, size] = value;
#else
            auto&& pos  = value.get<0>();
            auto&& size = value.get<1>();
#endif
            return std::format_to(ctx.out(), "{{{} {}}}", pos, size);
        }
    };
} // namespace std

namespace ljh
{
    template<typename T>
    inline constexpr basic_rect<T>::basic_rect(T x, T y, T w, T h) noexcept
        : x(x)
        , y(y)
        , w(w)
        , h(h)
    {}

    template<typename T>
    inline constexpr basic_rect<T>::basic_rect(basic_point<T> const& pos, basic_size<T> const& size) noexcept
        : x(pos.x)
        , y(pos.y)
        , w(size.w)
        , h(size.h)
    {}

    template<typename T>
    template<typename X, typename Y, typename W, typename H>
    inline constexpr basic_rect<T>::basic_rect(X x, Y y, W w, H h) noexcept
        : x(static_cast<value_type>(x))
        , y(static_cast<value_type>(y))
        , w(static_cast<value_type>(w))
        , h(static_cast<value_type>(h))
    {}

    template<typename T>
    template<typename P, typename S>
    inline constexpr basic_rect<T>::basic_rect(basic_point<P> const& pos, basic_size<S> const& size) noexcept
        : x(static_cast<value_type>(pos.x))
        , y(static_cast<value_type>(pos.y))
        , w(static_cast<value_type>(size.w))
        , h(static_cast<value_type>(size.h))
    {}

    template<typename T>
    template<typename P1, typename P2>
    inline constexpr basic_rect<T>::basic_rect(basic_point<P1> const& top_left, basic_point<P2> const& bottom_right) noexcept
        : x(static_cast<value_type>(top_left.x))
        , y(static_cast<value_type>(top_left.y))
        , w(static_cast<value_type>(bottom_right.x - top_left.x))
        , h(static_cast<value_type>(bottom_right.y - top_left.y))
    {}

    template<typename T>
    template<typename U>
    inline constexpr basic_rect<T>::operator basic_point<U>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    template<typename T>
    template<typename U>
    inline constexpr basic_rect<T>::operator basic_size<U>() const noexcept
    {
        return {static_cast<U>(w), static_cast<U>(h)};
    }

    template<typename T>
    template<typename U>
    inline constexpr basic_rect<T>::basic_rect(basic_rect<U> const& v) noexcept
        : x(static_cast<value_type>(v.x))
        , y(static_cast<value_type>(v.y))
        , w(static_cast<value_type>(v.w))
        , h(static_cast<value_type>(v.h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr basic_rect<T>& basic_rect<T>::operator=(basic_rect<U> const& v) noexcept
    {
        x = static_cast<value_type>(v.x);
        y = static_cast<value_type>(v.y);
        w = static_cast<value_type>(v.w);
        h = static_cast<value_type>(v.h);
        return *this;
    }

#ifdef GLM_VERSION
    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr basic_rect<T>::basic_rect(glm::vec<4, U, P> const& pos_size) noexcept
        : x(static_cast<value_type>(pos_size.x))
        , y(static_cast<value_type>(pos_size.y))
        , w(static_cast<value_type>(pos_size.z))
        , h(static_cast<value_type>(pos_size.w))
    {}

    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr basic_rect<T>::operator glm::vec<4, U, P>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(w), static_cast<U>(h)};
    }
#endif

    template<typename T>
    inline constexpr typename basic_rect<T>::value_type& basic_rect<T>::operator[](typename basic_rect<T>::length_type i) noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return w;
        case 3: return h;
        }
    }

    template<typename T>
    inline constexpr typename basic_rect<T>::value_type const& basic_rect<T>::operator[](typename basic_rect<T>::length_type i) const noexcept
    {
        assert(i >= 0 && i < this->length());
        switch (i)
        {
        default:
        case 0: return x;
        case 1: return y;
        case 2: return w;
        case 3: return h;
        }
    }

    template<typename T>
    inline constexpr typename basic_rect<T>::value_type basic_rect<T>::top() const noexcept
    {
        return y;
    }

    template<typename T>
    inline constexpr typename basic_rect<T>::value_type basic_rect<T>::bottom() const noexcept
    {
        return y + h;
    }

    template<typename T>
    inline constexpr typename basic_rect<T>::value_type basic_rect<T>::left() const noexcept
    {
        return x;
    }

    template<typename T>
    inline constexpr typename basic_rect<T>::value_type basic_rect<T>::right() const noexcept
    {
        return x + w;
    }

    template<typename T>
    template<typename S>
    inline constexpr auto&& basic_rect<T>::position(this S&& self) noexcept
    {
        using type    = point_type;
        auto index    = std::addressof(self.x);
        using pointer = make_like_t<std::remove_reference_t<S>, type>*;
        auto output   = reinterpret_cast<pointer>(index);
        using like    = make_like_t<S, type>;
        return static_cast<like>(*output);
    }

    template<typename T>
    template<typename S>
    inline constexpr auto&& basic_rect<T>::size(this S&& self) noexcept
    {
        using type    = size_type;
        auto index    = std::addressof(self.w);
        using pointer = make_like_t<std::remove_reference_t<S>, type>*;
        auto output   = reinterpret_cast<pointer>(index);
        using like    = make_like_t<S, type>;
        return static_cast<like>(*output);
    }

    template<typename T>
    inline constexpr bool basic_rect<T>::operator==(basic_rect<T> const& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h;
    }

    template<typename T>
    inline constexpr bool basic_rect<T>::contains(basic_rect<T>::point_type const& point) const noexcept
    {
        return left() <= point.x && top() <= point.y && point.x <= right() && point.y <= bottom();
    }

    template<typename T>
    inline constexpr bool basic_rect<T>::contains(basic_rect<T> const& rhs) const noexcept
    {
        return left() <= rhs.left() && top() <= rhs.top() && rhs.right() <= right() && rhs.bottom() <= bottom();
    }

    template<typename T>
    inline constexpr bool basic_rect<T>::intersects(basic_rect<T> const& rhs) const noexcept
    {
        return ((left() < rhs.left()) ? (right() > rhs.left()) : (left() < rhs.right())) &&
               ((top() < rhs.top()) ? (bottom() > rhs.top()) : (top() < rhs.bottom()));
    }

    template<typename T>
    inline constexpr basic_rect<T> basic_rect<T>::operator|(basic_rect<T> const& rhs) const noexcept
    {
        auto const lhs_empty = *this == basic_rect{};
        auto const rhs_empty = rhs == basic_rect{};
        if (lhs_empty && rhs_empty)
            return {};
        if (lhs_empty)
            return rhs;
        if (rhs_empty)
            return *this;

        auto const l = std::min(left(), rhs.left());
        auto const t = std::min(top(), rhs.top());
        auto const r = std::max(right(), rhs.right());
        auto const b = std::max(bottom(), rhs.bottom());
        return basic_rect{l, t, r - l, b - t};
    }

    template<typename T>
    inline constexpr basic_rect<T> basic_rect<T>::operator&(basic_rect<T> const& rhs) const noexcept
    {
        auto const l = std::max(left(), rhs.left());
        auto const r = std::min(right(), rhs.right());
        if (l >= r)
            return {};

        auto const t = std::max(top(), rhs.top());
        auto const b = std::min(bottom(), rhs.bottom());
        if (t >= b)
            return {};

        return basic_rect{l, t, r - l, b - t};
    }

    template<typename T>
    inline constexpr basic_rect<T>& basic_rect<T>::operator|=(basic_rect<T> const& rhs) noexcept
    {
        *this = *this & rhs;
        return *this;
    }

    template<typename T>
    inline constexpr basic_rect<T>& basic_rect<T>::operator&=(basic_rect<T> const& rhs) noexcept
    {
        *this = *this & rhs;
        return *this;
    }

    template<typename T>
    inline constexpr basic_rect<T> basic_rect<T>::operator+(point_type const& rhs) const noexcept
    {
        return {x + rhs.x, y + rhs.y, w, h};
    }

    template<typename T>
    inline constexpr basic_rect<T> basic_rect<T>::operator+(size_type const& rhs) const noexcept
    {
        return {x, y, w + rhs.w, h + rhs.h};
    }

    template<typename T>
    inline constexpr basic_rect<T> basic_rect<T>::operator-(point_type const& rhs) const noexcept
    {
        return {x - rhs.x, y - rhs.y, w, h};
    }

    template<typename T>
    inline constexpr basic_rect<T> basic_rect<T>::operator-(size_type const& rhs) const noexcept
    {
        return {x, y, w - rhs.w, h - rhs.h};
    }

    template<typename T>
    inline constexpr basic_rect<T>& basic_rect<T>::operator+=(point_type const& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr basic_rect<T>& basic_rect<T>::operator+=(size_type const& rhs) noexcept
    {
        w += rhs.w;
        h += rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr basic_rect<T>& basic_rect<T>::operator-=(point_type const& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr basic_rect<T>& basic_rect<T>::operator-=(size_type const& rhs) noexcept
    {
        w -= rhs.w;
        h -= rhs.h;
        return *this;
    }

    template<typename T>
    template<size_t I, typename S>
    inline constexpr auto&& basic_rect<T>::get(this S&& self) noexcept
    {
        using type = std::tuple_element_t<I, basic_rect<T>>;
        if constexpr (std::same_as<type, point_type>)
            return self.position();
        if constexpr (std::same_as<type, size_type>)
            return self.size();
    }
} // namespace ljh

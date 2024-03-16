
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// rect.hpp - v1.0
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
#include <utility>
#include <algorithm>

#if __has_include(<glm/fwd.hpp>)
#include <glm/fwd.hpp>
#endif

namespace ljh
{
    template<typename T>
    struct tpoint;

    template<typename T>
    struct tsize;

    template<typename T>
    struct trect
    {
        using value_type  = T;
        using length_type = std::size_t;
        using point_type  = tpoint<T>;
        using size_type   = tsize<T>;

        value_type x = 0;
        value_type y = 0;
        value_type w = 0;
        value_type h = 0;

        static constexpr length_type length() noexcept
        {
            return 4;
        }

        constexpr trect() noexcept               = default;
        constexpr trect(trect const& v) noexcept = default;
        constexpr trect(trect&& v) noexcept      = default;

        template<typename X = value_type, typename Y = value_type, typename W = value_type, typename H = value_type>
        constexpr trect(X x, Y y, W w, H h) noexcept;
        template<typename P = value_type, typename S = value_type>
        constexpr trect(tpoint<P> const& pos, tsize<S> const& size) noexcept;

        constexpr trect& operator=(trect const& v) noexcept = default;
        constexpr trect& operator=(trect&& v) noexcept      = default;

        constexpr ~trect() noexcept = default;

        template<typename U>
        constexpr explicit operator tpoint<U>() const noexcept;
        template<typename U>
        constexpr explicit operator tsize<U>() const noexcept;

        template<typename U>
        constexpr explicit trect(trect<U> const& v) noexcept;
        template<typename U>
        constexpr trect& operator=(trect<U> const& v) noexcept;

#ifdef GLM_VERSION
        template<typename U, glm::qualifier P>
        constexpr explicit trect(glm::vec<4, U, P> const& pos_size) noexcept;
        template<typename U, glm::qualifier P>
        constexpr explicit operator glm::vec<4, U, P>() const noexcept;
#endif

        constexpr value_type&       operator[](length_type i) noexcept;
        constexpr value_type const& operator[](length_type i) const noexcept;

        constexpr value_type top() const noexcept;
        constexpr value_type bottom() const noexcept;
        constexpr value_type left() const noexcept;
        constexpr value_type right() const noexcept;

        constexpr point_type position() const noexcept;
        constexpr size_type  size() const noexcept;

        constexpr bool operator==(trect const& rhs) const noexcept;
        constexpr bool operator!=(trect const& rhs) const noexcept = default;

        constexpr bool contains(point_type const& point) const noexcept;
        constexpr bool contains(trect const& rhs) const noexcept;

        constexpr trect operator|(trect const& rhs) const noexcept;
        constexpr trect operator&(trect const& rhs) const noexcept;

        constexpr trect& operator|=(trect const& rhs) noexcept;
        constexpr trect& operator&=(trect const& rhs) noexcept;

        constexpr trect operator+(point_type const& rhs) const noexcept;
        constexpr trect operator+(size_type const& rhs) const noexcept;
        constexpr trect operator-(point_type const& rhs) const noexcept;
        constexpr trect operator-(size_type const& rhs) const noexcept;

        constexpr trect& operator+=(point_type const& rhs) noexcept;
        constexpr trect& operator+=(size_type const& rhs) noexcept;
        constexpr trect& operator-=(point_type const& rhs) noexcept;
        constexpr trect& operator-=(size_type const& rhs) noexcept;
    };

    using rect  = trect<float>;
    using irect = trect<int>;

    template<typename T>
    template<typename X, typename Y, typename W, typename H>
    inline constexpr trect<T>::trect(X x, Y y, W w, H h) noexcept
        : x(static_cast<value_type>(x))
        , y(static_cast<value_type>(y))
        , w(static_cast<value_type>(w))
        , h(static_cast<value_type>(h))
    {}

    template<typename T>
    template<typename P, typename S>
    inline constexpr trect<T>::trect(tpoint<P> const& pos, tsize<S> const& size) noexcept
        : x(static_cast<value_type>(pos.x))
        , y(static_cast<value_type>(pos.y))
        , w(static_cast<value_type>(size.w))
        , h(static_cast<value_type>(size.h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr trect<T>::operator tpoint<U>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    template<typename T>
    template<typename U>
    inline constexpr trect<T>::operator tsize<U>() const noexcept
    {
        return {static_cast<U>(w), static_cast<U>(h)};
    }

    template<typename T>
    template<typename U>
    inline constexpr trect<T>::trect(trect<U> const& v) noexcept
        : x(static_cast<value_type>(v.x))
        , y(static_cast<value_type>(v.y))
        , w(static_cast<value_type>(v.w))
        , h(static_cast<value_type>(v.h))
    {}

    template<typename T>
    template<typename U>
    inline constexpr trect<T>& trect<T>::operator=(trect<U> const& v) noexcept
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
    inline constexpr trect<T>::trect(glm::vec<4, U, P> const& pos_size) noexcept
        : x(static_cast<value_type>(pos_size.x))
        , y(static_cast<value_type>(pos_size.y))
        , w(static_cast<value_type>(pos_size.z))
        , h(static_cast<value_type>(pos_size.w))
    {}

    template<typename T>
    template<typename U, glm::qualifier P>
    inline constexpr trect<T>::operator glm::vec<4, U, P>() const noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(w), static_cast<U>(h)};
    }
#endif

    template<typename T>
    inline constexpr typename trect<T>::value_type& trect<T>::operator[](typename trect<T>::length_type i) noexcept
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
    inline constexpr typename trect<T>::value_type const& trect<T>::operator[](typename trect<T>::length_type i) const noexcept
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
    inline constexpr typename trect<T>::value_type trect<T>::top() const noexcept
    {
        return y;
    }

    template<typename T>
    inline constexpr typename trect<T>::value_type trect<T>::bottom() const noexcept
    {
        return y + h;
    }

    template<typename T>
    inline constexpr typename trect<T>::value_type trect<T>::left() const noexcept
    {
        return x;
    }

    template<typename T>
    inline constexpr typename trect<T>::value_type trect<T>::right() const noexcept
    {
        return x + w;
    }

    template<typename T>
    inline constexpr typename trect<T>::point_type trect<T>::position() const noexcept
    {
        return trect<T>::point_type{*this};
    }

    template<typename T>
    inline constexpr typename trect<T>::size_type trect<T>::size() const noexcept
    {
        return trect<T>::size_type{*this};
    }

    template<typename T>
    inline constexpr bool trect<T>::operator==(trect<T> const& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h;
    }

    template<typename T>
    inline constexpr bool trect<T>::contains(trect<T>::point_type const& point) const noexcept
    {
        return left() <= point.x && top() <= point.y && point.x <= right() && point.y <= bottom();
    }

    template<typename T>
    inline constexpr bool trect<T>::contains(trect<T> const& rhs) const noexcept
    {
        return left() <= rhs.left() && top() <= rhs.top() && rhs.right() <= right() && rhs.bottom() <= bottom();
    }

    template<typename T>
    inline constexpr trect<T> trect<T>::operator|(trect<T> const& rhs) const noexcept
    {
        auto const lhs_empty = *this == trect{};
        auto const rhs_empty = rhs == trect{};
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
        return trect{l, t, r - l, b - t};
    }

    template<typename T>
    inline constexpr trect<T> trect<T>::operator&(trect<T> const& rhs) const noexcept
    {
        auto const l = std::max(left(), rhs.left());
        auto const r = std::min(right(), rhs.right());
        if (l >= r)
            return {};

        auto const t = std::max(top(), rhs.top());
        auto const b = std::min(bottom(), rhs.bottom());
        if (t >= b)
            return {};

        return trect{l, t, r - l, b - t};
    }

    template<typename T>
    inline constexpr trect<T>& trect<T>::operator|=(trect<T> const& rhs) noexcept
    {
        *this = *this & rhs;
        return *this;
    }

    template<typename T>
    inline constexpr trect<T>& trect<T>::operator&=(trect<T> const& rhs) noexcept
    {
        *this = *this & rhs;
        return *this;
    }

    template<typename T>
    inline constexpr trect<T> trect<T>::operator+(point_type const& rhs) const noexcept
    {
        return {x + rhs.x, y + rhs.y, w, h};
    }

    template<typename T>
    inline constexpr trect<T> trect<T>::operator+(size_type const& rhs) const noexcept
    {
        return {x, y, w + rhs.w, h + rhs.h};
    }

    template<typename T>
    inline constexpr trect<T> trect<T>::operator-(point_type const& rhs) const noexcept
    {
        return {x - rhs.x, y - rhs.y, w, h};
    }

    template<typename T>
    inline constexpr trect<T> trect<T>::operator-(size_type const& rhs) const noexcept
    {
        return {x, y, w - rhs.w, h - rhs.h};
    }

    template<typename T>
    inline constexpr trect<T>& trect<T>::operator+=(point_type const& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr trect<T>& trect<T>::operator+=(size_type const& rhs) noexcept
    {
        w += rhs.w;
        h += rhs.h;
        return *this;
    }

    template<typename T>
    inline constexpr trect<T>& trect<T>::operator-=(point_type const& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    template<typename T>
    inline constexpr trect<T>& trect<T>::operator-=(size_type const& rhs) noexcept
    {
        w -= rhs.w;
        h -= rhs.h;
        return *this;
    }
} // namespace ljh

namespace std
{
    template<typename T>
    struct tuple_size<ljh::trect<T>> : integral_constant<size_t, 2>
    {};

    template<typename T>
    struct tuple_element<0, ljh::trect<T>>
    {
        using type = typename ljh::trect<T>::point_type;
    };

    template<typename T>
    struct tuple_element<1, ljh::trect<T>>
    {
        using type = typename ljh::trect<T>::size_type;
    };
} // namespace std

namespace ljh
{
    template<std::size_t I, typename T>
    std::tuple_element_t<I, trect<T>> get(trect<T> const& item)
    {
        return static_cast<std::tuple_element_t<I, trect<T>>>(item);
    }
} // namespace ljh
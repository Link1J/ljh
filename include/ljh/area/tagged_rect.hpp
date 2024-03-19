
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// region.hpp - v1.0
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

#include "rect.hpp"

namespace ljh
{
    template<typename T, typename V>
    struct basic_tagged_rect : public basic_rect<T>
    {
        using base       = basic_rect<T>;
        using value_type = base::value_type;
        using point_type = base::point_type;
        using size_type  = base::size_type;
        using tag_type   = V;

        tag_type tag;

        constexpr basic_tagged_rect() noexcept                           = default;
        constexpr basic_tagged_rect(basic_tagged_rect const& v) noexcept = default;
        constexpr basic_tagged_rect(basic_tagged_rect&& v) noexcept      = default;

        constexpr basic_tagged_rect(base rect, V tag) noexcept;
        constexpr basic_tagged_rect(T x, T y, T w, T h, V tag) noexcept;
        constexpr basic_tagged_rect(basic_point<T> const& pos, basic_size<T> const& size, V tag) noexcept;

        template<typename X = value_type, typename Y = value_type, typename W = value_type, typename H = value_type>
        constexpr basic_tagged_rect(X x, Y y, W w, H h, V tag) noexcept;
        template<typename P = value_type, typename S = value_type>
        constexpr basic_tagged_rect(basic_point<P> const& pos, basic_size<S> const& size, V tag) noexcept;
        template<typename P1, typename P2>
        constexpr basic_tagged_rect(basic_point<P1> const& top_left, basic_point<P2> const& bottom_right, V tag) noexcept;

        constexpr basic_tagged_rect& operator=(basic_tagged_rect const& v) noexcept = default;
        constexpr basic_tagged_rect& operator=(basic_tagged_rect&& v) noexcept      = default;

        constexpr ~basic_tagged_rect() noexcept = default;

        constexpr bool operator==(basic_tagged_rect const& rhs) const noexcept;
        constexpr bool operator!=(basic_tagged_rect const& rhs) const noexcept = default;

        using base::operator point_type;
        using base::operator size_type;

        template<size_t I, typename S>
        constexpr auto&& get(this S&& self) noexcept;
    };
} // namespace ljh

namespace std
{
    template<typename T, typename V>
    struct tuple_size<ljh::basic_tagged_rect<T, V>> : integral_constant<size_t, tuple_size_v<ljh::basic_rect<T>> + 1>
    {};

    template<size_t I, typename T, typename V>
        requires(I < tuple_size_v<ljh::basic_rect<T>>)
    struct tuple_element<I, ljh::basic_tagged_rect<T, V>>
    {
        using type = tuple_element_t<I, ljh::basic_rect<T>>;
    };

    template<size_t I, typename T, typename V>
        requires(I == tuple_size_v<ljh::basic_rect<T>>)
    struct tuple_element<I, ljh::basic_tagged_rect<T, V>>
    {
        using type = V;
    };

    template<typename T, typename V, typename C>
    struct formatter<ljh::basic_tagged_rect<T, V>, C>
    {
        template<typename PC>
        constexpr PC::iterator parse(PC& ctx)
        {
            return ctx.begin();
        }

        template<typename FC>
        FC::iterator format(ljh::basic_tagged_rect<T, V> const& value, FC& ctx) const
        {
            auto&& [pos, size, tag] = value;
            return std::format_to(ctx.out(), "{{{} {} | {}}}", pos, size, tag);
        }
    };
} // namespace std

namespace ljh
{
    template<typename T, typename V>
    inline constexpr basic_tagged_rect<T, V>::basic_tagged_rect(base rect, V tag) noexcept
        : base(rect)
        , tag(tag)
    {}

    template<typename T, typename V>
    inline constexpr basic_tagged_rect<T, V>::basic_tagged_rect(T x, T y, T w, T h, V tag) noexcept
        : basic_tagged_rect({x, y, w, h}, tag)
    {}

    template<typename T, typename V>
    inline constexpr basic_tagged_rect<T, V>::basic_tagged_rect(basic_point<T> const& pos, basic_size<T> const& size, V tag) noexcept
        : basic_tagged_rect({pos, size}, tag)
    {}

    template<typename T, typename V>
    template<typename X, typename Y, typename W, typename H>
    inline constexpr basic_tagged_rect<T, V>::basic_tagged_rect(X x, Y y, W w, H h, V tag) noexcept
        : basic_tagged_rect({x, y, w, h}, tag)
    {}

    template<typename T, typename V>
    template<typename P, typename S>
    inline constexpr basic_tagged_rect<T, V>::basic_tagged_rect(basic_point<P> const& pos, basic_size<S> const& size, V tag) noexcept
        : basic_tagged_rect({pos, size}, tag)
    {}

    template<typename T, typename V>
    template<typename P1, typename P2>
    inline constexpr basic_tagged_rect<T, V>::basic_tagged_rect(basic_point<P1> const& top_left, basic_point<P2> const& bottom_right, V tag) noexcept
        : basic_tagged_rect({top_left, bottom_right}, tag)
    {}

    template<typename T, typename V>
    inline constexpr bool basic_tagged_rect<T, V>::operator==(basic_tagged_rect const& rhs) const noexcept
    {
        return base::operator==(rhs) && (tag == rhs.tag);
    }

    template<typename T, typename V>
    template<size_t I, typename S>
    inline constexpr auto&& basic_tagged_rect<T, V>::get(this S&& self) noexcept
    {
        using type = std::tuple_element_t<I, basic_tagged_rect<T, V>>;
        if constexpr (std::same_as<type, V>)
            return std::forward<S>(self).tag;
        else
            return std::forward<S>(self).base::template get<I>();
    }
} // namespace ljh

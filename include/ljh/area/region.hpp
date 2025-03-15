
//          Copyright Jared Irwin 2024-2025
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

#pragma once

#include "rect.hpp"
#include "tagged_rect.hpp"

#include <initializer_list>
#include <concepts>
#include <vector>
#include <array>

namespace ljh::__
{
    template<typename T, typename V>
        requires(std::is_void_v<V> || std::copyable<V>)
    using rect_type = std::conditional_t<std::is_void_v<V>, basic_rect<T>, basic_tagged_rect<T, V>>;
} // namespace ljh::__

namespace ljh
{
    template<typename T, typename V = void>
    struct basic_region
    {
    private:
        using container = std::vector<__::rect_type<T, V>>;

    public:
        using rect_type  = basic_rect<T>;
        using point_type = typename rect_type::point_type;

        using value_type      = typename container::value_type;
        using reference       = typename container::reference;
        using const_reference = typename container::const_reference;
        using iterator        = typename container::iterator;
        using const_iterator  = typename container::const_iterator;
        using difference_type = typename container::difference_type;
        using size_type       = typename container::size_type;

        constexpr basic_region() noexcept                 = default;
        constexpr basic_region(basic_region const& v)     = default;
        constexpr basic_region(basic_region&& v) noexcept = default;

        constexpr basic_region(std::initializer_list<value_type> list);

        constexpr basic_region& operator=(basic_region const& v) noexcept = default;
        constexpr basic_region& operator=(basic_region&& v) noexcept      = default;

        constexpr ~basic_region() noexcept = default;

        constexpr iterator       begin() noexcept;
        constexpr iterator       end() noexcept;
        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator end() const noexcept;
        constexpr const_iterator cbegin() const noexcept;
        constexpr const_iterator cend() const noexcept;

        constexpr void add(value_type const& rhs);
        constexpr void add(basic_region const& rhs);
        constexpr void subtract(rect_type const& rhs);
        constexpr void subtract(basic_region<T> const& rhs);

        constexpr basic_region& operator+=(value_type const& rhs);
        constexpr basic_region& operator+=(basic_region const& rhs);
        constexpr basic_region& operator-=(rect_type const& rhs);
        constexpr basic_region& operator-=(basic_region<T> const& rhs);

        constexpr bool operator==(basic_region const& rhs) const noexcept = default;
        constexpr bool operator!=(basic_region const& rhs) const noexcept = default;

    private:
        template<typename T1, typename C>
        friend struct std::formatter;

        constexpr void merge() noexcept;
        constexpr void merge_rects(reference rect, iterator& it) noexcept;

        static constexpr value_type build_rect(point_type top_left, point_type bottom_right, const_reference base) noexcept;

        container list;
    };

    using region  = basic_region<float>;
    using iregion = basic_region<int>;

    template<typename V>
    using tagged_region = basic_region<float, V>;
    template<typename V>
    using tagged_iregion = basic_region<int, V>;

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::basic_region(std::initializer_list<value_type> list)
        : list(list)
    {
        merge();
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::iterator basic_region<T, V>::begin() noexcept
    {
        return list.begin();
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::iterator basic_region<T, V>::end() noexcept
    {
        return list.end();
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::const_iterator basic_region<T, V>::begin() const noexcept
    {
        return list.begin();
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::const_iterator basic_region<T, V>::end() const noexcept
    {
        return list.end();
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::const_iterator basic_region<T, V>::cbegin() const noexcept
    {
        return list.cbegin();
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::const_iterator basic_region<T, V>::cend() const noexcept
    {
        return list.cend();
    }

    template<typename T, typename V>
    inline constexpr void basic_region<T, V>::add(value_type const& rhs)
    {
        list.push_back(rhs);
        merge();
    }

    template<typename T, typename V>
    inline constexpr void basic_region<T, V>::subtract(rect_type const& rhs)
    {
        for (auto it = std::begin(list); it != std::end(list); it++)
        {
            if (rhs.intersects(*it))
            {
                std::array<value_type, 4> new_items;
                auto                      end   = std::begin(new_items);
                auto&                     exist = *it;

                if (exist.top() < rhs.top())
                {
                    *end++ = build_rect({exist.left(), exist.top()}, {exist.right(), rhs.top()}, exist);
                    exist.h -= rhs.top() - exist.top();
                    exist.y = rhs.top();
                }
                if (exist.bottom() > rhs.bottom())
                {
                    *end++ = build_rect({exist.left(), rhs.bottom()}, {exist.right(), exist.bottom()}, exist);
                    exist.h -= exist.bottom() - rhs.bottom();
                }
                if (exist.left() < rhs.left())
                {
                    *end++ = build_rect({exist.left(), exist.top()}, {rhs.left(), exist.bottom()}, exist);
                    exist.w -= rhs.left() - exist.left();
                    exist.x = rhs.left();
                }
                if (exist.right() > rhs.right())
                {
                    *end++ = build_rect({rhs.right(), exist.top()}, {exist.right(), exist.bottom()}, exist);
                    exist.w -= exist.right() - rhs.right();
                }
                it = list.insert(list.erase(it), std::begin(new_items), end);
            }
        }
        merge();
    }

    template<typename T, typename V>
    inline constexpr void basic_region<T, V>::add(basic_region const& rhs)
    {
        for (auto&& rect : rhs)
            add(rect);
    }

    template<typename T, typename V>
    inline constexpr void basic_region<T, V>::subtract(basic_region<T> const& rhs)
    {
        for (auto&& rect : rhs)
            subtract(rect);
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>& basic_region<T, V>::operator+=(value_type const& rhs)
    {
        add(rhs);
        return *this;
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>& basic_region<T, V>::operator+=(basic_region const& rhs)
    {
        add(rhs);
        return *this;
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>& basic_region<T, V>::operator-=(rect_type const& rhs)
    {
        subtract(rhs);
        return *this;
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>& basic_region<T, V>::operator-=(basic_region<T> const& rhs)
    {
        subtract(rhs);
        return *this;
    }

    template<typename T, typename V>
    inline constexpr void basic_region<T, V>::merge() noexcept
    {
        for (int a = 0; a < 2; a++)
        {
            for (auto it1 = std::begin(list); it1 != std::end(list); it1++)
            {
                auto it2 = std::next(it1);
                while (it2 != std::end(list))
                {
                    auto&       lhs = *it1;
                    auto const& rhs = *it2;

                    bool tag_check = true;
                    if constexpr (std::same_as<value_type, basic_tagged_rect<T, V>>)
                        tag_check = lhs.tag == rhs.tag;

                    if (tag_check)
                    {
                        if ((lhs.right() == rhs.left() || lhs.left() == rhs.right()) && (lhs.y == rhs.y && lhs.h == rhs.h))
                        {
                            lhs.x = std::min(lhs.x, rhs.x);
                            lhs.w = lhs.w + rhs.w;
                            it2   = --list.erase(it2);
                            continue;
                        }
                        if ((lhs.top() == rhs.bottom() || lhs.bottom() == rhs.top()) && (lhs.x == rhs.x && lhs.w == rhs.w))
                        {
                            lhs.y = std::min(lhs.y, rhs.y);
                            lhs.h = lhs.h + rhs.h;
                            it2   = --list.erase(it2);
                            continue;
                        }
                    }

                    it2++;
                }
            }
        }
    }

    template<typename T, typename V>
    inline constexpr basic_region<T, V>::value_type basic_region<T, V>::build_rect(point_type top_left, point_type bottom_right, const_reference base) noexcept
    {
        if constexpr (std::same_as<value_type, basic_tagged_rect<T, V>>)
            return {top_left, bottom_right, base.tag};
        else
            return {top_left, bottom_right};
    }

    template<typename T, typename V>
    inline constexpr void basic_region<T, V>::merge_rects(reference rect, iterator& it) noexcept
    {}
} // namespace ljh

template<typename T, typename V, typename C>
struct std::formatter<ljh::basic_region<T, V>, C>
{
    template<typename PC>
    constexpr PC::iterator parse(PC& ctx)
    {
        return ctx.begin();
    }

    template<typename FC>
    FC::iterator format(ljh::basic_region<T, V> const& value, FC& ctx) const
    {
        bool not_first = false;
        ctx.advance_to(std::format_to(ctx.out(), "["));
        for (auto&& rect : value.list)
        {
            if (std::exchange(not_first, true))
                ctx.advance_to(std::format_to(ctx.out(), ","));
            ctx.advance_to(std::format_to(ctx.out(), "{}", rect));
        }
        ctx.advance_to(std::format_to(ctx.out(), "]"));
        return ctx.out();
    }
};

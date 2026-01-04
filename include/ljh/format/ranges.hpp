
//          Copyright Jared Irwin 2025-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// ranges.hpp - v1.0
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
#include "../cpp_version.hpp"
#include <format>
#include <ranges>

namespace ljh::fmt
{
    LJH_MODULE_MAIN_EXPORT template<typename It, typename Sent, typename Sep>
    struct join_view
    {
        It   _begin;
        Sent _end;
        Sep  _sep;
        bool _sep_before_first;

        auto begin()
        {
            return _begin;
        }

        auto end()
        {
            return _end;
        }
    };

    template<typename It, typename Sentinel, typename Sep,
             typename TSep = std::conditional_t<std::convertible_to<Sep, std::string_view>, std::string_view, char>>
        requires(std::same_as<Sep, char> || std::convertible_to<Sep, std::string_view>)
    inline auto join(It begin, Sentinel end, Sep sep, bool sep_before_first = false) -> join_view<It, Sentinel, TSep>
    {
        return {begin, end, sep, sep_before_first};
    }

    template<std::ranges::input_range Range, typename Sep>
        requires(std::same_as<Sep, char> || std::convertible_to<Sep, std::string_view>)
    inline auto join(Range&& r, Sep sep, bool sep_before_first = false)
    {
        return join(std::ranges::begin(r), std::ranges::end(r), sep, sep_before_first);
    }
} // namespace ljh::fmt

LJH_MODULE_MAIN_EXPORT template<typename It, typename Sent, typename Sep>
struct std::formatter<ljh::fmt::join_view<It, Sent, Sep>, char>
{
    using value_type = std::iter_value_t<It>;
    std::formatter<remove_cvref_t<value_type>, char> value_formatter;

    template<typename PC>
    constexpr PC::iterator parse(PC& ctx)
    {
        return value_formatter.parse(ctx);
    }

    template<typename FC>
    FC::iterator format(ljh::fmt::join_view<It, Sent, Sep>& value, FC& ctx) const
    {
        using iter = std::conditional_t<std::is_copy_constructible_v<It>, It, It&>;
        iter it    = value.begin();
        auto out   = ctx.out();
        if (it == value.end())
            return out;
        if (value._sep_before_first)
        {
            if constexpr (std::same_as<Sep, char>)
                *out++ = value._sep;
            else
                out = std::ranges::copy(value._sep, out).out;

            ctx.advance_to(out);
        }

        out = value_formatter.format(*it, ctx);
        ++it;
        while (it != value.end())
        {
            if constexpr (std::same_as<Sep, char>)
                *out++ = value._sep;
            else
                out = std::ranges::copy(value._sep, out).out;

            ctx.advance_to(out);
            out = value_formatter.format(*it, ctx);
            ++it;
        }
        return out;
    }
};
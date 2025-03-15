
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// views.hpp - v1.0
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
#include "range_adaptor_closure.hpp"

namespace ljh
{
    template<typename T>
    inline constexpr auto constructor = []() consteval {
        return []<typename... Args>(Args&&... args)
            requires std::constructible_from<T, Args...>
        { return T(std::forward<Args>(args)...); };
    }();
}

#if __has_include(<ranges>) && __cpp_lib_ranges
namespace ljh::ranges::views
{
    template<typename T>
    inline constexpr auto transform_into = std::ranges::views::transform(constructor<T>);

    inline constexpr adaptor transform_tuple = []<std::ranges::viewable_range R, typename F>(R&& r, F&& pred) {
        return std::ranges::views::transform(std::forward<R>(r),
                                             [pred = std::forward<F>(pred)]<typename T>(T&& t) { return std::apply(pred, std::forward<T>(t)); });
    };

#if __cpp_lib_ranges_zip >= 202110L
    namespace __zip
    {
        template<std::ranges::viewable_range... R>
        struct closure : range_adaptor_closure<closure<R...>>
        {
            std::tuple<R...> range;

            closure(R&&... ranges)
                : range(ranges...)
            {}

            template<std::ranges::viewable_range R2>
            constexpr auto operator()(R2&& r) &
            {
                return std::apply([&r]<typename... R3>(R3&&... o) { return std::ranges::zip_view(std::forward<R2>(r), std::forward<R3>(o)...); }, range);
            }

            template<std::ranges::viewable_range R2>
            constexpr auto operator()(R2&& r) const&
            {
                return std::apply([&r]<typename... R3>(R3&&... o) { return std::ranges::zip_view(std::forward<R2>(r), std::forward<R3>(o)...); }, range);
            }

            template<std::ranges::viewable_range R2>
            constexpr auto operator()(R2&& r) &&
            {
                return std::apply([&r]<typename... R3>(R3&&... o) { return std::ranges::zip_view(std::forward<R2>(r), std::forward<R3>(o)...); }, range);
            }

            template<std::ranges::viewable_range R2>
            constexpr auto operator()(R2&& r) const&&
            {
                return std::apply([&r]<typename... R3>(R3&&... o) { return std::ranges::zip_view(std::forward<R2>(r), std::forward<R3>(o)...); }, range);
            }
        };

        struct fn
        {
            template<std::ranges::viewable_range... R>
            constexpr auto operator()(R&&... r) const noexcept
            {
                return closure(std::forward<R>(r)...);
            }
        };
    } // namespace __zip

    inline constexpr auto zip = __zip::fn{};
#endif

#if __cpp_lib_ranges_fold >= 202207L
    inline constexpr adaptor fold_left = []<std::ranges::input_range R, typename T, typename F>(R&& r, T i, F f) {
        return std::ranges::fold_left(std::forward<R>(r), i, f);
    };
#endif
} // namespace ljh::ranges::views
#endif
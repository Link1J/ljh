
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// utility.hpp - v1.0
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

#if __cpp_lib_ranges_zip >= 202110L
namespace ljh::ranges::views
{
	namespace __zip
	{
		struct fn
		{
			template<ranges::viewable_range... R>
			constexpr auto operator()(R...&& r) const noexcept
			{
				struct closure : ranges::range_adaptor_closure<closure<F>>
				{
					std::tuple<R> range;

					closure(R&&... ranges)
						: range(ranges...)
					{}

					template<ranges::viewable_range R2>
					requires invocable<F&, R2>
					constexpr auto operator()(R2&& r) &
					{
						return std::apply([&r]<typename... R>(R&&... o) {
							return std::ranges::zip_view(std::forward<R2>(r), std::forward<R>(o)...);
						}, range);
					}

					template<ranges::viewable_range R>
					requires invocable<F const&, R>
					constexpr auto operator()(R&& r) const&
					{
						return std::apply([&r]<typename... R>(R&&... o) {
							return std::ranges::zip_view(std::forward<R2>(r), std::forward<R>(o)...);
						}, range);
					}
					
					template<ranges::viewable_range R>
					requires invocable<F&&, R>
					constexpr auto operator()(R&& r) &&
					{
						return std::apply([&r]<typename... R>(R&&... o) {
							return std::ranges::zip_view(std::forward<R2>(r), std::forward<R>(o)...);
						}, range);
					}
					
					template<ranges::viewable_range R>
					requires invocable<F const&&, R>
					constexpr auto operator()(R&& r) const&&
					{
						return std::apply([&r]<typename... R>(R&&... o) {
							return std::ranges::zip_view(std::forward<R2>(r), std::forward<R>(o)...);
						}, range);
					}
				};

				return 
			}
		};
	}
	inline constexpr adaptor zip = __zip::fn{};
}
#endif
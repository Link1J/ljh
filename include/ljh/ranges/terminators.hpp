
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// terminators.hpp - v1.0
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
#include "range_adaptor_closure.hpp"
#include <optional>

namespace ljh::ranges
{
	inline constexpr closure first = []<std::ranges::range R>(R&& range) -> std::optional<std::ranges::range_value_t<R>>
	{
		if (auto b = std::ranges::begin(range); b != std::ranges::end(range))
			return *b;
		return std::nullopt;
	};

	inline constexpr adaptor first_or = []<std::ranges::range R>(R&& range, std::ranges::range_value_t<R> default_value) -> std::ranges::range_value_t<R>
	{
		return (range | first).value_or(default_value);
	};

	inline constexpr closure last = []<std::ranges::range R>(R&& range) -> std::optional<std::ranges::range_value_t<R>>
	{
		if (auto b = std::ranges::rbegin(range); b != std::ranges::rend(range))
			return *b;
		return std::nullopt;
	};

	inline constexpr adaptor last_or = []<std::ranges::range R>(R&& range, std::ranges::range_value_t<R> default_value) -> std::ranges::range_value_t<R>
	{
		return (range | last).value_or(default_value);
	};
}

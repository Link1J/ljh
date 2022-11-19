
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

#include <optional>
#include <ranges>

namespace ljh::ranges
{
	struct last_t{};
	inline constexpr last_t last;

	template<std::ranges::range R>
	std::optional<std::ranges::range_value_t<R>> operator|(R&& range, last_t)
	{
		if (auto b = std::ranges::begin(range); b != std::ranges::end(range))
			return *b;
		return std::nullopt;
	}

	struct first_t{};
	inline constexpr first_t first;

	template<std::ranges::range R>
	std::optional<std::ranges::range_value_t<R>> operator|(R&& range, first_t)
	{
		if (auto b = std::ranges::end(range); b != std::ranges::begin(range))
			return *(b - 1);
		return std::nullopt;
	}
}
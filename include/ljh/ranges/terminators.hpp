
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
	template<std::ranges::range R>
	std::optional<std::ranges::range_value_t<R>> first(R&& range)
	{
		if (auto b = std::ranges::begin(range); b != std::ranges::end(range))
			return *b;
		return std::nullopt;
	}

	template<std::ranges::range R>
	std::ranges::range_value_t<R> first_or(R&& range, std::ranges::range_value_t<R> default_value)
	{
		return first(range).value_or(default_value);
	}
	
	template<std::ranges::range R>
	std::optional<std::ranges::range_value_t<R>> last(R&& range)
	{
		if (auto b = std::ranges::rbegin(range); b != std::ranges::rend(range))
			return *b;
		return std::nullopt;
	}
	
	template <std::ranges::range R>
	std::ranges::range_value_t<R> last_or(R&& range, std::ranges::range_value_t<R> default_value)
	{
		return last(range).value_or(default_value);
	}
}

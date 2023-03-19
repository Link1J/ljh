
//          Copyright Jared Irwin 2020-2023
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
#include <span>

#if __has_include(<ranges>) && __cpp_lib_ranges
namespace ljh::ranges
{
	inline constexpr closure first = []<std::ranges::range R>(R&& range) -> std::optional<std::ranges::range_value_t<R>>
	{
		if (auto b = std::ranges::begin(range); b != std::ranges::end(range))
			return *b;
		return std::nullopt;
	};

	inline constexpr adaptor first_or = []<std::ranges::range R>(R&& range, std::ranges::range_value_t<R> default_value) -> std::optional<std::ranges::range_value_t<R>>
	{
		return first(range).value_or(default_value);
	};

	inline constexpr closure last = []<std::ranges::range R>(R&& range) -> std::optional<std::ranges::range_value_t<R>>
	{
		if (auto b = std::ranges::rbegin(range); b != std::ranges::rend(range))
			return *b;
		return std::nullopt;
	};

	inline constexpr adaptor last_or = []<std::ranges::range R>(R&& range, std::ranges::range_value_t<R> default_value) -> std::optional<std::ranges::range_value_t<R>>
	{
		return last(range).value_or(default_value);
	};

	template<size_t offset, size_t count = std::dynamic_extent>
	inline constexpr closure subspan_const = []<std::ranges::contiguous_range R>(R&& r)
	{
		return std::span{std::forward<R>(r)}.template subspan<offset, count>();
	};

	inline constexpr adaptor subspan = []<std::ranges::contiguous_range R>(R&& r, size_t offset, size_t count = std::dynamic_extent)
	{
		return std::span{std::forward<R>(r)}.subspan(offset, count);
	};

	template<size_t count>
	inline constexpr closure first_n_const = []<std::ranges::contiguous_range R>(R&& r)
	{
		return std::span{std::forward<R>(r)}.template first<count>();
	};

	inline constexpr adaptor first_n = []<std::ranges::contiguous_range R>(R&& r, size_t count)
	{
		return std::span{std::forward<R>(r)}.first(count);
	};

	template<size_t count>
	inline constexpr closure last_n_const = []<std::ranges::contiguous_range R>(R&& r)
	{
		return std::span{std::forward<R>(r)}.template last<count>();
	};

	inline constexpr adaptor last_n = []<std::ranges::contiguous_range R>(R&& r, size_t count)
	{
		return std::span{std::forward<R>(r)}.last(count);
	};
}
#endif
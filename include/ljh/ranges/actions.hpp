
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// actions.hpp - v1.0
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

namespace ljh::ranges::actions
{
	namespace __
	{
		template<size_t length, std::ranges::range R>
		auto to_array(R&& range)
		{
			std::array<std::ranges::range_value_t<R>, length> decoded_key{};
			for (auto [c, i] : std::ranges::views::zip(range, std::views::iota(0)) | std::views::take(length))
				decoded_key[i] = c;
			return decoded_key;
		}
	}

	template<size_t Length>
	inline constexpr closure to_array = []<std::ranges::viewable_range R>(R&& r) { return __::to_array<Length>(); };
}

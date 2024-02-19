
//          Copyright Jared Irwin 2020-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// elements_of.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++14
// 
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "../cpp_version.hpp"

#if __has_include(<ranges>) && __cpp_lib_ranges
#include <ranges>
#endif

namespace ljh::ranges
{
#if __has_include(<ranges>) && __cpp_lib_ranges
	template<std::ranges::range rng>
#else 
	template<typename rng>
#endif
	struct elements_of
	{
		LJH_NO_UNIQUE_ADDRESS rng range;

#if defined(__clang__) && (LJH_CLANG_VERSION < LJH_COMPILER_GEN_VERSION(17, 0))
		template<typename r>
		elements_of(r&& rg)
			: range(std::move(rg))
		{}
#endif
	};

	template<typename rng>
	elements_of(rng&&) -> elements_of<rng&&>;
}

//          Copyright Jared Irwin 2021-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// case_insensitive_string.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++11
// 
// ABOUT
//     Case-insensitive strings
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"

#include <string>
#if __cpp_lib_string_view >= 201606L
#include <string_view>
#endif

namespace ljh
{
	template<typename C, typename A = std::allocator<C>>
	using basic_case_insensitive_string = std::string<C, case_insensitive_char_traits<C>, A>;

	using case_insensitive_string    = basic_case_insensitive_string<char    >;
	using wcase_insensitive_string   = basic_case_insensitive_string<wchar_t >;
#if __cpp_char8_t >= 201811L
	using u8case_insensitive_string  = basic_case_insensitive_string<char8_t >;
#endif
#if __cpp_unicode_characters >= 200704L
	using u16case_insensitive_string = basic_case_insensitive_string<char16_t>;
	using u32case_insensitive_string = basic_case_insensitive_string<char32_t>;
#endif
}
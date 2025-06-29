
//          Copyright Jared Irwin 2021-2025
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
	template<typename T>
	struct case_insensitive_char_traits : public std::char_traits<T>
	{
	private:
		using base = std::char_traits<T>;

		static constexpr typename base::char_type to_upper(typename base::char_type c)
		{
			if ('a' <= c && c <= 'z')
				return c - ('a' - 'A');
			return c;
		}

	public:
		static constexpr bool eq(typename base::char_type  c1, typename base::char_type  c2) noexcept { return to_upper(c1) == to_upper(c2); }
		static constexpr bool lt(typename base::char_type  c1, typename base::char_type  c2) noexcept { return to_upper(c1) <  to_upper(c2); }

		static constexpr int compare(const typename base::char_type* s1, const typename base::char_type* s2, size_t count) noexcept
		{
			for (int i = 0; i < count; i++)
			{
				if (to_upper(s1[i]) < to_upper(s2[i])) return -1;
				if (to_upper(s1[i]) > to_upper(s2[i])) return  1;
			}
			return 0;
		}
	};

	template<typename C, typename A = std::allocator<C>>
	using basic_case_insensitive_string = std::basic_string<C, case_insensitive_char_traits<C>, A>;

	using case_insensitive_string    = basic_case_insensitive_string<char    >;
	using wcase_insensitive_string   = basic_case_insensitive_string<wchar_t >;
#if __cpp_char8_t >= 201811L
	using u8case_insensitive_string  = basic_case_insensitive_string<char8_t >;
#endif
#if __cpp_unicode_characters >= 200704L
	using u16case_insensitive_string = basic_case_insensitive_string<char16_t>;
	using u32case_insensitive_string = basic_case_insensitive_string<char32_t>;
#endif

#if __cpp_lib_string_view >= 201606L
	template<typename C>
	using basic_case_insensitive_string_view = std::basic_string_view<C, case_insensitive_char_traits<C>>;

	using case_insensitive_string_view    = basic_case_insensitive_string_view<char    >;
	using wcase_insensitive_string_view   = basic_case_insensitive_string_view<wchar_t >;
#if __cpp_lib_string_view >= 201606L
#if __cpp_char8_t >= 201811L
	using u8case_insensitive_string_view  = basic_case_insensitive_string_view<char8_t >;
#endif
#if __cpp_unicode_characters >= 200704L
	using u16case_insensitive_string_view = basic_case_insensitive_string_view<char16_t>;
	using u32case_insensitive_string_view = basic_case_insensitive_string_view<char32_t>;
#endif
#endif
}
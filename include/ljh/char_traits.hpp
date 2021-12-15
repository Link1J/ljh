
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// char_traits.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++11
// 
// ABOUT
//     custom char_traits
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"

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
}
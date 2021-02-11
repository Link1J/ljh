
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "cpp_version.hpp"

#if LJH_CPP_VERSION > LJH_CPP17_VERSION
#if __has_include(<compare>)
#include <compare>
#define _i_LJH_COMPARE
#endif
#endif

namespace ljh
{
	template<typename T>
	struct _i_char_traits
	{
		using char_type  = T;
		using int_type   = int;
		using off_type   = size_t;
		using pos_type   = size_t;
		using state_type = size_t;
#if defined(_i_LJH_COMPARE)
		using comparison_category = std::strong_ordering;
#endif

		static constexpr bool eq(char_type  c1, char_type  c2) noexcept { return c1 == c2; }
		static constexpr bool lt(char_type  c1, char_type  c2) noexcept { return c1 <  c2; }

		static constexpr int compare(const char_type* s1, const char_type* s2, size_t count) noexcept
		{
			for (int i = 0; i < count; i++)
			{
				if (s1[i] < s2[i]) return -1;
				if (s1[i] > s2[i]) return  1;
			}
			return 0;
		}

		static constexpr size_t length(const char_type* s) noexcept
		{
			if (s == nullptr)
				return 0;
			auto end = s;
			while (*end != 0) { end++; }
			return end - s;
		}

		static constexpr const char_type* find(const char_type* s1, std::size_t n1, char_type c) noexcept
		{
			return find(s1, n1, &c, 1);
		}

		static constexpr const char_type* find(const char_type* s1, std::size_t n1, const char_type* s2, std::size_t n2) noexcept
		{
			for (int p = 0; p < n1 - n2; p++)
				if (compare(s1 + p, s2, n2) == 0)
					return s1 + p;
			return nullptr;
		}

		static constexpr const char_type* rfind(const char_type* s1, std::size_t n1, const char_type* s2, std::size_t n2) noexcept
		{
			for (int p = (n1 - n2) - 1; p >= 0; p--)
				if (compare(s1 + p, s2, n2) == 0)
					return s1 + p;
			return nullptr;
		}

		static constexpr char_type* copy(char_type* dest, const char_type* src, std::size_t count) noexcept
		{
			for (int p = 0; p < count; p++)
				dest[p] = src[p];
			return dest;
		}

		static constexpr char_type* move(char_type* dest, const char_type* src, std::size_t count) noexcept
		{
			return copy(dest, src, count);
		}
	};

	template<typename T>
	struct _i_case_insensitive_char_traits : public _i_char_traits<T>
	{
	private:
		using base = _i_char_traits<T>;

		static constexpr base::char_type to_upper(base::char_type c)
		{
			if ('a' <= c && c <= 'z')
				return c - ('a' - 'A');
			return c;
		}

	public:
		static constexpr bool eq(base::char_type  c1, base::char_type  c2) noexcept { return to_upper(c1) == to_upper(c2); }
		static constexpr bool lt(base::char_type  c1, base::char_type  c2) noexcept { return to_upper(c1) <  to_upper(c2); }

		static constexpr int compare(const base::char_type* s1, const base::char_type* s2, size_t count) noexcept
		{
			for (int i = 0; i < count; i++)
			{
				if (to_upper(s1[i]) < to_upper(s2[i])) return -1;
				if (to_upper(s1[i]) > to_upper(s2[i])) return  1;
			}
			return 0;
		}
	};

	template<typename T>
	struct char_traits;

	template<> struct char_traits<char    > : public _i_char_traits<char    > {};
	template<> struct char_traits<wchar_t > : public _i_char_traits<wchar_t > {};
#if __cpp_char8_t >= 201811L
	template<> struct char_traits<char8_t > : public _i_char_traits<char8_t > {};
#endif
#if __cpp_unicode_characters >= 200704L
	template<> struct char_traits<char16_t> : public _i_char_traits<char16_t> {};
	template<> struct char_traits<char32_t> : public _i_char_traits<char32_t> {};
#endif


	template<typename T>
	struct case_insensitive_char_traits;

	template<> struct case_insensitive_char_traits<char    > : public _i_case_insensitive_char_traits<char    > {};
	template<> struct case_insensitive_char_traits<wchar_t > : public _i_case_insensitive_char_traits<wchar_t > {};
#if __cpp_char8_t >= 201811L
	template<> struct case_insensitive_char_traits<char8_t > : public _i_case_insensitive_char_traits<char8_t > {};
#endif
#if __cpp_unicode_characters >= 200704L
	template<> struct case_insensitive_char_traits<char16_t> : public _i_case_insensitive_char_traits<char16_t> {};
	template<> struct case_insensitive_char_traits<char32_t> : public _i_case_insensitive_char_traits<char32_t> {};
#endif
}
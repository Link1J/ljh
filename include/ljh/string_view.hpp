
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <ljh/_i_internal/string_view.hpp>

namespace ljh
{
	template<typename T, typename R = char_traits<T>>
	using basic_string_view = _i_basic_string_view<T,R>;

	using string_view      = basic_string_view<char    >;
	using wstring_view     = basic_string_view<wchar_t >;
#if __cpp_char8_t >= 201811L
	using u8string_view    = basic_string_view<char8_t >;
#endif
#if __cpp_unicode_characters >= 200704L
	using u16string_view   = basic_string_view<char16_t>;
	using u32string_view   = basic_string_view<char32_t>;
#endif

	using cistring_view      = basic_string_view<char    , case_insensitive_char_traits<char    >>;
	using ciwstring_view     = basic_string_view<wchar_t , case_insensitive_char_traits<wchar_t >>;
#if __cpp_char8_t >= 201811L
	using ciu8string_view    = basic_string_view<char8_t , case_insensitive_char_traits<char8_t >>;
#endif
#if __cpp_unicode_characters >= 200704L
	using ciu16string_view   = basic_string_view<char16_t, case_insensitive_char_traits<char16_t>>;
	using ciu32string_view   = basic_string_view<char32_t, case_insensitive_char_traits<char32_t>>;
#endif
}

namespace std
{
	template<typename _i_char, typename _traits>
	constexpr void swap(ljh::basic_string_view<_i_char, _traits>& lhs, ljh::basic_string_view<_i_char, _traits>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}
}

namespace ljh
{
	template <typename _char, typename _traits>
	std::basic_ostream<_char, std::char_traits<typename _traits::char_type>>& operator<<(std::basic_ostream<_char, std::char_traits<typename _traits::char_type>>& os, const ljh::basic_string_view<_char, _traits>& str)
	{
		typename ljh::basic_string_view<_char, _traits>::size_type width = os.width();

		if (width > 0 && (os.flags() & std::ios_base::adjustfield) != std::ios_base::left)
			for (auto i = 0; i < width - str.size(); i++)
				os.put(os.fill());

		auto n = width < str.size() ? str.size() : width;
		os.rdbuf()->sputn(str.data(), n);

		if (width > 0 && (os.flags() & std::ios_base::adjustfield) == std::ios_base::left)
			for (auto i = 0; i < width - str.size(); i++)
				os << os.fill();

		os.width(0);
		return os;
	}

	template <typename _char, typename _traits>
	std::basic_istream<_char, std::char_traits<typename _traits::char_type>>& operator>>(std::basic_istream<_char, std::char_traits<typename _traits::char_type>>& is, const ljh::basic_string_view<_char, _traits>& str)
	{
		auto N = is.width() > 0 ? is.width() : str.max_size();
		str.erase();

		while (true)
		{
			if (std::isspace(is.peek(), is.getloc()))
				break;
			str.append(1, is.get());
			if (--N == 0)
				break;
		}

		is.width(0);
		return is;
	}
}
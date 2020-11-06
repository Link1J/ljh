
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// type_traits.hpp - v1.1
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
// Requires cpp_version.hpp
//
// Based on code from https://stackoverflow.com/a/10724828/13155694
//
// ABOUT
//     Adds extra type traits
//
// USAGE
//
// Version History
//     1.0 Inital Version
//     1.1 Add is_char and rename to type_traits.hpp
//     1.2 Change is_char to is_char_type

#pragma once
#include "cpp_version.hpp"
#include <type_traits>

namespace ljh
{
	template <typename T, bool = std::is_enum<T>::value>
	struct is_scoped_enum : std::false_type {};

	template <typename T>
	struct is_scoped_enum<T, true> : std::integral_constant<bool, !std::is_convertible<T, typename std::underlying_type<T>::type>::value> {};

	template<typename T>
	struct is_char_type : std::false_type {};

	template<> struct is_char_type<         char    > : std::true_type {};
	template<> struct is_char_type<signed   char    > : std::true_type {};
	template<> struct is_char_type<unsigned char    > : std::true_type {};
	template<> struct is_char_type<        wchar_t  > : std::true_type {};
#if defined(__cpp_char8_t)
	template<> struct is_char_type<         char8_t > : std::true_type {};
#endif
	template<> struct is_char_type<         char16_t> : std::true_type {};
	template<> struct is_char_type<         char32_t> : std::true_type {};

#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
	template <typename T>
	inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;
	
	template<typename T>
	inline constexpr bool is_char_type_v = is_char_type<T>::value;
#endif
}

//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// static_cast_underlying.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++11
// 
// ABOUT
//     A type-safe function for casting scoped enums to underlying type
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include <type_traits>

namespace ljh
{
	template<typename T> 
	constexpr typename std::underlying_type<T>::type static_cast_underlying(T a) 
	{
		return static_cast<typename std::underlying_type<T>::type>(a); 
	};
}
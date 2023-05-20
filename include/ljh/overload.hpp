
//          Copyright Jared Irwin 2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// overload.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++11
// 
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "cpp_version.hpp"

namespace ljh
{
	template<typename... T>
	struct overload : T...
	{
		using T::operator()...;
	};

	template<typename... T>
	overload(T...) -> overload<T...>;
}
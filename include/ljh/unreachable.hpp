
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// unreachable.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++
// Requires cpp_version.hpp
//
// Based on code from https://stackoverflow.com/q/81870
//
// ABOUT
//     Adds extra type traits
//
// USAGE
//
// Version History
//     1.0 Inital Version

#include "cpp_version.hpp"

namespace ljh
{
	LJH_NORETURN inline void unreachable()
	{
#if defined(LJH_COMPILER_CLANG)
		__builtin_unreachable();
#elif defined(LJH_COMPILER_GCC)
		__builtin_unreachable();
#elif defined(LJH_COMPILER_MSVC)
		__assume(false);
#endif
	}
}

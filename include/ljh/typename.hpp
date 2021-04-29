
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// type_traits.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
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

#pragma once

#include "cpp_version.hpp"
#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
#include <string_view>
#elif LJH_CPP_VERSION >= LJH_CPP14_VERSION
#include "compile_time_string.hpp"
#endif

namespace ljh
{
#if LJH_CPP_VERSION >= LJH_CPP17_VERSION
	template<typename T>
	constexpr auto type_name() noexcept
	{
		std::string_view name = "Error: unsupported compiler", prefix, suffix;
#if defined(LJH_COMPILER_CLANG)
		name   = __PRETTY_FUNCTION__;
		prefix = "auto type_name() [T = ";
		suffix = "]";
#elif defined(LJH_COMPILER_GCC)
		name   = __PRETTY_FUNCTION__;
		prefix = "constexpr auto type_name() [with T = ";
		suffix = "]";
#elif defined(LJH_COMPILER_MSVC)
		name   = __FUNCSIG__;
		prefix = "auto __cdecl type_name<";
		suffix = ">(void) noexcept";
#endif
		name.remove_prefix(prefix.size());
		name.remove_suffix(suffix.size());
		return name;
	}
#elif LJH_CPP_VERSION >= LJH_CPP14_VERSION
	template<typename T>
	constexpr auto type_name() noexcept
	{
#if defined(LJH_COMPILER_CLANG)
		compile_time_string name = __PRETTY_FUNCTION__;
		return compile_time_string(name.data() + 31, name.size() - 31 - 1);
#elif defined(LJH_COMPILER_GCC)
		compile_time_string name = __PRETTY_FUNCTION__;
#if LJH_CPP_VERSION < LJH_CPP14_VERSION
		return compile_time_string(name.data() + 36, name.size() - 36 - 1);
#else
		return compile_time_string(name.data() + 46, name.size() - 46 - 1);
#endif
#elif defined(LJH_COMPILER_MSVC)
		compile_time_string name = __FUNCSIG__;
		return compile_time_string(p.data() + 38, p.size() - 38 - 7);
#else
		return compile_time_string("Error: unsupported compiler");
#endif
	}
#endif
} // namespace ljh
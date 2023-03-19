
//          Copyright Jared Irwin 2020-2023
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
#include <cstdint>

#if __cpp_lib_constexpr_string >= 201907L
#include <string>
#include <string_view>
#include <tuple>
#elif __cpp_lib_string_view >= 201606L
#include <string_view>
#endif

namespace ljh
{
	template<typename T>
	[[nodiscard]] constexpr auto type_name() noexcept
	{
#if !(__cpp_lib_constexpr_string >= 201907L || __cpp_lib_string_view >= 201606L)
		return "Error: unsupported C++ version";
#else
#if defined(LJH_COMPILER_CLANG)
		constexpr char prefix_text[] = "auto ljh::type_name() [T = ";
		constexpr char suffix_text[] = "]";
#elif defined(LJH_COMPILER_GCC)
		constexpr char prefix_text[] = "constexpr auto ljh::type_name() [with T = ";
		constexpr char suffix_text[] = "]";
#elif defined(LJH_COMPILER_MSVC)
		constexpr char prefix_text[] = "auto __cdecl ljh::type_name<";
		constexpr char suffix_text[] = ">(void) noexcept";
#endif
		constexpr std::size_t prefix = sizeof(prefix_text) - 1;
		constexpr std::size_t suffix = sizeof(suffix_text) - 1;

#if __cpp_lib_constexpr_string >= 201907L
		std::string name = LJH_PRETTY_FUNCTION;
#elif __cpp_lib_string_view >= 201606L
		std::string_view name = LJH_PRETTY_FUNCTION;
#endif

		name = name.substr(prefix, name.size() - prefix - suffix);

#if defined(LJH_COMPILER_MSVC)
#if __cpp_lib_constexpr_string >= 201907L
		for (std::size_t a = 0; a < name.size(); a++) {
			auto part = name.substr(a);
			if (part.starts_with("struct "))
				name.erase(a, 7);
			else if (part.starts_with("class "))
				name.erase(a, 6);
			else if (part.starts_with("enum "))
				name.erase(a, 5);
		}
#elif __cpp_lib_string_view >= 201606L
		if (part.starts_with("struct "))
			name.remove_prefix(7);
		else if (part.starts_with("class "))
			name.remove_prefix(6);
		else if (part.starts_with("enum "))
			name.remove_prefix(5);
#endif
#endif

		return name;
#endif
	}
}
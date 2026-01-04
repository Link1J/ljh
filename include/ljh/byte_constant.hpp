
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// fixed_point.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++17
///
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#include <string>
#include <cstdint>

namespace ljh
{
#if LJH_CPP_VERSION >= LJH_CPP20_VERSION
    namespace __
    {
        struct udl_string
        {
            char const* value;
            constexpr udl_string(char const* value)
                : value(value)
            {}
        };
    } // namespace __

    LJH_MODULE_STRING_EXPORT inline namespace literals
    {
        template<__::udl_string String>
        constexpr auto operator""_bytes()
        {
            return []<std::size_t... I>(std::index_sequence<I...>) {
                return std::array{std::uint8_t(String.value[I])...};
            }(std::make_index_sequence<std::char_traits<char>::length(String.value)>{});
        }
    }
#endif
} // namespace ljh
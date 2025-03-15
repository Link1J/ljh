
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// smarc/control_block.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++23
// Requires MVSC 19.32, GCC 14 or Clang 18
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

namespace ljh::smarc
{
    template<typename T>
    struct node;

    template<typename T>
    struct ref;

    template<typename T>
    struct ptr;

    template<typename T>
    struct list;

    namespace __
    {
        template<typename T>
        struct control_block;

        template<typename T, bool is_const, bool reverse>
        struct list_iterator;
    } // namespace __
} // namespace ljh::smarc
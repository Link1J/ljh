
//          Copyright Jared Irwin 2023-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// thread_name.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "cpp_version.hpp"
#include <string>
#include <thread>

namespace ljh::thread_name
{
    LJH_MODULE_OS_EXPORT void set_for_current(std::string const& name);
    LJH_MODULE_OS_EXPORT void set(std::thread::native_handle_type id, std::string const& name);

    LJH_MODULE_OS_EXPORT inline void set(std::thread /*const*/& thread, std::string const& name)
    {
        set(thread.native_handle(), name);
    }
} // namespace ljh::thread_name
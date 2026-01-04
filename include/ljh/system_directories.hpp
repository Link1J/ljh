
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// system_directories.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++
//
// ABOUT
//     Functions for getting system directories.
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "cpp_version.hpp"
#include <string>

namespace ljh
{
    namespace system_directories
    {
        LJH_MODULE_OS_EXPORT std::string home();
        LJH_MODULE_OS_EXPORT std::string cache();
        LJH_MODULE_OS_EXPORT std::string config();
        LJH_MODULE_OS_EXPORT std::string data();
        LJH_MODULE_OS_EXPORT std::string documents();
        LJH_MODULE_OS_EXPORT std::string desktop();
        LJH_MODULE_OS_EXPORT std::string pictures();
        LJH_MODULE_OS_EXPORT std::string music();
        LJH_MODULE_OS_EXPORT std::string videos();
        LJH_MODULE_OS_EXPORT std::string downloads();
        LJH_MODULE_OS_EXPORT std::string save_games();
    } // namespace system_directories
} // namespace ljh
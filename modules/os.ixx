
//          Copyright Jared Irwin 2025-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#define LJH_MODULE_OS 1
#define LJH_MODULE_OS_EXPORT export
export module ljh.os;
extern "C++"
{
#include "ljh/thread_name.hpp"
#include "ljh/system_info.hpp"
#include "ljh/system_directories.hpp"
#include "ljh/named_mutex.hpp"
#include "ljh/memory_mapped_file.hpp"
#include "ljh/delay_loaded_functions.hpp"
}
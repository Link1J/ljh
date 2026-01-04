
//          Copyright Jared Irwin 2025-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#define LJH_MODULE_STRING 1
#define LJH_MODULE_STRING_EXPORT export
export module ljh.string;
extern "C++"
{
#include "ljh/string_utils.hpp"
#include "ljh/compile_time_string.hpp"
#include "ljh/char_convertions.hpp"
#include "ljh/case_insensitive_string.hpp"
#include "ljh/byte_constant.hpp"
}
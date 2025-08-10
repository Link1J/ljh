
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

module;
#define LJH_MODULE_COROUTINE 1
#define LJH_MODULE_COROUTINE_EXPORT export
export module ljh.coroutine;
extern "C++"
{
#include "ljh/coroutine.hpp"
#include "ljh/channel.hpp"
#include "ljh/generator.hpp"
#include "ljh/token_bucket.hpp"
}

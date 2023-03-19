
//          Copyright Jared Irwin 2021-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../cpp_version.hpp"

#if __cpp_impl_coroutine >= 201902L && __cpp_lib_coroutine >= 201902L
#include <coroutine>
#define LJH_COROUTINES_AVALIABLE 1
#elif __has_include(<experimental/coroutine>)
#include <experimental/coroutine>
namespace std { using namespace std::experimental; }
#define LJH_COROUTINES_AVALIABLE 1
#endif
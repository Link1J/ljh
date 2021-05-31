
//          Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// coroutine.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
// Requires MVSC 16.8
//
// ABOUT
//     A header to include all C++ coroutine stuff
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"

#include "coroutine/invoke_lambda.hpp"
#include "coroutine/fire_and_forget.hpp"
#include "coroutine/shared_mutex.hpp"
#include "coroutine/task.hpp"
#include "coroutine/cold_task.hpp"
//#include "coroutine/com_aware_task.hpp"
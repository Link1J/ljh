
//          Copyright Jared Irwin 2021-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "coroutine_headers.hpp"

namespace ljh::co
{
    LJH_MODULE_COROUTINE_EXPORT template<typename TLambda>
    auto invoke_lambda(TLambda lambda) -> decltype(lambda())
    {
        co_return co_await lambda();
    }
} // namespace ljh::co
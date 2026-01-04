
//          Copyright Jared Irwin 2021-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <utility>

namespace ljh::__::co
{
    struct promise_policies
    {
        bool m_terminate_on_unhandled_exception = false;
    };
} // namespace ljh::__::co

namespace ljh::co
{
    struct task_policy
    {
        task_policy(__::co::promise_policies& policies)
            : policies(policies)
        {}

        bool terminate_on_unhandled_exception(bool value = true) const noexcept
        {
            return std::exchange(policies.m_terminate_on_unhandled_exception, value);
        }

    private:
        __::co::promise_policies& policies;
    };

    struct get_task_policy
    {};
} // namespace ljh::co

//          Copyright Jared Irwin 2021-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "promise.hpp"

#ifdef __linux__
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace ljh::__::co
{
#if defined(__linux__)
    using check_type = int;
#else
    using check_type = bool;
#endif

    template<typename T>
    struct task_base
    {
        task_base(promise<T>* initial = nullptr) noexcept
            : promise(initial)
        {}

        struct cannot_await_lvalue_use_std_move
        {};
        cannot_await_lvalue_use_std_move operator co_await() & = delete;

        T get() &&
        {
            if (!promise->client_await_ready())
            {
                check_type completed = false;
                if (promise->client_await_suspend(&completed, wake_by_address))
                {
                    auto ready = completed;
                    while (!completed)
                    {
#if defined(_WIN32)
                        WaitOnAddress(&completed, &ready, sizeof(completed), INFINITE);
#elif defined(__linux__)
                        syscall(SYS_futex, &completed, FUTEX_WAIT_PRIVATE, ready, nullptr, nullptr, 0);
#endif
                    }
                }
            }
            return std::exchange(promise, {})->client_await_resume();
        }

    protected:
        promise_ptr<T> promise;

        static void wake_by_address(void* completed)
        {
            *reinterpret_cast<check_type*>(completed) = true;
#if defined(_WIN32)
            WakeByAddressSingle(completed);
#elif defined(__linux__)
            syscall(SYS_futex, &completed, FUTEX_WAKE_PRIVATE, 1, nullptr, nullptr, 0);
#endif
        }
    };
} // namespace ljh::__::co

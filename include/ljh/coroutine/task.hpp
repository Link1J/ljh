
//          Copyright Jared Irwin 2021-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "task_base.hpp"

namespace ljh::__::co
{
    template<typename T>
    struct promise_awaiter
    {
        promise_ptr<T> self;

        bool await_ready()
        {
            return self->client_await_ready();
        }

        auto await_suspend(std::coroutine_handle<> handle)
        {
            return self->client_await_suspend(handle.address());
        }

        T await_resume()
        {
            return self->client_await_resume();
        }
    };
} // namespace ljh::__::co

namespace ljh::co
{
    template<typename T>
    struct task : __::co::task_base<T>
    {
        using base = __::co::task_base<T>;
        task()     = default;
        task(__::co::promise<T>* initial)
            : base(initial)
        {
            this->_promise->start();
        }

        void swap(task& other)
        {
            std::swap(this->_promise, other.promise);
        }

        using base::operator co_await;

        auto operator co_await() &&
        {
            return __::co::promise_awaiter<T>{std::move(this->_promise)};
        }
    };

    template<typename T>
    void swap(task<T>& left, task<T>& right)
    {
        left.swap(right);
    }
} // namespace ljh::co

template<typename T, typename... Args>
struct std::coroutine_traits<ljh::co::task<T>, Args...>
{
    using promise_type = ljh::__::co::promise<T>;
};

//          Copyright Jared Irwin 2021-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "state.hpp"
#include <atomic>

namespace ljh::co
{
    struct shared_mutex_state;

    template<>
    struct extra_await_data<shared_mutex_state>
    {
        extra_await_data(bool kind)
            : exclusive(kind)
        {}
        bool exclusive;
    };

    struct shared_mutex_state : state<shared_mutex_state>
    {
        std::atomic<int> owners;

        static bool exclusive_transition(int current, int& future) noexcept
        {
            if (current != 0)
                return false;
            future = -1;
            return true;
        }

        static bool shared_transition(int current, int& future) noexcept
        {
            if (current < 0)
                return false;
            future = current + 1;
            return true;
        }

        bool fast_claim(extra_await_data const& e) noexcept
        {
            if (any_waiters())
                return false;
            if (e.exclusive)
                return calc_claim<true>(owners, 0, exclusive_transition);
            else
                return calc_claim<true>(owners, shared_transition);
        }

        bool claim(extra_await_data const& e) noexcept
        {
            if (any_waiters())
                return false;
            if (e.exclusive)
                return calc_claim<false>(owners, exclusive_transition);
            else
                return calc_claim<false>(owners, shared_transition);
        }

        void unlock_exclusive(node_list& list)
        {
            resume_a_bunch(list);
        }

        void unlock_shared(node_list& list)
        {
            auto current = owners.load(std::memory_order_relaxed);
            int  future;
            do
            {
                if (current == 1)
                    future = -1;
                else
                    future = current - 1;
            }
            while (owners.compare_exchange_weak(current, future, std::memory_order_relaxed));
            if (future == -1)
            {
                resume_a_bunch(list);
            }
        }

        void resume_a_bunch(node_list& list)
        {
            auto count = 0;
            auto peek  = peek_head();
            if (!peek)
            {
                // nobody to release
            }
            else if (peek->exclusive)
            {
                resume_one(list);
                count = -1;
            }
            else
            {
                do
                {
                    ++count;
                    resume_one(list);
                    peek = peek_head();
                }
                while (peek && !peek->exclusive);
            }
            owners.store(count, std::memory_order_relaxed);
        }
    };

    LJH_MODULE_COROUTINE_EXPORT struct shared_mutex : sync_object<shared_mutex_state>
    {
        void operator co_await() = delete;

        auto lock_shared()
        {
            return make_awaiter(false);
        }
        auto lock_exclusive()
        {
            return make_awaiter(true);
        }

        void unlock_exclusive()
        {
            action_impl(&state::unlock_exclusive);
        }

        void unlock_shared()
        {
            action_impl(&state::unlock_shared);
        }
    };
} // namespace ljh::co
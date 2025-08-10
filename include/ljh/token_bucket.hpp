
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// token_bucket.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++11
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"

#include <atomic>
#include <chrono>

#if __cpp_impl_coroutine >= 201902L && __cpp_lib_coroutine >= 201902L
#include "coroutine/state.hpp"
#endif

namespace ljh
{
#if __cpp_impl_coroutine >= 201902L && __cpp_lib_coroutine >= 201902L
    LJH_MODULE_COROUTINE_EXPORT class token_bucket;
    struct token_bucket_state;

    template<>
    struct co::extra_await_data<token_bucket_state>
    {
        extra_await_data(token_bucket* bucket, uint64_t tokens)
            : tokens(tokens)
            , bucket(bucket)
        {}

        token_bucket* bucket;
        uint64_t      tokens;
    };

    struct token_bucket_state : co::state<token_bucket_state>
    {
        token_bucket_state() = default;

        bool fast_claim(extra_await_data const& e) noexcept;
        bool claim(extra_await_data const& e) noexcept;
    };
#endif

    class token_bucket
#if __cpp_impl_coroutine >= 201902L && __cpp_lib_coroutine >= 201902L
        : public co::sync_object<token_bucket_state>
#endif
    {
    public:
        token_bucket()
        {}

        token_bucket(uint64_t const rate, uint64_t const burstSize)
        {
            timePerToken_ = 1000000 / rate;
            timePerBurst_ = burstSize * timePerToken_;
        }

        token_bucket(token_bucket const& other)
        {
            timePerToken_ = other.timePerToken_.load();
            timePerBurst_ = other.timePerBurst_.load();
        }

        token_bucket& operator=(token_bucket const& other)
        {
            timePerToken_ = other.timePerToken_.load();
            timePerBurst_ = other.timePerBurst_.load();
            return *this;
        }

        bool consume(uint64_t const tokens)
        {
            uint64_t const now        = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            uint64_t const timeNeeded = tokens * timePerToken_.load(std::memory_order::relaxed);
            uint64_t const minTime    = now - timePerBurst_.load(std::memory_order::relaxed);
            uint64_t       oldTime    = time_.load(std::memory_order::relaxed);
            uint64_t       newTime    = oldTime;

            if (minTime > oldTime)
            {
                newTime = minTime;
            }

            for (;;)
            {
                newTime += timeNeeded;
                if (newTime > now)
                {
                    return false;
                }
                if (time_.compare_exchange_weak(oldTime, newTime, std::memory_order::relaxed, std::memory_order::relaxed))
                {
                    return true;
                }
                newTime = oldTime;
            }
        }

#if __cpp_impl_coroutine >= 201902L && __cpp_lib_coroutine >= 201902L
        [[nodiscard]] auto consume_async(const uint64_t tokens) noexcept
        {
            return make_awaiter(co::extra_await_data<token_bucket_state>{this, tokens});
        }

        auto operator co_await()
        {
            return consume_async(1);
        }
#endif

    private:
        std::atomic<uint64_t> time_         = {0};
        std::atomic<uint64_t> timePerToken_ = {0};
        std::atomic<uint64_t> timePerBurst_ = {0};
    };

#if __cpp_impl_coroutine >= 201902L && __cpp_lib_coroutine >= 201902L
    bool token_bucket_state::fast_claim(extra_await_data const& e) noexcept
    {
        return e.bucket->consume(e.tokens);
    }

    bool token_bucket_state::claim(extra_await_data const& e) noexcept
    {
        return e.bucket->consume(e.tokens);
    }
#endif
} // namespace ljh


//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// smarc/control_block.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++23
// Requires MVSC 19.32, GCC 14 or Clang 18
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "fwd.hpp"
#include <atomic>
#include <utility>
#include <bit>

namespace ljh::smarc::__
{
    template<typename T>
    struct control_block
    {
        control_block(uintptr_t strong, T* object) noexcept
            : strong(strong)
            , object(object)
        {}

        void inc_weak() noexcept
        {
            weak.fetch_add(1, std::memory_order::relaxed);
        }

        void dec_weak() noexcept
        {
            if (weak.fetch_sub(1, std::memory_order::acq_rel) == 1)
                delete this;
        }

        void inc_strong(size_t count = 1) noexcept
        {
            strong.fetch_add(count, std::memory_order::relaxed);
        }

        bool inc_strong_nz(size_t count = 1) noexcept
        {
            auto sc = strong.load(std::memory_order::relaxed);
            while (sc != 0)
                if (strong.compare_exchange_weak(sc, sc + count, std::memory_order::relaxed))
                    return true;
            return false;
        }

        size_t dec_strong(size_t count = 1) noexcept
        {
            auto rem = strong.fetch_sub(count, std::memory_order::acq_rel);
            if (rem == count)
            {
                std::exchange(object, nullptr);
                dec_weak();
            }
            return rem - count;
        }

        size_t use_strong() const noexcept
        {
            return strong.load();
        }

        std::atomic<size_t> strong = 0;
        std::atomic<size_t> weak   = 1;
        T*                  object = nullptr;
    };
} // namespace ljh::smarc::__

//          Copyright Jared Irwin 2024-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// smarc/ref.hpp - v1.0
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
#include "control_block.hpp"
#include <concepts>

namespace ljh::smarc
{
    /// @brief A weak reference to a smarc item
    /// @tparam T Node data type
    template<typename T>
    struct ref
    {
        // static_assert(std::derived_from<T, node<T>>);

        ref() noexcept = default;

        ref(T& other) noexcept
        {
            copy_ref(other.make_weak_ref());
        }

        ref(ptr<T> const& other) noexcept
        {
            copy_ref(other->make_weak_ref());
        }

        ref(ref const& other) noexcept
        {
            copy_ref(other.block);
        }

        ref(ref&& other) noexcept
        {
            std::swap(block, other.block);
        }

        ~ref() noexcept
        {
            reset();
        }

        ref<T>& operator=(ref const& other) noexcept
        {
            reset();
            copy_ref(other.block);
            return *this;
        }

        ref<T>& operator=(ref&& other) noexcept
        {
            std::swap(block, other.block);
            return *this;
        }

        ref<T>& operator=(T& other) noexcept
        {
            reset();
            copy_ref(other.make_weak_ref());
            return *this;
        }

        ref<T>& operator=(ptr<T> other) noexcept
        {
            reset();
            copy_ref(other->make_weak_ref());
            return *this;
        }

        void reset() noexcept
        {
            if (auto dead = std::exchange(block, nullptr); dead != nullptr)
                dead->dec_weak();
        }

        [[nodiscard]] size_t use_count() const noexcept
        {
            return block != nullptr ? block->use_strong() : 0;
        }

        [[nodiscard]] bool expired() const noexcept
        {
            return use_count() == 0;
        }

        [[nodiscard]] ptr<T> lock() const noexcept
        {
            ptr<T> out;
            out.ctor_from_weak(*this);
            return out;
        }

    private:
        friend ptr<T>;
        friend node<T>;

        void copy_ref(__::control_block<T>* other) noexcept
        {
            if (other)
            {
                block = other;
                block->inc_weak();
            }
        }

        __::control_block<T>* block = nullptr;
    };
} // namespace ljh::smarc
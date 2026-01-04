
//          Copyright Jared Irwin 2024-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// smarc/ptr.hpp - v1.0
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
#include <concepts>
#include <utility>
#include <memory>

namespace ljh::smarc
{
    /// @brief A strong reference to a smarc item
    /// @tparam T Node data type
    template<typename T>
    struct ptr
    {
        // static_assert(std::derived_from<T, node<T>>);

        ptr() noexcept = default;
        ptr(std::nullptr_t) noexcept
        {}

        ptr(T& other) noexcept
        {
            copy_ref(std::addressof(other));
        }

        ptr(ptr const& other) noexcept
        {
            copy_ref(other.object);
        }

        ptr(ptr&& other) noexcept
        {
            object       = other.object;
            other.object = nullptr;
        }

        ~ptr() noexcept
        {
            reset();
        }

        ptr<T>& operator=(T& other) noexcept
        {
            if (object != std::addressof(other))
            {
                reset();
                copy_ref(std::addressof(other));
            }
            return *this;
        }

        ptr<T>& operator=(ptr const& other) noexcept
        {
            if (this != std::addressof(other))
            {
                reset();
                copy_ref(other.object);
            }
            return *this;
        }

        ptr<T>& operator=(ptr&& other) noexcept
        {
            if (this != std::addressof(other))
            {
                std::swap(object, other.object);
            }
            return *this;
        }

        void reset() noexcept
        {
            std::exchange(object, nullptr)->safe_dec_strong();
        }

        T* get() const noexcept
        {
            return object;
        }

        T* operator->() const noexcept
        {
            return get();
        }

        T& operator*() const noexcept
        {
            return *get();
        }

        explicit operator bool() const noexcept
        {
            return object;
        }

        friend bool operator==(ptr const& lhs, ptr const& rhs) noexcept
        {
            return lhs.object == rhs.object;
        }

        friend bool operator==(ptr const& lhs, std::nullptr_t) noexcept
        {
            return !lhs;
        }

        template<typename U>
        friend ptr<U> make_item(auto&&... args) noexcept
            requires(std::derived_from<U, node<U>> && std::constructible_from<U, decltype(args)...>);

    private:
        friend ref<T>;
        friend node<T>;

        ptr(T* object) noexcept
            : object(object)
        {}

        void ctor_from_weak(ref<T> const& other) noexcept
        {
            if (other.block && other.block->inc_strong_nz())
                object = other.block->object;
        }

        void copy_ref(T* other) noexcept
        {
            if (other && other->inc_strong_nz())
                object = other;
        }

        T* object = nullptr;
    };

    template<typename T>
    ptr<T> make_item(auto&&... args) noexcept
        requires(std::derived_from<T, node<T>> && std::constructible_from<T, decltype(args)...>)
    {
        return {new (std::nothrow) T(std::forward<decltype(args)>(args)...)};
    }
} // namespace ljh::smarc
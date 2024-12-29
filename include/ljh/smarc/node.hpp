
//          Copyright Jared Irwin 2024-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// smarc/node.hpp - v1.0
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
#include <numeric>

namespace ljh::smarc
{
    namespace __
    {
        struct refcount
        {

            static constexpr uintptr_t ptr_bit = uintptr_t(1) << (std::numeric_limits<uintptr_t>::digits - 1);

            static bool is_count_ptr(uintptr_t references) noexcept
            {
                return (references & ptr_bit) == ptr_bit;
            }

            template<typename T>
            static __::control_block<T>* decode_count_ptr(uintptr_t references) noexcept
            {
                return reinterpret_cast<__::control_block<T>*>(references << 1);
            }

            template<typename T>
            static uintptr_t encode_count_ptr(__::control_block<T>* references) noexcept
            {
                return reinterpret_cast<uintptr_t>(references) >> 1 | ptr_bit;
            }

            template<typename T>
            __::control_block<T>* make_weak_ref(T* object) const noexcept
            {
                auto cop = ref_count.load(std::memory_order::relaxed);
                if (is_count_ptr(cop))
                    return decode_count_ptr<T>(cop);

                auto ptr = new (std::nothrow) __::control_block<T>(cop, object);
                auto enc = encode_count_ptr(ptr);

                while (true)
                {
                    if (ref_count.compare_exchange_weak(cop, enc, std::memory_order::acq_rel, std::memory_order::relaxed))
                        return ptr;

                    if (is_count_ptr(cop))
                    {
                        delete ptr;
                        return decode_count_ptr<T>(cop);
                    }

                    ptr->strong.store(cop, std::memory_order::relaxed);
                }
            }

            void inc_strong(size_t count = 1) const noexcept
            {
                auto cop = ref_count.load(std::memory_order::relaxed);
                while (true)
                {
                    if (is_count_ptr(cop))
                        return decode_count_ptr<void>(cop)->inc_strong(count);
                    if (ref_count.compare_exchange_weak(cop, cop + count, std::memory_order::relaxed))
                        return;
                }
            }

            bool inc_strong_nz(size_t count = 1) const noexcept
            {
                auto cop = ref_count.load(std::memory_order::relaxed);
                while (cop != 0)
                {
                    if (is_count_ptr(cop))
                        return decode_count_ptr<void>(cop)->inc_strong_nz(count);
                    if (ref_count.compare_exchange_weak(cop, cop + count, std::memory_order::relaxed))
                        return true;
                }
                return false;
            }

            size_t dec_strong(size_t count = 1) const noexcept
            {
                auto cop = ref_count.load(std::memory_order::relaxed);
                while (true)
                {
                    if (is_count_ptr(cop))
                        return decode_count_ptr<void>(cop)->dec_strong(count);
                    if (ref_count.compare_exchange_weak(cop, cop - count, std::memory_order::relaxed))
                        return cop - count;
                }
            }

            size_t use_strong() const noexcept
            {
                auto cop = ref_count.load(std::memory_order::relaxed);
                if (is_count_ptr(cop))
                    return decode_count_ptr<void>(cop)->use_strong();
                return cop;
            }

            friend void swap(refcount& lhs, refcount& rhs)
            {
                // This probably isn't atomic, but it will hopefully work well enough.
                lhs.ref_count.store(rhs.ref_count.exchange(lhs.ref_count.load()));
            }

            mutable std::atomic<uintptr_t> ref_count = 1;
        };
    } // namespace __

    template<typename T>
    struct node
    {
        node(node const&)            = delete;
        node(node&&)                 = delete;
        node& operator=(node const&) = delete;
        node& operator=(node&&)      = delete;

        template<typename S>
        auto get_strong(this S& self) noexcept
        {
            ptr<S> out;
            out.copy_ref(std::addressof(self));
            return out;
        }

        template<typename S>
        auto get_weak(this S& self) noexcept
        {
            ref<S> out;
            out.copy_ref(self.make_weak_ref());
            return out;
        }

        void remove_from_list(this T& self) noexcept
        {
            self.delink();
            self.init();
        }

        void insert_before(this T& self, T& other) noexcept
        {
            if (std::addressof(self) == std::addressof(other))
                return;

            self.inc_strong(2);
            self.delink();
            self.next       = std::addressof(other);
            self.prev       = other.prev;
            self.prev->next = std::addressof(self);
            other.prev      = std::addressof(self);
        }

        void insert_after(this T& self, T& other) noexcept
        {
            if (std::addressof(self) == std::addressof(other))
                return;

            self.inc_strong(2);
            self.delink();
            self.prev       = std::addressof(other);
            self.next       = other.next;
            self.next->prev = std::addressof(self);
            other.next      = std::addressof(self);
        }

    protected:
        friend ptr<T>;
        friend ptr<T const>;
        friend ref<T>;
        friend ref<T const>;
        friend list<T>;
        friend __::list_iterator<T, false, false>;
        friend __::list_iterator<T, true, false>;
        friend __::list_iterator<T, false, true>;
        friend __::list_iterator<T, true, true>;

        node() noexcept
        {
            reinterpret_cast<T*>(this)->init();
        }

        void init(this T& self) noexcept
        {
            self.prev = std::addressof(self);
            self.next = std::addressof(self);
        }

        void delink(this T& self) noexcept
        {
            if (auto s = std::addressof(self); self.next != s && self.prev != s)
            {
                self.prev->next = self.next;
                self.next->prev = self.prev;
                self.best_dec_strong(2);
            }
        }

        template<typename S>
        __::control_block<S>* make_weak_ref(this S& self) noexcept
        {
            return self.count.make_weak_ref(std::addressof(self));
        }

        void inc_strong(this node<T> const& self, size_t count = 1) noexcept
        {
            self.count.inc_strong(count);
        }

        bool inc_strong_nz(this node<T> const& self, size_t count = 1) noexcept
        {
            return self.count.inc_strong_nz(count);
        }

        template<typename S>
        void best_dec_strong(this S& self, size_t count = 1) noexcept
        {
            if (self.count.dec_strong(count) == 0)
            {
                if constexpr (!std::is_const_v<S>)
                    delete std::addressof(self);
            }
        }

        template<typename S>
        void safe_dec_strong(this S& self, size_t count = 1) noexcept
        {
            if (std::addressof(self) == nullptr)
                return;
            self.best_dec_strong(count);
        }

        __::refcount count;
        T*           prev;
        T*           next;
    };
} // namespace ljh::smarc
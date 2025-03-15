
//          Copyright Jared Irwin 2024-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// smarc/list.hpp - v1.0
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
#include "ptr.hpp"
#include <concepts>

namespace ljh::smarc
{
    namespace __
    {
        template<typename T, bool is_const, bool reverse>
        struct /*[[clang::preferred_name(ljh::list<T>::iterator), clang::preferred_name(ljh::list<T>::const_iterator),
                 clang::preferred_name(ljh::list<T>::reverse_iterator), clang::preferred_name(ljh::list<T>::const_reverse_iterator)]]*/
            list_iterator
        {
            using difference_type = std::ptrdiff_t;
            using element_type    = std::conditional_t<is_const, T const, T>;
            using pointer         = element_type*;
            using reference       = element_type&;

            list_iterator() noexcept
            {
                // This should not be reached
                std::abort();
            }

            list_iterator(list_iterator const& other) noexcept
            {
                (prev = other.prev)->inc_strong();
                (curr = other.curr)->inc_strong();
                (next = other.next)->inc_strong();
            }

            list_iterator& operator=(list_iterator const& other) noexcept
            {
                (prev = other.prev)->inc_strong();
                (curr = other.curr)->inc_strong();
                (next = other.next)->inc_strong();
            }

            ~list_iterator()
            {
                std::exchange(prev, nullptr)->safe_dec_strong();
                std::exchange(curr, nullptr)->safe_dec_strong();
                std::exchange(next, nullptr)->safe_dec_strong();
            }

            reference operator*() const noexcept
            {
                return *curr;
            }

            list_iterator& operator++() noexcept
            {
                advance<!reverse>();
                return *this;
            }

            list_iterator& operator--() noexcept
            {
                advance<reverse>();
                return *this;
            }

            list_iterator operator++(int) noexcept
            {
                auto tmp = *this;
                ++(*this);
                return tmp;
            }

            list_iterator operator--(int) noexcept
            {
                auto tmp = *this;
                --(*this);
                return tmp;
            }

            friend bool operator==(list_iterator const& lhs, list_iterator const& rhs) noexcept
            {
                return lhs.curr == rhs.curr;
            }

        private:
            friend list<T>;
            using base = element_type*;

            list_iterator(base curr) noexcept
                : prev(curr->prev)
                , curr(curr)
                , next(curr->next)
            {
                prev->inc_strong();
                curr->inc_strong();
                next->inc_strong();
            }

            template<bool forward>
            void advance() noexcept
            {
                base dead;
                if constexpr (forward)
                {
                    next->next->inc_strong();
                    curr = std::exchange(next, next->next);
                    dead = std::exchange(prev, curr->prev);
                }
                else
                {
                    prev->prev->inc_strong();
                    curr = std::exchange(prev, prev->prev);
                    dead = std::exchange(next, curr->next);
                }
                dead->best_dec_strong();
            }

            base prev = nullptr;
            base curr = nullptr;
            base next = nullptr;
        };
    } // namespace __

    /// @brief A list of strong references to smarc items
    /// @tparam T Node data type
    template<typename T>
    struct list final
    {
        using value_type             = T;
        using reference              = value_type&;
        using const_reference        = value_type const&;
        using pointer                = value_type*;
        using const_pointer          = value_type const*;
        using iterator               = __::list_iterator<value_type, false, false>;
        using const_iterator         = __::list_iterator<value_type, true, false>;
        using reverse_iterator       = __::list_iterator<value_type, false, true>;
        using const_reverse_iterator = __::list_iterator<value_type, true, true>;

        static_assert(std::bidirectional_iterator<iterator>);
        static_assert(std::bidirectional_iterator<const_iterator>);
        static_assert(std::bidirectional_iterator<reverse_iterator>);
        static_assert(std::bidirectional_iterator<const_reverse_iterator>);

        list() noexcept
        {}

        list(list const&)            = delete;
        list& operator=(list const&) = delete;

        list(list&& other)
        {
            using std::swap;
            swap(base.prev, other.base.prev);
            swap(base.next, other.base.next);
            swap(base.count, other.base.count);
        }

        list& operator=(list&& other)
        {
            using std::swap;
            swap(base.prev, other.base.prev);
            swap(base.next, other.base.next);
            swap(base.count, other.base.count);
            return *this;
        }

        ~list()
        {
            static_assert(std::derived_from<T, node<T>>);
            static_assert(std::ranges::range<list>);
            static_assert(std::ranges::common_range<list>);
            static_assert(std::ranges::range<list const>);
            static_assert(std::ranges::common_range<list const>);
            static_assert(std::ranges::constant_range<list const>);
        }

        template<typename S>
        auto& first(this S& self)
        {
            return *self.first_ptr();
        }

        template<typename S>
        auto& last(this S& self)
        {
            return *self.last_ptr();
        }

        template<typename S>
        [[nodiscard]] auto begin(this S& self) noexcept
        {
            return std::conditional_t<std::is_const_v<S>, const_iterator, iterator>{self.first_ptr()};
        }

        template<typename S>
        [[nodiscard]] auto end(this S& self) noexcept
        {
            return ++std::conditional_t<std::is_const_v<S>, const_iterator, iterator>{self.last_ptr()};
        }

        template<typename S>
        [[nodiscard]] auto rbegin(this S& self) noexcept
        {
            return std::conditional_t<std::is_const_v<S>, const_reverse_iterator, reverse_iterator>{self.first_ptr()};
        }

        template<typename S>
        [[nodiscard]] auto rend(this S& self) noexcept
        {
            return ++std::conditional_t<std::is_const_v<S>, const_reverse_iterator, reverse_iterator>{self.last_ptr()};
        }

        [[nodiscard]] const_iterator cbegin() const noexcept
        {
            return begin();
        }

        [[nodiscard]] const_iterator cend() const noexcept
        {
            return end();
        }

        [[nodiscard]] const_reverse_iterator crbegin() const noexcept
        {
            return begin();
        }

        [[nodiscard]] const_reverse_iterator crend() const noexcept
        {
            return end();
        }

    private:
        template<typename S>
        using correct_pointer = std::conditional_t<std::is_const_v<S>, T const, T>*;

        template<typename S>
        correct_pointer<S> first_ptr(this S& self) noexcept
        {
            return self.base.next;
        }

        template<typename S>
        correct_pointer<S> last_ptr(this S& self) noexcept
        {
            return self.base.prev;
        }

        node<T> base;
    };
} // namespace ljh::smarc

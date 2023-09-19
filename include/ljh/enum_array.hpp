
//          Copyright Jared Irwin 2023-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// cpp_version.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++17
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "cpp_version.hpp"
#include <array>

namespace ljh
{
    template<typename T, typename N, std::size_t E = (std::size_t)N::END + 1>
    struct enum_array
    {
        std::array<T, E> items;

        using value_type             = typename decltype(items)::value_type;
        using pointer                = typename decltype(items)::pointer;
        using const_pointer          = typename decltype(items)::const_pointer;
        using reference              = typename decltype(items)::reference;
        using const_reference        = typename decltype(items)::const_reference;
        using iterator               = typename decltype(items)::iterator;
        using const_iterator         = typename decltype(items)::const_iterator;
        using size_type              = typename decltype(items)::size_type;
        using difference_type        = typename decltype(items)::difference_type;
        using reverse_iterator       = typename decltype(items)::reverse_iterator;
        using const_reverse_iterator = typename decltype(items)::const_reverse_iterator;

        [[nodiscard]] constexpr bool empty() const noexcept
        {
            return false;
        }

        [[nodiscard]] constexpr size_type size() const noexcept
        {
            return E;
        }

        [[nodiscard]] constexpr size_type max_size() const noexcept
        {
            return E;
        }

        LJH_CPP17_CONSTEXPR T* data() noexcept
        {
            return items.data();
        }

        LJH_CPP17_CONSTEXPR T const* data() const noexcept
        {
            return items.data();
        }

        LJH_CPP17_CONSTEXPR reference operator[](N pos)
        {
            return items[(std::size_t)pos];
        }

        LJH_CPP17_CONSTEXPR const_reference operator[](N pos) const
        {
            return items[(std::size_t)pos];
        }

        LJH_CPP17_CONSTEXPR iterator begin() noexcept
        {
            return items.begin();
        }

        LJH_CPP17_CONSTEXPR const_iterator begin() const noexcept
        {
            return items.begin();
        }

        LJH_CPP17_CONSTEXPR const_iterator cbegin() const noexcept
        {
            return items.cbegin();
        }

        LJH_CPP17_CONSTEXPR iterator end() noexcept
        {
            return items.end();
        }

        LJH_CPP17_CONSTEXPR const_iterator end() const noexcept
        {
            return items.end();
        }

        LJH_CPP17_CONSTEXPR const_iterator cend() const noexcept
        {
            return items.cend();
        }

        LJH_CPP17_CONSTEXPR iterator rbegin() noexcept
        {
            return items.rbegin();
        }

        LJH_CPP17_CONSTEXPR const_iterator rbegin() const noexcept
        {
            return items.rbegin();
        }

        LJH_CPP17_CONSTEXPR const_iterator crbegin() const noexcept
        {
            return items.crbegin();
        }

        LJH_CPP17_CONSTEXPR iterator rend() noexcept
        {
            return items.rend();
        }

        LJH_CPP17_CONSTEXPR const_iterator rend() const noexcept
        {
            return items.rend();
        }

        LJH_CPP17_CONSTEXPR const_iterator crend() const noexcept
        {
            return items.crend();
        }

        LJH_CPP20_CONSTEXPR void fill(T const& value)
        {
            items.fill(value);
        }
    };
} // namespace ljh

//          Copyright Jared Irwin 2021-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "coroutine_headers.hpp"
#include <utility>

namespace ljh::_co
{
    struct node_base
    {
        node_base* next;
        node_base* prev;
    };

    struct node_handle : node_base
    {
        std::coroutine_handle<> handle{};
    };

    template<typename Extra>
    struct node : node_handle
    {
        template<typename... Args>
        node(Args&&... args)
            : extra(std::forward<Args>(args)...)
        {}

        Extra extra;
    };
} // namespace ljh::_co

namespace ljh::co
{
    struct node_list : _co::node_base
    {
        node_list()
            : _co::node_base{this, this}
        {}

        node_list(node_list const&)      = delete;
        void operator=(node_list const&) = delete;

        bool empty() const noexcept
        {
            return next == this;
        }

        void append_node(_co::node_base& node) noexcept
        {
            node.next  = this;
            node.prev  = prev;
            prev->next = std::addressof(node);
            prev       = std::addressof(node);
        }

        bool append_list(node_list& other) noexcept
        {
            if (other.empty())
                return false;
            other.prev->next = next;
            next->prev       = other.prev;
            next             = other.next;
            next->prev       = this;
            other.next = other.prev = std::addressof(other);
            return true;
        }

        _co::node_base* peek_head() const noexcept
        {
            return empty() ? nullptr : next;
        }

        _co::node_base* try_remove_head() noexcept
        {
            if (empty())
                return nullptr;
            auto node  = next;
            next       = node->next;
            next->prev = this;
            return node;
        }
    };
} // namespace ljh::co
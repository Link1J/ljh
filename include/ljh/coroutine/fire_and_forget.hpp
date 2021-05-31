
//          Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "coroutine_headers.hpp"
#include "../type_traits.hpp"

namespace ljh::co
{
	struct fire_and_forget;

	template <typename T>
	fire_and_forget no_await(T t)
	{
		if constexpr (std::is_invocable_v<T>)
			co_await t();
		else
			co_await t;
	}
}

template <typename... Args>
struct std::coroutine_traits<ljh::co::fire_and_forget, Args...>
{
	struct promise_type
	{
		winrt::fire_and_forget get_return_object() const noexcept
		{
			return {};
		}

		void return_void() const noexcept
		{
		}

		suspend_never initial_suspend() const noexcept
		{
			return {};
		}

		suspend_never final_suspend() const noexcept
		{
			return {};
		}

		void unhandled_exception() const noexcept
		{
			winrt::terminate();
		}
	};
};
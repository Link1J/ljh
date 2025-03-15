
//          Copyright Jared Irwin 2021-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "task_base.hpp"

namespace ljh::__::co
{
	template<typename T>
	struct cold_promise_awaiter
	{
		promise_ptr<T> self;

		bool await_ready()
		{
			return self->template client_await_ready<true>();
		}

		auto await_suspend(std::coroutine_handle<> handle)
		{
			return self->template client_await_suspend<true>(handle.address());
		}

		T await_resume()
		{
			return self->client_await_resume();
		}
	};
}

namespace ljh::co
{
	template<typename T>
	struct cold_task : __::co::task_base<T>
	{
		using base = __::co::task_base<T>;
		cold_task(__::co::promise<T>* initial = nullptr)
			: base(initial)
		{
			this->_promise->start();
		}

		void swap(cold_task& other)
		{
			std::swap(this->_promise, other._promise);
		}

		using base::operator co_await;

		auto operator co_await()&&
		{
			return __::co::cold_promise_awaiter<T>{ std::move(this->_promise) };
		}
	};

	template<typename T>
	void swap(cold_task<T>& left, cold_task<T>& right)
	{
		left.swap(right);
	}
}

template <typename T, typename... Args>
struct std::coroutine_traits<ljh::co::cold_task<T>, Args...>
{
	using promise_type = ljh::__::co::cold_promise_awaiter<T>;
};
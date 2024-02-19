
//          Copyright Jared Irwin 2021-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "promise.hpp"

namespace ljh::__::co
{
	template<typename T>
	struct task_base
	{
		task_base(promise<T>* initial = nullptr) noexcept : promise(initial) { }

		struct cannot_await_lvalue_use_std_move {};
		cannot_await_lvalue_use_std_move operator co_await() & = delete;

		T get()&&
		{
			if (!promise->client_await_ready()) {
				bool completed = false;
				if (promise->client_await_suspend(&completed, wake_by_address)) {
					bool ready = true;
					while (!completed) {
						WaitOnAddress(&completed, &ready, sizeof(completed), INFINITE);
					}
				}
			}
			return std::exchange(promise, {})->client_await_resume();
		}

	protected:
		promise_ptr<T> promise;

		static void __stdcall wake_by_address(void* completed)
		{
			*reinterpret_cast<bool*>(completed) = true;
			WakeByAddressSingle(completed);
		}
	};
}

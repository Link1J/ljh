
//          Copyright Jared Irwin 2021-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "task_base.hpp"

#include <winrt/base.h>
#include <ctxtcall.h>

namespace ljh::_co
{
	template<typename T>
	struct com_promise_awaiter
	{
		com_promise_awaiter(promise_ptr<T>&& ptr)
			: self(std::move(ptr))
		{
		}

		promise_ptr<T> self;
		std::experimental::coroutine_handle<> waiter;
		winrt::com_ptr<IContextCallback> context;

		bool await_ready()
		{
			return self->client_await_ready();
		}

		auto await_suspend(std::experimental::coroutine_handle<> handle)
		{
			waiter = handle;
			winrt::check_hresult(CoGetObjectContext(IID_PPV_ARGS(&context)));
			return self->client_await_suspend(this, resume_in_context);
		}

		T await_resume()
		{
			return self->client_await_resume();
		}

		static auto as_self(void* p)
		{
			return reinterpret_cast<com_promise_awaiter*>(p);
		}

		static void resume_in_context(void* parameter)
		{
			as_self(parameter)->resume_context();
		}

		void resume_context()
		{
			ComCallData data{};
			data.pUserDefined = this;
			auto local_context = std::move(context);
			winrt::check_hresult(local_context->ContextCallback(resume_apartment_callback, &data, IID_ICallbackWithNoReentrancyToApplicationSTA, 5, nullptr));
		}

		static HRESULT CALLBACK resume_apartment_callback(ComCallData* data) noexcept
		{
			as_self(data->pUserDefined)->waiter();
			return S_OK;
		}
	};
}

namespace ljh::co
{
	template<typename T>
	struct com_aware_task : _co::task_base<T>
	{
		using base = _co::task_base<T>;
		com_aware_task() = default;
		com_aware_task(_co::promise<T>* initial)
			: base(initial)
		{
			this->promise->start();
		}

		void swap(com_task& other)
		{
			std::swap(this->promise, other.promise);
		}

		using base::operator co_await;

		auto operator co_await()&&
		{
			return _co::com_promise_awaiter<T>{ std::move(promise) };
		}
	};

	template<typename T>
	void swap(com_aware_task<T>& left, com_aware_task<T>& right)
	{
		left.swap(right);
	}
}

template <typename T, typename... Args>
struct std::coroutine_traits<ljh::co::com_aware_task<T>, Args...>
{
	using promise_type = ljh::_co::promise<T>;
};
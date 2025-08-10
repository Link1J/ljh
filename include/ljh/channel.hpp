
//          Copyright Jared Irwin 2025-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "coroutine/coroutine_headers.hpp"

#include <exception>
#include <type_traits>
#include <memory>
#include <stack>
#include <utility>

namespace ljh
{
	LJH_MODULE_COROUTINE_EXPORT template<typename R, typename S>
	struct channel
	{
		using _value     = std::remove_cvref_t<R>;
		using _reference = R&&;
		using _rref      = std::conditional_t<std::is_lvalue_reference_v<_reference>, std::remove_reference_t<_reference>&&, _reference>;
		static_assert(std::common_reference_with<_reference&&, _value&>);
		static_assert(std::common_reference_with<_reference&&, _rref&&>);
		static_assert(std::common_reference_with<_rref&&, _value const&>);

		using yielded = std::conditional_t<std::is_reference_v<_reference>, _reference, _reference const&>;

		struct awaiter;
		struct promise_type
		{
			std::add_pointer_t<yielded> ptr = nullptr;
			S                           input;

			auto initial_suspend() noexcept
			{
				return std::suspend_never();
			}
			auto final_suspend() noexcept
			{
				return std::suspend_always();
			}
			void unhandled_exception() noexcept
			{
				std::terminate();
			}

			auto yield_value(yielded value)
			{
				ptr = std::addressof(value);
				return awaiter(this);
			}

			[[nodiscard]] channel get_return_object() noexcept
			{
				return channel{std::coroutine_handle<promise_type>::from_promise(*this)};
			}
			
			void return_void() const noexcept
			{}
		};
		struct awaiter : std::suspend_always
		{
			friend promise_type;
			constexpr S await_resume() const
			{
				return m_p->input;
			}

		private:
			constexpr awaiter(promise_type* p)
				: m_p(p)
			{}
			promise_type* m_p;
		};

		std::coroutine_handle<promise_type> coroutine = nullptr;

		explicit channel(std::coroutine_handle<promise_type> coroutine) noexcept
			: coroutine(coroutine)
		{}

		channel(channel const&) = delete;
		channel(channel&& other) noexcept
			: coroutine(::std::exchange(other.coroutine, {}))
		{}

		channel& operator=(channel other) noexcept
		{
			::std::swap(coroutine, other.coroutine);
			return *this;
		}

		~channel()
		{
			if (coroutine)
				coroutine.destroy();
			coroutine = nullptr;
		}

		R receive()
		{
			return static_cast<R>(*coroutine.promise().ptr);
		}

		void send(S value)
		{
			coroutine.promise().input = value;
			coroutine.resume();
		}

		bool done() const noexcept
		{
			return coroutine.done();
		}
	};
} // namespace ljh
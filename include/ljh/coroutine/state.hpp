
//          Copyright Jared Irwin 2021-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "node_list.hpp"
#include <mutex>

namespace ljh::co
{
	// Specialize if you need extra data for co_await.
	template <typename State>
	struct extra_await_data
	{
	};

	template <typename State>
	class state
	{
		std::mutex mutex;
		node_list sentinel;

		State& parent() { return static_cast<State&>(*this); }

		static auto& extra_node(_co::node_base& node)
		{
			return static_cast<_co::node<extra_await_data>&>(node);
		}

		static auto* extra_node(_co::node_base* node)
		{
			return static_cast<_co::node<extra_await_data>*>(node);
		}

	public:
		bool any_waiters() const noexcept
		{
			return !sentinel.empty();
		}

		template <bool fast, typename T, typename... Args, typename... Params>
		bool calc_claim(std::atomic<T>& value, bool (*transition)(T, T& , Params&&...), T initial, Args&&... args)
		{
			constexpr auto order = fast ? std::memory_order::acquire
										: std::memory_order::relaxed;
			for (;;)
			{
				T future;
				if (!transition(initial, future, std::forward<Args>(args)...))
				{
					return false;
				}
				auto success = value.compare_exchange_weak(initial, future, order, std::memory_order::relaxed);
				if constexpr (fast)
					return success;
				else if (success)
					return success;
			}
		}

		template <bool fast, typename T, typename... Args>
		bool calc_claim(std::atomic<T>& value, T initial, Args&&... args)
		{
			return calc_claim<fast>(value,& State::transition, initial, std::forward<Args>(args)...);
		}

		template <bool fast, typename T, typename... Args, typename... Params>
		bool calc_claim(std::atomic<T>& value, bool (*transition)(T, T& , Params&&...), Args&&... args)
		{
			return calc_claim<fast>(value, transition, value.load(std::memory_order::relaxed), std::forward<Args>(args)...);
		}

		template <bool fast, typename T>
		bool calc_claim(std::atomic<T>& value)
		{
			return calc_claim<fast>(value, &State::transition, value.load(std::memory_order::relaxed));
		}

		using extra_await_data = extra_await_data<State>;
		using node_list = node_list;

		bool fast_claim(extra_await_data const&) const noexcept
		{
			return false;
		}

		extra_await_data* peek_head() const noexcept
		{
			auto node = sentinel.peek_head();
			return node ?& extra_node(node)->extra : nullptr;
		}

		bool resume_one(node_list& list) noexcept
		{
			auto node = sentinel.try_remove_head();
			if (!node)
				return false;
			list.append_node(*node);
			return true;
		}

		bool resume_all(node_list& list) noexcept
		{
			return list.append_list(sentinel);
		}

		bool await_suspend(std::coroutine_handle<> handle, _co::node<extra_await_data>& node)
		{
			auto guard = std::lock_guard(mutex);
			if (parent().claim(node.extra))
				return false;
			node.handle = handle;
			sentinel.append_node(node);
			return true;
		}

		void get_result() const noexcept {}

		decltype(auto) await_resume(_co::node<extra_await_data>& node) noexcept
		{
			node.handle = nullptr;
			return parent().get_result();
		}

		void destruct_node(_co::node_handle& node) noexcept
		{
			if (node.handle)
			{
				auto guard = std::lock_guard(mutex);
				node.next = node.prev->next;
				node.prev = node.next->prev;
			}
		}

		template <typename... Params, typename... Args>
		void action_impl(void (State::*handler)(node_list& , Params...), Args&&... args)
		{
			node_list list;
			{
				std::lock_guard guard(mutex);
				(parent().*handler)(list, std::forward<Args>(args)...);
			}
			resume_list(list);
		}

	private:
		void resume_list(node_list& list)
		{
			auto node = list.next;
			while (node != std::addressof(list))
			{
				resume_node(std::exchange(node, node->next));
			}
		}

		void resume_node(_co::node_base* node) noexcept
		{
			extra_node(*node).handle.resume();
		}
	};

	template <typename State>
	class sync_object
	{
		std::shared_ptr<State> shared;

	public:
		template <typename... Args>
		sync_object(Args&&... args)
			: shared(std::make_shared<State>(std::forward<Args>(args)...))
		{
		}

		template <typename Arg = typename State::extra_await_data>
		auto make_awaiter(Arg arg = {})
		{
			return awaiter{*shared, std::forward<Arg>(arg)};
		}

		auto operator co_await() { return awaiter{*shared}; }

	protected:
		using state = State;

		State& get_state() const noexcept { return *shared; }

		template <typename... Args>
		void action_impl(Args&&... args) const
		{
			shared->action_impl(std::forward<Args>(args)...);
		}

		struct awaiter
		{
			template <typename... Args>
			awaiter(State& state, Args&&... args)
				: s(state)
				, node(std::forward<Args>(args)...)
			{
			}

			State& s;
			_co::node<extra_await_data<State>> node;

			bool await_ready()
			{
				return s.fast_claim(node.extra);
			}

			bool await_suspend(std::coroutine_handle<> handle)
			{
				return s.await_suspend(handle, node);
			}

			decltype(auto) await_resume()
			{
				return s.await_resume(node);
			}

			~awaiter() { s.destruct_node(node); }
		};
	};
}
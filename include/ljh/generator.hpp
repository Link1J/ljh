
//          Copyright Jared Irwin 2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "coroutine/coroutine_headers.hpp"
#include "ranges/elements_of.hpp"

#include <exception>
#include <type_traits>
#include <memory>
#include <stack>
#include <utility>

namespace ljh
{
	template<typename R, typename V = void>
	struct generator;
}

namespace ljh::__
{
	template<typename yielded>
	struct promise_base;

	template<typename yielded>
	using coroutine_handle = std::coroutine_handle<promise_base<yielded>>;

	template<typename yielded>
	struct nesting_info
	{
		std::exception_ptr        except;
		coroutine_handle<yielded> parent;
		coroutine_handle<yielded> root;
	};

	template<typename yielded>
	struct element_awaiter : std::suspend_always
	{
		std::remove_cvref_t<yielded> val;

		element_awaiter(std::remove_reference_t<yielded> const& val)
			: val{val}
		{}

		template<typename promise>
		constexpr void await_suspend(std::coroutine_handle<promise> handle) noexcept
		{
			promise_base<yielded>& cur = handle.promise();
			cur.ptr                    = std::addressof(val);
		}
	};

	template<typename yielded>
	struct final_awaiter : std::suspend_always
	{
		template<typename promise>
		[[nodiscard]] std::coroutine_handle<> await_suspend(std::coroutine_handle<promise> handle) noexcept
		{
			promise_base<yielded>& cur = handle.promise();
			if (!cur.info)
				return ::std::noop_coroutine();

			coroutine_handle<yielded> cont = cur.info->parent;
			cur.info->root.promise().top   = cont;
			cur.info                       = nullptr;
			return cont;
		}
	};

	template<typename yielded, typename R, typename V>
	struct nested_awaiter
	{
		nesting_info<yielded> nested;
		ljh::generator<R, V>  gen;

		explicit nested_awaiter(generator<R, V>&& gen) noexcept
			: gen(std::move(gen))
		{}

		[[nodiscard]] bool await_ready() noexcept
		{
			return !gen.coroutine;
		}

		template<typename promise>
		[[nodiscard]] std::coroutine_handle<> await_suspend(std::coroutine_handle<promise> handle) noexcept
		{
			auto target   = coroutine_handle<yielded>::from_address(gen.coroutine.address());
			nested.parent = coroutine_handle<yielded>::from_address(handle.address());

			promise_base<yielded>& parent_promise = nested.parent.promise();
			if (parent_promise.info)
				nested.root = parent_promise.info->root;
			else
				nested.root = nested.parent;

			nested.root.promise().top = target;
			target.promise().info     = ::std::addressof(nested);
			return target;
		}

		void await_resume()
		{
			if (nested.except)
				::std::rethrow_exception(::std::move(nested.except));
		}
	};

	template<typename yielded>
	struct promise_base
	{
		using coroutine_handle = coroutine_handle<yielded>;
		using nesting_info     = nesting_info<yielded>;
		using element_awaiter  = element_awaiter<yielded>;
		using final_awaiter    = final_awaiter<yielded>;
		template<typename R, typename V>
		using nested_awaiter = nested_awaiter<yielded, R, V>;

		coroutine_handle            top  = coroutine_handle::from_promise(*this);
		std::add_pointer_t<yielded> ptr  = nullptr;
		nesting_info*               info = nullptr;

		[[nodiscard]] std::suspend_always initial_suspend() const noexcept
		{
			return {};
		}

		auto final_suspend() noexcept
		{
			return final_awaiter{};
		}

		std::suspend_always yield_value(yielded val) noexcept
		{
			ptr = std::addressof(val);
			return {};
		}

		auto yield_value(std::remove_reference_t<yielded> const& val)
			requires std::is_rvalue_reference_v<yielded> && std::constructible_from<std::remove_cvref_t<yielded>, std::remove_reference_t<yielded> const&>
		{
			return element_awaiter{val};
		}

		template<typename R2, typename V2>
			requires std::same_as<typename generator<R2, V2>::yielded, yielded>
		auto yield_value(ljh::ranges::elements_of<generator<R2, V2>&&> g) noexcept
		{
			return nested_awaiter<R2, V2>{std::move(g.range)};
		}

		void await_transform() = delete;

		void return_void() const noexcept
		{}

		void unhandled_exception()
		{
			if (info)
				info->except = ::std::current_exception();
			else
				throw;
		}
	};

	template<typename R>
	using yielded = std::conditional_t<std::is_reference_v<R>, R, R const&>;

	template<typename V, typename R>
	struct iterator
	{
		coroutine_handle<yielded<R>> coroutine;

		using value_type      = V;
		using difference_type = std::ptrdiff_t;

		explicit iterator(coroutine_handle<yielded<R>> coroutine) noexcept
			: coroutine{coroutine}
		{}

		iterator(iterator&& other) noexcept
			: coroutine(std::exchange(other.coroutine, {}))
		{}

		iterator& operator=(iterator&& other) noexcept
		{
			coroutine = std::exchange(other.coroutine, {});
			return *this;
		}

		R operator*() const noexcept(std::is_nothrow_copy_constructible_v<R>)
		{
			return static_cast<R>(*coroutine.promise().top.promise().ptr);
		}

		iterator& operator++()
		{
			coroutine.promise().top.resume();
			return *this;
		}

		void operator++(int)
		{
			++*this;
		}

		friend bool operator==(iterator const& i, std::default_sentinel_t)
		{
			return i.coroutine.done();
		}
	};
} // namespace ljh::__

namespace ljh
{
	template<typename R, typename V>
	struct generator : public std::ranges::view_interface<generator<R, V>>
	{
		using _value     = std::conditional_t<std::is_void_v<V>, std::remove_cvref_t<R>, V>;
		using _reference = std::conditional_t<std::is_void_v<V>, R&&, R>;
		using _rref      = std::conditional_t<std::is_lvalue_reference_v<_reference>, std::remove_reference_t<_reference>&&, _reference>;
		static_assert(std::common_reference_with<_reference&&, _value&>);
		static_assert(std::common_reference_with<_reference&&, _rref&&>);
		static_assert(std::common_reference_with<_rref&&, _value const&>);

		using yielded  = __::yielded<_reference>;
		using iterator = __::iterator<_value, _reference>;

		struct promise_type : __::promise_base<yielded>
		{
			[[nodiscard]] generator get_return_object() noexcept
			{
				return generator{std::coroutine_handle<promise_type>::from_promise(*this)};
			}
		};

		std::coroutine_handle<promise_type> coroutine = nullptr;

		explicit generator(std::coroutine_handle<promise_type> coroutine) noexcept
			: coroutine(coroutine)
		{}

		generator(generator const&) = delete;
		generator(generator&& other) noexcept
			: coroutine(::std::exchange(other.coroutine, {}))
		{}

		generator& operator=(generator other) noexcept
		{
			::std::swap(coroutine, other.coroutine);
			return *this;
		}

		~generator()
		{
			if (coroutine)
				coroutine.destroy();
			coroutine = nullptr;
		}

		iterator begin()
		{
			coroutine.resume();
			return iterator{__::coroutine_handle<yielded>::from_address(coroutine.address())};
		}

		[[nodiscard]] std::default_sentinel_t end() const noexcept
		{
			return std::default_sentinel;
		}
	};
} // namespace ljh
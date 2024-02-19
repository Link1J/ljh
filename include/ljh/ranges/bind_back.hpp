
//          Copyright Jared Irwin 2020-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// terminators.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++14
// 
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "../cpp_version.hpp"

#if __cpp_lib_bind_back <= 202202L
#include <tuple>
#include <type_traits>

namespace ljh::__
{
	template<typename F, typename... A>
	struct binder_back
	{
		template<typename FI, typename... AI, typename = typename std::enable_if<std::is_constructible<F, FI>::value>::type>
		constexpr explicit binder_back(FI&& func, AI&&... args) 
			: __func{std::forward<FI>(func)}
			, __args{std::forward<AI>(args)...}
		{}

		template<typename... A2, typename = typename std::enable_if<std::is_invocable<F, A2..., A...>::value>::type>
		constexpr auto operator()(A2&&... args2) & noexcept(std::is_nothrow_invocable<F, A2..., A...>::value)
		{
			return std::apply(__func, std::tuple_cat(std::forward_as_tuple(std::forward<A2>(args2)...), __args));
		}

		template<typename... A2, typename = typename std::enable_if<std::is_invocable<F, A2..., A...>::value>::type>
		constexpr auto operator()(A2&&... args2) const& noexcept(std::is_nothrow_invocable<F, A2..., A...>::value)
		{
			return std::apply(__func, std::tuple_cat(std::forward_as_tuple(std::forward<A2>(args2)...), __args));
		}

		template<typename... A2, typename = typename std::enable_if<std::is_invocable<F, A2..., A...>::value>::type>
		constexpr auto operator()(A2&&... args2) && noexcept(std::is_nothrow_invocable<F, A2..., A...>::value)
		{
			return std::apply(std::move(__func), std::tuple_cat(std::forward_as_tuple(std::forward<A2>(args2)...), std::move(__args)));
		}

		template<typename... A2, typename = typename std::enable_if<std::is_invocable<F, A2..., A...>::value>::type>
		constexpr auto operator()(A2&&... args2) const&& noexcept(std::is_nothrow_invocable<F, A2..., A...>::value)
		{
			return std::apply(std::move(__func), std::tuple_cat(std::forward_as_tuple(std::forward<A2>(args2)...), std::move(__args)));
		}

		F                __func;
		std::tuple<A...> __args;
	};

	template<typename F, typename... A>
	struct can_bind_back : std::bool_constant<std::is_constructible<typename std::decay<F>::type, F>::value && std::is_move_constructible<typename std::decay<F>::type>::value && (std::is_constructible<typename std::decay<A>::type, A>::value && ...) && (std::is_move_constructible<typename std::decay<A>::type>::value && ...)>
	{};
}

namespace ljh
{
	template<typename F, typename... A, typename = typename std::enable_if<__::can_bind_back<F, A...>::value>::type>
	[[nodiscard]] constexpr auto bind_back(F&& f, A&&... args)
	{
		return __::binder_back<typename std::decay<F>::type, typename std::decay<A>::type...>{std::forward<F>(f), std::forward<A>(args)...};
	}
}
#else
#include <functional>

namespace ljh
{
	using std::bind_back;
}
#endif


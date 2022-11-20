
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// utility.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++20
// 
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include "../cpp_version.hpp"
#include "bind_back.hpp"

#include <ranges>
#include <concepts>

namespace ljh::ranges
{
#if __cpp_lib_ranges <= 202202L
	template<typename D>
		requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
	struct range_adaptor_closure;
#else
	template<typename D>
	using range_adaptor_closure = std::ranges::range_adaptor_closure<D>;
#endif

	template<typename t>
	concept is_range_adaptor_closure = std::derived_from<std::remove_cvref_t<t>, range_adaptor_closure<std::remove_cvref_t<t>>>;

	template<typename L, typename R>
	concept can_pipe = requires(L&& l, R&& r) { static_cast<R&&>(r)(static_cast<L&&>(l)); };

	template<typename L, typename R>
	concept can_compose = std::constructible_from<std::remove_cvref_t<L>, L> && std::constructible_from<std::remove_cvref_t<R>, R>;

#if __cpp_lib_ranges <= 202202L
	template<typename Left, typename Right>
	struct pipeline : range_adaptor_closure<pipeline<Left, Right>>
	{
		template<typename T1, typename T2>
		constexpr explicit pipeline(T1&& f1, T2&& f2) 
			noexcept(std::is_nothrow_convertible_v<T1, Left>&& std::is_nothrow_convertible_v<T2, Right>)
			: l(std::forward<T1>(f1))
			, r(std::forward<T2>(f2))
		{}

		template<typename T>
		[[nodiscard]] constexpr auto operator()(T&& val) 
			noexcept(noexcept(r(l(std::forward<T>(val))))) 
			requires requires { r(l(std::forward<T>(val))); }
		{
			return r(l(std::forward<T>(val)));
		}

		template<typename T>
		[[nodiscard]] constexpr auto operator()(T&& val) const 
			noexcept(noexcept(r(l(std::forward<T>(val))))) 
			requires requires { r(l(std::forward<T>(val))); }
		{
			return r(l(std::forward<T>(val)));
		}

	private:
		[[no_unique_address]] Left  l;
		[[no_unique_address]] Right r;
	};

	template<typename T1, typename T2>
	pipeline(T1, T2) -> pipeline<T1, T2>;

	template<typename D>
		requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
	struct range_adaptor_closure
	{
		template<std::ranges::viewable_range V, is_range_adaptor_closure C>
			requires std::same_as<D, std::remove_cvref_t<C>> && std::invocable<C, V>
		[[nodiscard]] friend constexpr decltype(auto) operator|(V&& view, C&& closure) noexcept(std::is_nothrow_invocable_v<C, V>)
		{
			return std::invoke(std::forward<C>(closure), std::forward<V>(view));
		}

		template<is_range_adaptor_closure C1, is_range_adaptor_closure C2>
			requires std::same_as<D, std::remove_cvref_t<C1>> && std::constructible_from<std::decay_t<C1>, C1> && std::constructible_from<std::decay_t<C2>, C2>
		[[nodiscard]] friend constexpr auto operator|(C1&& c1, C2&& c2) 
			noexcept(std::is_nothrow_constructible_v<std::decay_t<C1>, C1> && std::is_nothrow_constructible_v<std::decay_t<C2>, C2>)
		{
			return pipeline(std::forward<C2>(c2), std::forward<C1>(c1));
		}
	};
#endif

	template <typename F>
	class closure : public range_adaptor_closure<closure<F>>
	{
		F f;

	public:
		constexpr closure(F f) : f(f) { }

		template <std::ranges::viewable_range R>
			requires std::invocable<F const&, R>
		constexpr auto operator()(R&& r) const
		{
			return f(std::forward<R>(r));
		}
	};

	template <typename F>
	class adaptor
	{
		F f;

	public:
		constexpr adaptor(F f) : f(f) { }

		template <typename... Args>
		constexpr auto operator()(Args&&... args) const
		{
			if constexpr (std::invocable<F const&, Args...>)
				return f(std::forward<Args>(args)...);
			else
				return closure(ljh::bind_back(f, std::forward<Args>(args)...));
		}
	};
}
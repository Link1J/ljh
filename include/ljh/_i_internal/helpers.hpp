
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <iterator>

#if __cpp_constexpr_dynamic_alloc >= 201907L
#define _i_LJH_CONSTEXPR_CONTAINER constexpr
#else
#define _i_LJH_CONSTEXPR_CONTAINER
#endif

namespace ljh
{
	using size_t = decltype(sizeof(int));

	template<typename T>
	static constexpr auto _i_round_to_power2(T value) -> T
	{
		T power = 1;
		while(power < value)
			power *= 2;
		return power;
	}

	template<class T> struct _i_remove_ref      {typedef T type;};
	template<class T> struct _i_remove_ref<T& > {typedef T type;};
	template<class T> struct _i_remove_ref<T&&> {typedef T type;};

	template<typename T>
	static constexpr typename _i_remove_ref<T>::type&& _i_move(T&& t) noexcept
	{
		return static_cast<typename _i_remove_ref<T>::type&&>(t);
	}

	template<typename T>
	static constexpr void _i_swap(T& l, T& r) noexcept
	{
		auto t = _i_move(l);
		     l = _i_move(r);
		     r = _i_move(t);
	}
}
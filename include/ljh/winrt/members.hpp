
//          Copyright Jared Irwin 2023-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// members.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++20
// Requires WinRT
// 
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include <winrt/base.h>
#include <type_traits>

namespace ljh::winrt
{
	template<typename T>
	constexpr T empty_value() noexcept
	{
		if constexpr (std::is_base_of_v<::winrt::Windows::Foundation::IUnknown, T>)
			return nullptr;
		else 
			return {};
	}

	template<typename T>
	struct property
	{
		property(T const& initial = empty_value<T>())
			: value(initial) 
		{}

		T operator()() const { return value; }
		void operator()(T const& newValue) { value = newValue; }

		T value;
	};

	template<typename T>
	struct property<::winrt::com_array<T>>
	{
		property() = default;
		property(::winrt::com_array<T>&& initial)
			: value(std::move(initial)) 
		{}

		::winrt::com_array<T>& operator()() { return value; }
		::winrt::com_array<T> const& operator()() const { return value; }
		void operator()(::winrt::com_array<T> const& newValue) { value = newValue; }
		void operator()(::winrt::array_view<const T> const& newValue) { value = ::winrt::com_array<T>{std::begin(newValue), std::end(newValue)}; }

		::winrt::com_array<T> value;
	};

	template<typename D>
	struct event : ::winrt::event<D>
	{
	    event() = default;

	    using ::winrt::event<D>::operator();
	    auto operator()(D const& handler) { return this->add(handler); }
	    void operator()(::winrt::event_token const& token) { this->remove(token); }
	};
}
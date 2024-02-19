
//          Copyright Jared Irwin 2020-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// unboxing.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++20
// Requires winrt
// 
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include <ljh/function_traits.hpp>
#include <ljh/concepts.hpp>
#include <variant>

#if __has_include(<winrt/Windows.Foundation.h>)
#include <winrt/Windows.Foundation.h>

#if __has_include(<Windows.Foundation.h>)
#include <Windows.Foundation.h>
#endif

namespace ljh::winrt
{
	template<typename T>
	T unbox_any(::winrt::Windows::Foundation::IInspectable const& value)
	{
		using IPV = ::winrt::Windows::Foundation::IPropertyValue;

		if constexpr (matches_return_type<T, &IPV::GetDouble>)
			return value.as<IPV>().GetDouble();
		else if constexpr (matches_return_type<T, &IPV::GetSingle>)
			return value.as<IPV>().GetSingle();
		else if constexpr (matches_return_type<T, &IPV::GetUInt64>)
			return value.as<IPV>().GetUInt64();
		else if constexpr (matches_return_type<T, &IPV::GetUInt32>)
			return value.as<IPV>().GetUInt32();
		else if constexpr (matches_return_type<T, &IPV::GetUInt16>)
			return value.as<IPV>().GetUInt16();
		else if constexpr (matches_return_type<T, &IPV::GetUInt8>)
			return value.as<IPV>().GetUInt8();
		else if constexpr (matches_return_type<T, &IPV::GetInt64>)
			return value.as<IPV>().GetInt64();
		else if constexpr (matches_return_type<T, &IPV::GetInt32>)
			return value.as<IPV>().GetInt32();
		else if constexpr (matches_return_type<T, &IPV::GetInt16>)
			return value.as<IPV>().GetInt16();
		else if constexpr (matches_return_type<T, &IPV::GetString>)
			return value.as<IPV>().GetString();
		else if constexpr (matches_return_type<T, &IPV::GetBoolean>)
			return value.as<IPV>().GetBoolean();
		else
			return unbox_value<T>(value);
	}

#if __has_include(<Windows.Foundation.h>)
	template<typename T>
	T unbox_any_or(::winrt::Windows::Foundation::IInspectable const& value, T default_value)
	{
		using IPV = ABI::Windows::Foundation::IPropertyValue;
		auto prop{value.try_as<ABI::Windows::Foundation::IPropertyValue>()};
		if (!prop && !std::is_base_of_v<::winrt::Windows::Foundation::IInspectable, T>)
			return default_value;

		if constexpr (matches_return_type<T, &IPV::GetDouble>)
			prop->GetDouble(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetSingle>)
			prop->GetSingle(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetUInt64>)
			prop->GetUInt64(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetUInt32>)
			prop->GetUInt32(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetUInt16>)
			prop->GetUInt16(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetUInt8>)
			prop->GetUInt8(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetInt64>)
			prop->GetInt64(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetInt32>)
			prop->GetInt32(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetInt16>)
			prop->GetInt16(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetString>)
			prop->GetString(&default_value);
		else if constexpr (matches_return_type<T, &IPV::GetBoolean>)
			prop->GetBoolean(&default_value);
		else
			return unbox_value_or<T>(value, default_value);

		return default_value;
	}
#endif

	inline std::variant<double, std::uint64_t, std::int64_t, ::winrt::hstring, bool> as_variant(::winrt::Windows::Foundation::IPropertyValue const& value)
	{
		switch (value.Type())
		{
		case ::winrt::Windows::Foundation::PropertyType::Int16:
		case ::winrt::Windows::Foundation::PropertyType::Int32:
		case ::winrt::Windows::Foundation::PropertyType::Int64:
			return unbox_any<int64_t>(value);

		case ::winrt::Windows::Foundation::PropertyType::UInt8:
		case ::winrt::Windows::Foundation::PropertyType::UInt16:
		case ::winrt::Windows::Foundation::PropertyType::UInt32:
		case ::winrt::Windows::Foundation::PropertyType::UInt64:
			return unbox_any<uint64_t>(value);

		case ::winrt::Windows::Foundation::PropertyType::Single:
		case ::winrt::Windows::Foundation::PropertyType::Double:
			return unbox_any<double>(value);

		case ::winrt::Windows::Foundation::PropertyType::String:
			return unbox_any<::winrt::hstring>(value);
			
		case ::winrt::Windows::Foundation::PropertyType::Boolean:
			return unbox_any<bool>(value);

		default:
			throw ::winrt::hresult_not_implemented{};
		}
	}

	inline bool check_if_equal(::winrt::Windows::Foundation::IInspectable const& lhs, ::winrt::Windows::Foundation::IInspectable const& rhs)
	{
		if (lhs == rhs)
			return true;

		auto plhs = lhs.try_as<::winrt::Windows::Foundation::IPropertyValue>();
		auto prhs = rhs.try_as<::winrt::Windows::Foundation::IPropertyValue>();

		if (!plhs || !prhs)
			return false;

		return std::visit(
			[]<typename L, typename R>(L lhs, R rhs) {
				if constexpr (!std::same_as<L, R> && (either_same_as<L, R, ::winrt::hstring> || either_same_as<L, R, bool>))
					return false;
				else
					return lhs == rhs;
			},
			as_variant(plhs), as_variant(prhs)
		);
	}

	template<typename T>
	bool check_if_equal(T const& lhs, ::winrt::Windows::Foundation::IInspectable const& rhs)
	{
		auto prhs = rhs.try_as<::winrt::Windows::Foundation::IPropertyValue>();
		if (!prhs)
			return false;
		return lhs == unbox_any<T>(prhs);
	}

	template<typename T>
	bool check_if_equal(::winrt::Windows::Foundation::IInspectable const& lhs, T const& rhs)
	{
		auto plhs = lhs.try_as<::winrt::Windows::Foundation::IPropertyValue>();
		if (!plhs)
			return false;
		return unbox_any<T>(plhs) == rhs;
	}
}
#endif
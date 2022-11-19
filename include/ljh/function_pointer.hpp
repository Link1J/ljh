
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// function_pointer.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires C++11
// 
// ABOUT
//     Adds function_pointer - like std::function but for pointers
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "function_traits.hpp"
#include "int_types.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4348)
#endif

#if LJH_CPP_VERSION >= LJH_CPP17_VERSION && __has_include(<string_view>)
#include <string_view>
#define _string_type std::string_view
#else
#include <string>
#define _string_type std::string
#endif

namespace ljh
{
	template<typename T, int = 0>
	class function_pointer;

#define POINTERS_INTERALS(CC, Noexcept)\
	template<typename R, typename... Args>\
	class function_pointer<\
		R LJH_CALLING_CONVENTION_##CC (Args...) LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept),\
		std::is_same<calling_conventions::cdecl_::type, calling_conventions::CC##_::type>::value\
			? calling_conventions::CC##_::id \
			: 0 \
	>\
	{\
	public:\
		using traits = function_traits<R LJH_CALLING_CONVENTION_##CC (Args...) LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept)>;\
		using type = typename traits::as::function_pointer;\
\
	private:\
		type function = nullptr;\
\
		template<typename T, int = sizeof(function_traits<T>)>\
		struct enable_function_pointer;\
\
		template<typename T>\
		struct enable_function_pointer<T, 1>\
		{\
			using ref = typename std::enable_if<function_traits<T>::is::function_pointer, function_pointer&>::type;\
			using boo = typename std::enable_if<function_traits<T>::is::function_pointer, bool             >::type;\
		};\
\
	public:\
		constexpr function_pointer() noexcept = default;\
		constexpr          function_pointer(type      other) noexcept : function{                       other } {}\
		constexpr explicit function_pointer(uintptr_t other) noexcept : function{reinterpret_cast<type>(other)} {}\
		constexpr explicit function_pointer(void*     other) noexcept : function{reinterpret_cast<type>(other)} {}\
		template<typename T, typename = typename enable_function_pointer<T>::ref>\
		constexpr explicit function_pointer(T         other) noexcept : function{reinterpret_cast<type>(other)} {}\
\
		function_pointer& operator=(uintptr_t other) noexcept { function = reinterpret_cast<type>(other); return *this; }\
		function_pointer& operator=(void*     other) noexcept { function = reinterpret_cast<type>(other); return *this; }\
		template<typename T> typename enable_function_pointer<T>::ref \
		                  operator=(T         other) noexcept { function = reinterpret_cast<type>(other); return *this; }\
\
		bool operator==(type           other) const noexcept { return function ==                        other ; }\
		bool operator==(uintptr_t      other) const noexcept { return function == reinterpret_cast<type>(other); }\
		bool operator==(void*          other) const noexcept { return function == reinterpret_cast<type>(other); }\
		bool operator==(std::nullptr_t      ) const noexcept { return function == nullptr                      ; }\
		template<typename T> typename enable_function_pointer<T>::boo \
		     operator==(T              other) const noexcept { return function == reinterpret_cast<type>(other); }\
\
		bool operator!=(type           other) const noexcept { return !(*this == other  ); }\
		bool operator!=(uintptr_t      other) const noexcept { return !(*this == other  ); }\
		bool operator!=(void*          other) const noexcept { return !(*this == other  ); }\
		bool operator!=(std::nullptr_t      ) const noexcept { return !(*this == nullptr); }\
		template<typename T> typename enable_function_pointer<T>::boo \
		     operator!=(T              other) const noexcept { return !(*this == other  ); }\
\
		R operator()(Args... args) const LJH_NOEXCEPT_FUNCTION_TYPE(Noexcept) { return function(args...); }\
		type get() const noexcept { return function; }\
		bool empty() const noexcept { return function == nullptr; }\
\
		         operator bool     () const noexcept { return !empty(); }\
		         operator type     () const noexcept { return            get(); }\
		explicit operator void*    () const noexcept { return (void*    )get(); }\
		explicit operator uintptr_t() const noexcept { return (uintptr_t)get(); }\
	}

#if __cpp_noexcept_function_type >= 201510L && LJH_CPP_VERSION >= LJH_CPP17_VERSION
#define MAKE_POINTERS(CC)\
	POINTERS_INTERALS(CC, false);\
	POINTERS_INTERALS(CC, true )
#else
#define MAKE_POINTERS(CC)\
	POINTERS_INTERALS(CC, false)
#endif

#ifdef cdecl
#undef cdecl
#endif

	MAKE_POINTERS(cdecl             );
	MAKE_POINTERS(stdcall           );
	MAKE_POINTERS(fastcall          );
	MAKE_POINTERS(vectorcall        );
	MAKE_POINTERS(regcall           );
	MAKE_POINTERS(ms_abi            );
	MAKE_POINTERS(sysv_abi          );
//	MAKE_POINTERS(thiscall          );

#undef MAKE_POINTERS
#undef POINTERS_INTERALS

	template<typename T>
	using funcptr = function_pointer<T>;
}

#undef _string_type

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

static_assert(sizeof(ljh::function_pointer<void()>) == sizeof(void(*)()), "ljh::function_pointer and a function pointer must be the same size");
static_assert(std::is_convertible<decltype(std::declval<ljh::function_pointer<void()>>() != nullptr), bool>::value, "ljh::function_pointer must be comparable to nullptr");
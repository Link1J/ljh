
//          Copyright Jared Irwin 2020
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


#include <ljh/function_traits.hpp>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

namespace ljh
{
	template<typename T, int = 0>
	class function_pointer;

#define POINTERS_INTERALS(CC, Noexcept)\
	template<typename R, typename... Args>\
	class function_pointer<\
		R LJH_CALLING_CONVENTION_##CC (Args...) noexcept(Noexcept),\
		std::is_same<calling_conventions::cdecl_::type, calling_conventions::CC##_::type>::value\
			? calling_conventions::CC##_::id \
			: 0 \
	>\
	{\
	public:\
		using traits = function_traits<R LJH_CALLING_CONVENTION_##CC (Args...) noexcept(Noexcept)>;\
		using type = typename traits::as::function_pointer;\
\
	private:\
		using noexcept_t = function_traits<R LJH_CALLING_CONVENTION_##CC (Args...) noexcept(!Noexcept)>;\
		type function = nullptr;\
\
	public:\
		constexpr function_pointer() noexcept = default;\
		constexpr function_pointer(uintptr_t other) noexcept : function{reinterpret_cast<type>(other)} {}\
		constexpr function_pointer(void*     other) noexcept : function{reinterpret_cast<type>(other)} {}\
\
		function_pointer& operator=(uintptr_t other) noexcept { function = reinterpret_cast<type>(other); return *this; }\
		function_pointer& operator=(void*     other) noexcept { function = reinterpret_cast<type>(other); return *this; }\
\
		bool operator==(uintptr_t other) const noexcept { return (uintptr_t)function ==        other; }\
		bool operator==(void*     other) const noexcept { return (void*    )function ==        other; }\
\
		bool operator!=(uintptr_t other) const noexcept { return !(function == other); }\
		bool operator!=(void*     other) const noexcept { return !(function == other); }\
\
		R operator()(Args... args) const noexcept(traits::is::no_exceptions) { return function(args...); }\
		type get() const noexcept { return function; }\
		bool empty() const noexcept { return function == nullptr; }\
\
		operator bool() const noexcept { return !empty(); }\
	}

#if __cpp_noexcept_function_type >= 201510L
#define MAKE_POINTERS(CC)\
	POINTERS_INTERALS(CC, false);\
	POINTERS_INTERALS(CC, true )
#else
#define MAKE_POINTERS(CC)\
	POINTERS_INTERALS(CC, false)
#endif

	MAKE_POINTERS(cdecl             );
	MAKE_POINTERS(stdcall           );
	MAKE_POINTERS(fastcall          );
	MAKE_POINTERS(vectorcall        );
	MAKE_POINTERS(regcall           );
	MAKE_POINTERS(ms_abi            );
	MAKE_POINTERS(preserve_all      );
	MAKE_POINTERS(preserve_most     );
	MAKE_POINTERS(thiscall          );
	MAKE_POINTERS(aarch64_vector_pcs);

#undef MAKE_POINTERS
#undef POINTERS_INTERALS
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

const auto a = sizeof(ljh::function_pointer<void()>) == sizeof(void(*)());

static_assert(sizeof(ljh::function_pointer<void()>) == sizeof(void(*)()), "ljh::function_pointer and a function pointer must be the same size");
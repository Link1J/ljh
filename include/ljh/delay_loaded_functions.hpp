
//          Copyright Jared Irwin 2020-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// delay_loaded_functions.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
//
// ABOUT
//     Adds delay loaded dll like things
//
// USAGE
//    Define a variable with the type of
//    `ljh::delay_load::function<dll, func, sig>`
//    replacing dll with a string will the shared lib's filename,
//    func with the name of the function, and sig with the function
//    signature (Return type, Calling convection, Parameters)
//
// Version History
//     1.0 Inital Version

#pragma once

#include "compile_time_string.hpp"
#include "concepts.hpp"
#include "os_build_info.hpp"
#include "function_traits.hpp"

namespace ljh::delay_load
{
	namespace _os
	{
#if defined(LJH_TARGET_Windows)
		using LPCSTR  = const char*;
		using HMODULE = void*      ;
		using FARPROC = void(*)()  ;
		using BOOL    = int        ;
		extern "C" {
			_os::HMODULE __stdcall LJH_OS_LoadLibraryA    (_os::LPCSTR lpLibFileName                   );
			_os::FARPROC __stdcall LJH_OS_GetProcAddress  (_os::HMODULE hModule, _os::LPCSTR lpProcName);
			_os::BOOL    __stdcall LJH_OS_FreeLibrary     (_os::HMODULE hLibModule                     );
			_os::HMODULE __stdcall LJH_OS_GetModuleHandleA(_os::LPCSTR lpModuleName                    );
		};
#if defined(_M_IX86)
#pragma comment(linker, "/alternatename:_LJH_OS_LoadLibraryA@4=_LoadLibraryA@4")
#pragma comment(linker, "/alternatename:_LJH_OS_GetProcAddress@8=_GetProcAddress@8")
#pragma comment(linker, "/alternatename:_LJH_OS_FreeLibrary@4=_FreeLibrary@4")
#pragma comment(linker, "/alternatename:_LJH_OS_GetModuleHandleA@4=_GetModuleHandleA@4")
#else
#pragma comment(linker, "/alternatename:LJH_OS_LoadLibraryA=LoadLibraryA")
#pragma comment(linker, "/alternatename:LJH_OS_GetProcAddress=GetProcAddress")
#pragma comment(linker, "/alternatename:LJH_OS_FreeLibrary=FreeLibrary")
#pragma comment(linker, "/alternatename:LJH_OS_GetModuleHandleA=GetModuleHandleA")
#endif
#define LOAD_LIB(name) ljh::delay_load::_os::LJH_OS_LoadLibraryA(name)
#define GET_FUNC(dll, name) ljh::delay_load::_os::LJH_OS_GetProcAddress(dll, name)
#define CLOSE_LIB(dll) ljh::delay_load::_os::LJH_OS_FreeLibrary(dll)
#define LOADED_LIB(name) ljh::delay_load::_os::LJH_OS_GetModuleHandleA(name)
#else
		extern "C" {
			void* dlopen (const char*, int  );
			void* dlsym  (void*, const char*);
			int   dlclose(void*             );
		};
#define LOAD_LIB(name) ljh::delay_load::_os::dlopen(name, 1)
#define GET_FUNC(dll, name) ljh::delay_load::_os::dlsym(dll, name)
#define CLOSE_LIB(dll) ljh::delay_load::_os::dlclose(dll)
#define LOADED_LIB(name) ljh::delay_load::_os::dlopen(name, 4|1)
#endif
	}

	template<compile_time_string dll_name, compile_time_string function_name, function_type type>
	struct function;

	template<compile_time_string name>
	class library
	{
		template<compile_time_string dll_name, compile_time_string function_name, function_type type>
		friend struct function;
#if defined(LJH_TARGET_Windows)
		template<compile_time_string dll_name, uint16_t ordinal_number, function_type type>
		friend struct ordinal;
#endif

		inline static void* dll = nullptr;
		
		static void* function(std::string_view func_name) noexcept
		{
			if (dll == nullptr)
				dll = LOADED_LIB(name.data());
			if (dll == nullptr)
				dll = LOAD_LIB(name.data());
			return (void*)(GET_FUNC(dll, func_name.data()));
		}
		
#if defined(LJH_TARGET_Windows)
		static void* function(uint16_t func_name) noexcept
		{
			if (dll == nullptr)
				dll = LOADED_LIB(name.data());
			if (dll == nullptr)
				dll = LOAD_LIB(name.data());
			return (void*)(GET_FUNC(dll, (_os::LPCSTR)func_name));
		}
#endif

	public:
		static void unload() noexcept
		{
			while (LOADED_LIB(name.data()))
				CLOSE_LIB(dll);
			dll = nullptr;
		}
	};

	template<compile_time_string dll_name, compile_time_string function_name, function_type type>
	struct function
	{
		using dll = library<dll_name>;
		using traits = function_traits<type>;

		template<typename... argument_types>
		std::enable_if_t<std::is_invocable_v<type, argument_types...>, typename traits::return_type>
		operator()(argument_types... args) noexcept(traits::is::no_exceptions)
		{
			return ((typename traits::as::function_pointer)(dll::function(function_name)))(args...);
		}

		bool is_loadable() const noexcept
		{
			return dll::function(function_name) != nullptr;
		}

		operator bool() const noexcept
		{
			return is_loadable();
		}
	};

#if defined(LJH_TARGET_Windows)
	template<compile_time_string dll_name, uint16_t ordinal_number, function_type type>
	struct ordinal
	{
		using dll = library<dll_name>;
		using traits = function_traits<type>;

		template<typename... argument_types>
		std::enable_if_t<std::is_invocable_v<type, argument_types...>, typename traits::return_type>
		operator()(argument_types... args) noexcept(traits::is::no_exceptions)
		{
			return ((traits::as::function_pointer)(dll::function(ordinal_number)))(args...);
		}

		bool is_loadable() const noexcept
		{
			return dll::function(ordinal_number) != nullptr;
		}

		operator bool() const noexcept
		{
			return is_loadable();
		}
	};
#endif
}

#undef LOAD_LIB
#undef GET_FUNC
#undef CLOS_LIB

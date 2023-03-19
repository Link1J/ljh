
//          Copyright Jared Irwin 2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// Based on https://learn.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code?view=vs-2022

#include "ljh/thread_name.hpp"
#include "ljh/string_utils.hpp"
#include "ljh/delay_loaded_functions.hpp"

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

namespace ljh::thread_name
{
	ljh::delay_loaded::function<"KernelBase.dll", "SetThreadDescription", HRESULT(HANDLE hThread, PCWSTR lpThreadDescription)> SetThreadDescription;

	void set_for_current(std::string const& name)
	{
		set(GetCurrentThread(), name);
	}

	void ThrowThreadName(THREADNAME_INFO info)
	{
#pragma warning(push)
#pragma warning(disable: 6320 6322)
		__try{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER){
		}
#pragma warning(pop)
	}

	void set(std::thread::native_handle_type id, std::string const& name)
	{
		if (SetThreadDescription) 
		{
			auto wname = ljh::convert_string(name);
			SetThreadDescription(id, wname.c_str());
		}
		else 
		{
			THREADNAME_INFO info;
			info.dwType = 0x1000;
			info.szName = name.get();
			info.dwThreadID = GetThreadId(id);
			info.dwFlags = 0;
			ThrowThreadName(info);
		}
	}
}

//          Copyright Jared Irwin 2020-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/named_mutex.hpp"
#include <Windows.h>

namespace ljh::__
{
	void* create_mutex(char const* name)
	{
		return CreateMutexA(nullptr, false, name);
	}

	void release_mutex(void* object)
	{
		CloseHandle(object);
	}

	void lock_mutex(void* object)
	{
		WaitForSingleObject(object, INFINITE);
	}

	bool try_lock_mutex(void* object)
	{
		return WaitForSingleObject(object, 0) == WAIT_OBJECT_0;
	}

	void unlock_mutex(void* object)
	{
		ReleaseMutex(object);
	}
}
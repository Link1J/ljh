
//          Copyright Jared Irwin 2023-2025
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// Based on https://learn.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code?view=vs-2022

#include "ljh/thread_name.hpp"
#include <sys/prctl.h>

namespace ljh::thread_name
{
	void set_for_current(std::string const& name)
	{
		prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
	}

	void set(std::thread::native_handle_type id, std::string const& name)
	{
		pthread_setname_np(id, name.c_str());
	}
}
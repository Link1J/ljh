
//          Copyright Jared Irwin 2020-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// get_index.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
// Requires cpp_version.hpp
// Requires compile_time_string.hpp
//
// ABOUT
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "cpp_version.hpp"
#include "compile_time_string.hpp"

namespace ljh
{
	namespace __
	{
		void* create_mutex(char const* name);
		void release_mutex(void* object);
		void lock_mutex(void* object);
		bool try_lock_mutex(void* object);
		void unlock_mutex(void* object);
	}

	template <ljh::compile_time_string name> 
	struct named_mutex
	{
		static_assert(std::is_same_v<typename decltype(name)::char_type, char>, "`name` must be a string of type char");

		named_mutex() noexcept
		{
			__object = __::create_mutex(name.data());
		}

		~named_mutex() noexcept
		{
			__::unlock_mutex(__object);
			__::release_mutex(__object);
		}

		void lock() const noexcept
		{
			__::lock_mutex(__object);
		}

		void unlock() const noexcept
		{
			__::unlock_mutex(__object);
		}

		[[nodiscard]] bool try_lock() const noexcept
		{
			return __::try_lock_mutex(__object);
		}

	private:
		void* __object;
	};
} // namespace ljh
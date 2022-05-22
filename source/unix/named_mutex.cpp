
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/named_mutex.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string>

namespace ljh::__
{
	void* create_mutex(char const* name)
	{
		using namespace std::string_literals;
		return sem_open(("/"s + name).data(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 1);
	}

	void release_mutex(void* object)
	{
		sem_close((sem_t*)object);
	}

	void lock_mutex(void* object)
	{
		sem_wait((sem_t*)object);
	}

	bool try_lock_mutex(void* object)
	{
		return sem_trywait((sem_t*)object) == -1;
	}

	void unlock_mutex(void* object)
	{
		sem_post((sem_t*)object);
	}
}

//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/system_info.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#endif

#if defined(LJH_TARGET_Linux) || defined(LJH_TARGET_Unix)
#include <sys/utsname.h>
#include <fstream>
#endif

#if defined(LJH_TARGET_Linux)
ljh::expected<ljh::system_info::platform, ljh::system_info::error> ljh::system_info::get_platform()
{
	return platform::Linux;
}
#elif defined(LJH_TARGET_Unix)
ljh::expected<ljh::system_info::platform, ljh::system_info::error> ljh::system_info::get_platform()
{
	return platform::Unix;
}
#endif

#if defined(LJH_TARGET_Linux) || defined(LJH_TARGET_Unix)
ljh::expected<ljh::version, ljh::system_info::error> ljh::system_info::get_version()
{
	struct utsname buffer;
	uname(&buffer);
	return buffer.release;
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_string()
{
	std::ifstream file;
	std::string name;

	file.open("/etc/os-release");
	if (file.is_open())
	{
		std::string version;
		std::string temp;

		while(file)
		{
			std::getline(file, temp);

			if (temp.starts_with("NAME="   ))
				name    = temp.substr(6, temp.find_last_of('"') - 6);
			if (temp.starts_with("VERSION="))
				version = temp.substr(9, temp.find_last_of('"') - 9);
		}

		if (version != "")
			name += " " + version;
		name += " on ";
	}

	struct utsname buffer;
	uname(&buffer);
	name += buffer.sysname + std::string{" "} + buffer.release;

	return name;
}

ljh::expected<ljh::u32, ljh::system_info::error> ljh::system_info::get_sdk()
{
	return unexpected{error::no_info};
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_model()
{
	return unexpected{error::no_info};
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_manufacturer()
{
	return unexpected{error::no_info};
}
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

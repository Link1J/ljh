
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/system_info.hpp"

#if !defined(LJH_TARGET_Windows) && !defined(LJH_TARGET_iOS) && !defined(LJH_TARGET_MacOS) \
	&& !defined(LJH_TARGET_Android) && !defined(LJH_TARGET_Linux) && !defined(LJH_TARGET_Unix)

ljh::expected<ljh::system_info::platform, ljh::system_info::error> ljh::system_info::get_platform()
{
	return unexpected{error::unknown_os};
}

ljh::expected<ljh::version, ljh::system_info::error> ljh::system_info::get_version()
{
	return unexpected{error::unknown_os};
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_string()
{
	return unexpected{error::unknown_os};
}

ljh::expected<ljh::u32, ljh::system_info::error> ljh::system_info::get_sdk()
{
	return unexpected{error::unknown_os};
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_model()
{
	return unexpected{error::unknown_os};
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_manufacturer()
{
	return unexpected{error::unknown_os};
}

#endif
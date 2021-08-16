
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/system_info.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#endif

#import <Foundation/Foundation.h>
#import <CoreServices/CoreServices.h>

#include <TargetConditionals.h>
#include <string>
#include <sys/utsname.h>
#include "ljh/version.hpp"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

std::string __os_info_get_model_obj_c()
{
	std::string string;
	if ([[UIDevice currentDevice] respondsToSelector:@selector(model)])
	{
		NSString* text = [[UIDevice currentDevice] model];
		string = {[text cStringUsingEncoding:NSUTF8StringEncoding], [text lengthOfBytesUsingEncoding:NSUTF8StringEncoding]};
	}
	return string;
}
#endif

using namespace ljh::int_types;

ljh::expected<ljh::system_info::platform, ljh::system_info::error> ljh::system_info::get_platform()
{
#if defined(LJH_TARGET_MacOS)
	return platform::macOS;
#elif defined(LJH_TARGET_iOS)
	return platform::iOS;
#endif
}

ljh::expected<ljh::version, ljh::system_info::error> ljh::system_info::get_version()
{
	version number;

	if ([[NSProcessInfo processInfo] respondsToSelector:@selector(operatingSystemVersion)])
	{
		NSOperatingSystemVersion os = [[NSProcessInfo processInfo] operatingSystemVersion];
		number = {(version::value_type)os.majorVersion, (version::value_type)os.minorVersion, (version::value_type)os.patchVersion};
	}
	else
	{
		SInt32 major, minor, patch;
		Gestalt(gestaltSystemVersionMajor , &major);
		Gestalt(gestaltSystemVersionMinor , &minor);
		Gestalt(gestaltSystemVersionBugFix, &patch);
		return ljh::version{(version::value_type)major, (version::value_type)minor, (version::value_type)patch};
	}

#if defined(LJH_TARGET_MacOS)
	if (version > macOS_10_15 && version < macOS_11)
		version = macOS_11.version;
#endif

	return version;
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_string()
{
	std::string string;
	if ([[NSProcessInfo processInfo] respondsToSelector:@selector(operatingSystemVersionString)])
	{
		NSString* text = [[NSProcessInfo processInfo] operatingSystemVersionString];
		string = {[text cStringUsingEncoding:NSUTF8StringEncoding], [text lengthOfBytesUsingEncoding:NSUTF8StringEncoding]};
		string = string.substr(8);
	}
	else
	{
		string = *get_version();
	}

#if defined(LJH_TARGET_MacOS)
	if (ver >= macOS_10_12)
		string = "macOS " + string;
	else if (ver >= OS_X_10_8)
		string = "OS X " + string;
	else if (ver >= Mac_OS_X_10_0)
		string = "Mac OS X " + string;
	else
		string = "Mac OS " + string;
#elif defined(LJH_TARGET_iOS)
	if (ver >= iPadOS_13_1 && __os_info_get_model_obj_c().starts_with("iPad"))
		string = "iPadOS " + string;
	else if (ver >= iOS_4_0)
		string = "iOS " + string;
	else
		string = "iPhone OS " + string;
#endif

	return string;
}

ljh::expected<ljh::u32, ljh::system_info::error> ljh::system_info::get_sdk()
{
	auto version = get_version();
	return version.major() << 16 | version.minor() << 8  | version.build();
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_model()
{
	struct utsname buffer;
	uname(&buffer);
	return buffer.machine;
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_manufacturer()
{
	return "Apple";
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

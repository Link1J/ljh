
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#if __APPLE__

#import <Foundation/NSProcessInfo.h>
#include "ljh/version.hpp"
#include <string>

#include <TargetConditionals.h>

ljh::version __os_info_get_version_obj_c()
{
	if ([[NSProcessInfo processInfo] respondsToSelector:@selector(operatingSystemVersion)])
	{
		id os = [[NSProcessInfo processInfo] operatingSystemVersion];
		return version{os.majorVersion, os.minorVersion, os.patchVersion};
	}
	else
	{
		SInt32 major, minor, patch;
		Gestalt(gestaltSystemVersionMajor , &major);
		Gestalt(gestaltSystemVersionMinor , &minor);
		Gestalt(gestaltSystemVersionBugFix, &patch);
		return version{major, minor, patch};
	}
}

std::string __os_info_get_string_obj_c()
{
	std::string string;
	if ([[NSProcessInfo processInfo] respondsToSelector:@selector(operatingSystemVersionString)])
	{
		id text = [[NSProcessInfo processInfo] operatingSystemVersionString];
		string = {[text cStringUsingEncoding:NSUTF8StringEncoding], [text lengthOfBytesUsingEncoding:NSUTF8StringEncoding]};
	}
	return string;
}

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>

std::string __os_info_get_model_obj_c()
{
	std::string string;
	if ([[UIDevice currentDevice] respondsToSelector:@selector(model)])
	{
		id text = [[UIDevice currentDevice] model];
		string = {[text cStringUsingEncoding:NSUTF8StringEncoding], [text lengthOfBytesUsingEncoding:NSUTF8StringEncoding]};
	}
	return string;
}
#endif
#endif
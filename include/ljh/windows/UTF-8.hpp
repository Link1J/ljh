
//          Copyright Jared Irwin 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// UTF-8.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
// 
// Requires Windows SDK
// 
// ABOUT
//     Sets the Windows Console codepage to UTF-8
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once
#include <windows.h>

namespace ljh 
{
	namespace windows 
	{
		namespace console 
		{
			void make_UTF8()
			{
				SetConsoleOutputCP(CP_UTF8);
			}
		}
	}
}
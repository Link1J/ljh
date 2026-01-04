
//          Copyright Jared Irwin 2020-2026
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// system_info.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++17
//
// ABOUT
//     System Info Collection
//
// USAGE
//
// Version History
//     1.0 Inital Version

#pragma once

#include "version.hpp"
#include "expected.hpp"
#include "int_types.hpp"
#include "os_build_info.hpp"

namespace ljh::system_info
{
    LJH_MODULE_OS_EXPORT enum class error : ljh::int_types::u08
    {
        unknown_os,
        cannot_get_version,
        no_info,
    };

    LJH_MODULE_OS_EXPORT enum class platform : ljh::int_types::u08
    {
        Windows    = 0,
        Unix       = 1,
        macOS      = 2,
        iOS        = 3,
        Linux      = 4,
        Android    = 5,
        WindowsDOS = 6,

        WindowsNT = Windows,
        Mac_OS_X  = macOS,
        OS_X      = macOS,
        Mac_OS    = macOS,
        iPhone_OS = iOS,
        iPadOS    = iOS,
    };

    LJH_MODULE_OS_EXPORT struct info_data
    {
        ljh::system_info::platform platform{};
        ljh::version               version;
        u32                        sdk{};

        bool operator==(ljh::system_info::platform const&) const;
        bool operator==(ljh::version const&) const;
        bool operator!=(ljh::version const&) const;
        bool operator<(ljh::version const&) const;
        bool operator>(ljh::version const&) const;
        bool operator>=(ljh::version const&) const;
        bool operator<=(ljh::version const&) const;
        bool operator==(ljh::u32 const&) const;
        bool operator!=(ljh::u32 const&) const;
        bool operator<(ljh::u32 const&) const;
        bool operator>(ljh::u32 const&) const;
        bool operator>=(ljh::u32 const&) const;
        bool operator<=(ljh::u32 const&) const;
    };

    LJH_MODULE_OS_EXPORT expected<platform, error> get_platform();        // Use for Version Check
    LJH_MODULE_OS_EXPORT expected<version, error> get_version();          // Use for Version Check
    LJH_MODULE_OS_EXPORT expected<std::string, error> get_string();       // Only use for Logs
    LJH_MODULE_OS_EXPORT expected<u32, error> get_sdk();                  // An infinitely increasing number
    LJH_MODULE_OS_EXPORT expected<std::string, error> get_model();        // Only use for Logs
    LJH_MODULE_OS_EXPORT expected<std::string, error> get_manufacturer(); // Only use for Logs

    // OS Versions
    LJH_MODULE_OS_EXPORT inline namespace versions
    {
        extern info_data const Windows_1_01;
        extern info_data const Windows_1_0;
        extern info_data const Windows_1_02;
        extern info_data const Windows_1_03;
        extern info_data const Windows_1_04;
        extern info_data const Windows_2_03;
        extern info_data const Windows_2_0;
        extern info_data const Windows_2_10;
        extern info_data const Windows_2_11;
        extern info_data const Windows_3_0;
        extern info_data const Windows_3_1;
        extern info_data const Windows_3_11;
        extern info_data const Windows_95;
        extern info_data const Windows_98;
        extern info_data const Windows_98SE;
        extern info_data const Windows_ME;
        extern info_data const Windows_NT_3_1;
        extern info_data const Windows_NT_3_1_SP_1;
        extern info_data const Windows_NT_3_1_SP_2;
        extern info_data const Windows_NT_3_1_SP_3;
        extern info_data const Windows_NT_3_5;
        extern info_data const Windows_NT_3_5_SP_1;
        extern info_data const Windows_NT_3_5_SP_2;
        extern info_data const Windows_NT_3_5_SP_3;
        extern info_data const Windows_NT_3_51;
        extern info_data const Windows_NT_3_51_SP_1;
        extern info_data const Windows_NT_3_51_SP_2;
        extern info_data const Windows_NT_3_51_SP_3;
        extern info_data const Windows_NT_3_51_SP_4;
        extern info_data const Windows_NT_3_51_SP_5;
        extern info_data const Windows_NT_4_0;
        extern info_data const Windows_NT_4_0_SP_1;
        extern info_data const Windows_NT_4_0_SP_2;
        extern info_data const Windows_NT_4_0_SP_3;
        extern info_data const Windows_NT_4_0_SP_4;
        extern info_data const Windows_NT_4_0_SP_5;
        extern info_data const Windows_NT_4_0_SP_6;
        extern info_data const Windows_NT_4_0_SP_6a;
        extern info_data const Windows_NT_5_0;
        extern info_data const Windows_2000;
        extern info_data const Windows_2000_SP_1;
        extern info_data const Windows_2000_SP_2;
        extern info_data const Windows_2000_SP_3;
        extern info_data const Windows_2000_SP_4;
        extern info_data const Windows_NT_5_1;
        extern info_data const Windows_XP;
        extern info_data const Windows_XP_SP_1;
        extern info_data const Windows_XP_SP_2;
        extern info_data const Windows_XP_SP_3;
        extern info_data const Windows_NT_5_2;
        extern info_data const Windows_XP_x64;
        extern info_data const Windows_XP_x64_SP_3;
        extern info_data const Windows_NT_6_0;
        extern info_data const Windows_Vista;
        extern info_data const Windows_Vista_SP_1;
        extern info_data const Windows_Vista_SP_2;
        extern info_data const Windows_Vista_SP_2_Update;
        extern info_data const Windows_NT_6_1;
        extern info_data const Windows_7;
        extern info_data const Windows_7_SP_1;
        extern info_data const Windows_NT_6_2;
        extern info_data const Windows_8;
        extern info_data const Windows_NT_6_3;
        extern info_data const Windows_8_1;
        extern info_data const Windows_NT_10_0;
        extern info_data const Windows_10;
        extern info_data const Windows_10_1507;
        extern info_data const Windows_10_1511;
        extern info_data const Windows_10_1607;
        extern info_data const Windows_10_1703;
        extern info_data const Windows_10_1709;
        extern info_data const Windows_10_1803;
        extern info_data const Windows_10_1809;
        extern info_data const Windows_10_1903;
        extern info_data const Windows_10_1909;
        extern info_data const Windows_10_2004;
        extern info_data const Windows_10_20H2;
        extern info_data const Windows_10_21H1;
        extern info_data const Windows_10_21H2;
        extern info_data const Windows_10_22H2;
        extern info_data const Windows_11;
        extern info_data const Windows_11_21H2;
        extern info_data const Windows_11_22H2;

        extern info_data const Android_API_1;
        extern info_data const Android_1;
        extern info_data const Android_API_2;
        extern info_data const Android_1_1;
        extern info_data const Android_API_3;
        extern info_data const Android_1_5;
        extern info_data const Android_Cupcake;
        extern info_data const Android_API_4;
        extern info_data const Android_1_6;
        extern info_data const Android_Donut;
        extern info_data const Android_API_5;
        extern info_data const Android_2;
        extern info_data const Android_Eclair;
        extern info_data const Android_API_6;
        extern info_data const Android_2_0_1;
        extern info_data const Android_API_7;
        extern info_data const Android_2_1;
        extern info_data const Android_API_8;
        extern info_data const Android_2_2;
        extern info_data const Android_Froyo;
        extern info_data const Android_API_9;
        extern info_data const Android_2_3;
        extern info_data const Android_Gingerbread;
        extern info_data const Android_API_10;
        extern info_data const Android_2_3_3;
        extern info_data const Android_API_11;
        extern info_data const Android_3;
        extern info_data const Android_Honeycomb;
        extern info_data const Android_API_12;
        extern info_data const Android_3_1;
        extern info_data const Android_API_13;
        extern info_data const Android_3_2;
        extern info_data const Android_API_14;
        extern info_data const Android_4;
        extern info_data const Android_Ice_Cream_Sandwich;
        extern info_data const Android_API_15;
        extern info_data const Android_4_0_3;
        extern info_data const Android_API_16;
        extern info_data const Android_4_1;
        extern info_data const Android_Jelly_Bean;
        extern info_data const Android_API_17;
        extern info_data const Android_4_2;
        extern info_data const Android_API_18;
        extern info_data const Android_4_3;
        extern info_data const Android_API_19;
        extern info_data const Android_4_4;
        extern info_data const Android_KitKat;
        extern info_data const Android_API_20;
        extern info_data const Android_4_4W;
        extern info_data const Android_API_21;
        extern info_data const Android_5;
        extern info_data const Android_Lollipop;
        extern info_data const Android_API_22;
        extern info_data const Android_5_1;
        extern info_data const Android_API_23;
        extern info_data const Android_6;
        extern info_data const Android_Marshmallow;
        extern info_data const Android_API_24;
        extern info_data const Android_7;
        extern info_data const Android_Nougat;
        extern info_data const Android_API_25;
        extern info_data const Android_7_1;
        extern info_data const Android_API_26;
        extern info_data const Android_8;
        extern info_data const Android_Oreo;
        extern info_data const Android_API_27;
        extern info_data const Android_8_1;
        extern info_data const Android_API_28;
        extern info_data const Android_9;
        extern info_data const Android_Pie;
        extern info_data const Android_API_29;
        extern info_data const Android_10;
        extern info_data const Android_API_30;
        extern info_data const Android_11;
        extern info_data const Android_API_31;
        extern info_data const Android_12;
        extern info_data const Android_API_32;
        extern info_data const Android_12L;
        extern info_data const Android_API_33;
        extern info_data const Android_13;

        extern info_data const Mac_OS_X_10_0;
        extern info_data const Mac_OS_X_10_1;
        extern info_data const Mac_OS_X_10_2;
        extern info_data const Mac_OS_X_Jaguar;
        extern info_data const Mac_OS_X_10_3;
        extern info_data const Mac_OS_X_Panther;
        extern info_data const Mac_OS_X_10_4;
        extern info_data const Mac_OS_X_Tiger;
        extern info_data const Mac_OS_X_10_5;
        extern info_data const Mac_OS_X_Leopard;
        extern info_data const Mac_OS_X_10_6;
        extern info_data const Mac_OS_X_Snow_Leopard;
        extern info_data const Mac_OS_X_10_7;
        extern info_data const Mac_OS_X_Lion;
        extern info_data const OS_X_10_8;
        extern info_data const OS_X_Mountain_Lion;
        extern info_data const OS_X_10_9;
        extern info_data const OS_X_Mavericks;
        extern info_data const OS_X_10_10;
        extern info_data const OS_X_Yosemite;
        extern info_data const OS_X_10_11;
        extern info_data const OS_X_El_Capitan;
        extern info_data const macOS_10_12;
        extern info_data const macOS_Sierra;
        extern info_data const macOS_10_13;
        extern info_data const macOS_High_Sierra;
        extern info_data const macOS_10_14;
        extern info_data const macOS_Mojave;
        extern info_data const macOS_10_15;
        extern info_data const macOS_Catalina;
        extern info_data const macOS_11;
        extern info_data const macOS_Big_Sur;
        extern info_data const macOS_11_1;
        extern info_data const macOS_11_2;
        extern info_data const macOS_11_3;
        extern info_data const macOS_11_4;
        extern info_data const macOS_11_5;
        extern info_data const macOS_11_6;
        extern info_data const macOS_12;
        extern info_data const macOS_Monterey;
        extern info_data const macOS_12_1;
        extern info_data const macOS_12_2;
        extern info_data const macOS_12_3;
        extern info_data const macOS_12_4;
        extern info_data const macOS_12_5;

        extern info_data const iPhone_OS_1_0;
        extern info_data const iPhone_OS_1_1;
        extern info_data const iPhone_OS_2_0;
        extern info_data const iPhone_OS_2_1;
        extern info_data const iPhone_OS_2_2;
        extern info_data const iPhone_OS_3_0;
        extern info_data const iPhone_OS_3_1;
        extern info_data const iPhone_OS_3_2;
        extern info_data const iOS_4_0;
        extern info_data const iOS_4_1;
        extern info_data const iOS_5_0;
        extern info_data const iOS_5_1;
        extern info_data const iOS_6_0;
        extern info_data const iOS_6_1;
        extern info_data const iOS_7_0;
        extern info_data const iOS_7_1;
        extern info_data const iOS_8_0;
        extern info_data const iOS_8_1;
        extern info_data const iOS_9_0;
        extern info_data const iOS_9_1;
        extern info_data const iOS_9_2;
        extern info_data const iOS_9_3;
        extern info_data const iOS_9_4;
        extern info_data const iOS_10_0;
        extern info_data const iOS_10_1;
        extern info_data const iOS_10_2;
        extern info_data const iOS_10_3;
        extern info_data const iOS_11_0;
        extern info_data const iOS_11_1;
        extern info_data const iOS_11_2;
        extern info_data const iOS_11_3;
        extern info_data const iOS_12_0;
        extern info_data const iOS_12_1;
        extern info_data const iOS_12_2;
        extern info_data const iOS_12_3;
        extern info_data const iOS_12_4;
        extern info_data const iOS_13_0;
        extern info_data const iOS_13_1;
        extern info_data const iPadOS_13_1;
        extern info_data const iOS_13_2;
        extern info_data const iPadOS_13_2;
        extern info_data const iOS_13_3;
        extern info_data const iPadOS_13_3;
        extern info_data const iOS_13_4;
        extern info_data const iPadOS_13_4;
        extern info_data const iOS_13_5;
        extern info_data const iPadOS_13_5;
        extern info_data const iOS_13_6;
        extern info_data const iPadOS_13_6;
        extern info_data const iOS_13_7;
        extern info_data const iPadOS_13_7;
        extern info_data const iOS_14_0;
        extern info_data const iPadOS_14_0;
        extern info_data const iOS_14_1;
        extern info_data const iPadOS_14_1;
        extern info_data const iOS_14_2;
        extern info_data const iPadOS_14_2;
        extern info_data const iOS_14_3;
        extern info_data const iPadOS_14_3;
        extern info_data const iOS_14_4;
        extern info_data const iPadOS_14_4;
        extern info_data const iOS_14_5;
        extern info_data const iPadOS_14_5;
        extern info_data const iOS_14_6;
        extern info_data const iPadOS_14_6;
        extern info_data const iOS_14_7;
        extern info_data const iPadOS_14_7;
        extern info_data const iOS_14_8;
        extern info_data const iPadOS_14_8;
        extern info_data const iOS_15_0;
        extern info_data const iPadOS_15_0;
        extern info_data const iOS_15_1;
        extern info_data const iPadOS_15_1;
        extern info_data const iOS_15_2;
        extern info_data const iPadOS_15_2;
        extern info_data const iOS_15_3;
        extern info_data const iPadOS_15_3;
        extern info_data const iOS_15_4;
        extern info_data const iPadOS_15_4;
        extern info_data const iOS_15_5;
        extern info_data const iPadOS_15_5;
        extern info_data const iOS_15_6;
        extern info_data const iPadOS_15_6;
    } // namespace versions

    LJH_MODULE_OS_EXPORT bool operator==(ljh::system_info::platform const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator==(ljh::version const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator!=(ljh::version const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator<(ljh::version const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator>(ljh::version const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator>=(ljh::version const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator<=(ljh::version const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator==(ljh::u32 const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator!=(ljh::u32 const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator<(ljh::u32 const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator>(ljh::u32 const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator>=(ljh::u32 const&, ljh::system_info::info_data const&);
    LJH_MODULE_OS_EXPORT bool operator<=(ljh::u32 const&, ljh::system_info::info_data const&);
}; // namespace ljh::system_info
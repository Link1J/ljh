
//          Copyright Jared Irwin 2020-2022
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/system_info.hpp"

bool ljh::system_info::info_data::operator==(const ljh::system_info::platform& other) const { return platform == other; }
bool ljh::system_info::info_data::operator==(const ljh::version              & other) const { return version  == other; }
bool ljh::system_info::info_data::operator!=(const ljh::version              & other) const { return version  != other; }
bool ljh::system_info::info_data::operator< (const ljh::version              & other) const { return version  <  other; }
bool ljh::system_info::info_data::operator> (const ljh::version              & other) const { return version  >  other; }
bool ljh::system_info::info_data::operator>=(const ljh::version              & other) const { return version  >= other; }
bool ljh::system_info::info_data::operator<=(const ljh::version              & other) const { return version  <= other; }
bool ljh::system_info::info_data::operator==(const ljh::u32                  & other) const { return sdk      == other; }
bool ljh::system_info::info_data::operator!=(const ljh::u32                  & other) const { return sdk      != other; }
bool ljh::system_info::info_data::operator< (const ljh::u32                  & other) const { return sdk      <  other; }
bool ljh::system_info::info_data::operator> (const ljh::u32                  & other) const { return sdk      >  other; }
bool ljh::system_info::info_data::operator>=(const ljh::u32                  & other) const { return sdk      >= other; }
bool ljh::system_info::info_data::operator<=(const ljh::u32                  & other) const { return sdk      <= other; }
bool ljh::system_info::operator==(const ljh::system_info::platform& lhs, const ljh::system_info::info_data& rhs) { return rhs == lhs; }
bool ljh::system_info::operator==(const ljh::version              & lhs, const ljh::system_info::info_data& rhs) { return rhs == lhs; }
bool ljh::system_info::operator!=(const ljh::version              & lhs, const ljh::system_info::info_data& rhs) { return rhs != lhs; }
bool ljh::system_info::operator< (const ljh::version              & lhs, const ljh::system_info::info_data& rhs) { return rhs >  lhs; }
bool ljh::system_info::operator> (const ljh::version              & lhs, const ljh::system_info::info_data& rhs) { return rhs <  lhs; }
bool ljh::system_info::operator>=(const ljh::version              & lhs, const ljh::system_info::info_data& rhs) { return rhs <= lhs; }
bool ljh::system_info::operator<=(const ljh::version              & lhs, const ljh::system_info::info_data& rhs) { return rhs >= lhs; }
bool ljh::system_info::operator==(const ljh::u32                  & lhs, const ljh::system_info::info_data& rhs) { return rhs == lhs; }
bool ljh::system_info::operator!=(const ljh::u32                  & lhs, const ljh::system_info::info_data& rhs) { return rhs != lhs; }
bool ljh::system_info::operator< (const ljh::u32                  & lhs, const ljh::system_info::info_data& rhs) { return rhs >  lhs; }
bool ljh::system_info::operator> (const ljh::u32                  & lhs, const ljh::system_info::info_data& rhs) { return rhs <  lhs; }
bool ljh::system_info::operator>=(const ljh::u32                  & lhs, const ljh::system_info::info_data& rhs) { return rhs <= lhs; }
bool ljh::system_info::operator<=(const ljh::u32                  & lhs, const ljh::system_info::info_data& rhs) { return rhs >= lhs; }

// apple_version_to_sdk
constexpr ljh::u32 avs(ljh::u32 major, ljh::u32 minor)
{
	return major << 16 | minor;
}

namespace ljh::system_info::inline versions
{
	const info_data Windows_1_01                { platform::WindowsDOS, { 1,  1,     0            },           0};
	const info_data Windows_1_02                { platform::WindowsDOS, { 1,  2,     0            },           0};
	const info_data Windows_1_03                { platform::WindowsDOS, { 1,  3,     0            },           0};
	const info_data Windows_1_04                { platform::WindowsDOS, { 1,  4,     0            },           0};
	const info_data Windows_2_03                { platform::WindowsDOS, { 2,  3,     0            },           0};
	const info_data Windows_2_10                { platform::WindowsDOS, { 2, 10,     0            },           0};
	const info_data Windows_2_11                { platform::WindowsDOS, { 2, 11,     0            },           0};
	const info_data Windows_3_0                 { platform::WindowsDOS, { 3,  0,     0            },           0};
	const info_data Windows_3_1                 { platform::WindowsDOS, { 3,  1,   103            },         103};
	const info_data Windows_3_11                { platform::WindowsDOS, { 3, 11,   300            },         300};
	const info_data Windows_95                  { platform::WindowsDOS, { 4,  0,   950            },         950};
	const info_data Windows_98                  { platform::WindowsDOS, { 4, 10,  1998            },        1998};
	const info_data Windows_98SE                { platform::WindowsDOS, { 4, 10,  2222            },        2222};
	const info_data Windows_ME                  { platform::WindowsDOS, { 4, 90,  3000            },        3000};
	const info_data Windows_NT_3_1              { platform::WindowsNT , { 3,  1,   511 /*,     1*/},         511};
	const info_data Windows_NT_3_1_SP_1         { platform::WindowsNT , { 3,  1,   528 /*,     1*/},         528};
	const info_data Windows_NT_3_1_SP_2         { platform::WindowsNT , { 3,  1,   528 /*,     1*/},         528};
	const info_data Windows_NT_3_1_SP_3         { platform::WindowsNT , { 3,  1,   528 /*,     1*/},         528};
	const info_data Windows_NT_3_5              { platform::WindowsNT , { 3,  5,   807 /*,     1*/},         807};
	const info_data Windows_NT_3_5_SP_1         { platform::WindowsNT , { 3,  5,   807 /*,     1*/},         807};
	const info_data Windows_NT_3_5_SP_2         { platform::WindowsNT , { 3,  5,   807 /*,     1*/},         807};
	const info_data Windows_NT_3_5_SP_3         { platform::WindowsNT , { 3,  5,   807 /*,     1*/},         807};
	const info_data Windows_NT_3_51             { platform::WindowsNT , { 3, 51,  1057 /*,     1*/},        1057};
	const info_data Windows_NT_3_51_SP_1        { platform::WindowsNT , { 3, 51,  1057 /*,     2*/},        1057};
	const info_data Windows_NT_3_51_SP_2        { platform::WindowsNT , { 3, 51,  1057 /*,     3*/},        1057};
	const info_data Windows_NT_3_51_SP_3        { platform::WindowsNT , { 3, 51,  1057 /*,     4*/},        1057};
	const info_data Windows_NT_3_51_SP_4        { platform::WindowsNT , { 3, 51,  1057 /*,     5*/},        1057};
	const info_data Windows_NT_3_51_SP_5        { platform::WindowsNT , { 3, 51,  1057 /*,     6*/},        1057};
	const info_data Windows_NT_4_0              { platform::WindowsNT , { 4,  0,  1381 /*,     1*/},        1381};
	const info_data Windows_NT_4_0_SP_1         { platform::WindowsNT , { 4,  0,  1381 /*,     2*/},        1381};
	const info_data Windows_NT_4_0_SP_2         { platform::WindowsNT , { 4,  0,  1381 /*,     3*/},        1381};
	const info_data Windows_NT_4_0_SP_3         { platform::WindowsNT , { 4,  0,  1381 /*,     4*/},        1381};
	const info_data Windows_NT_4_0_SP_4         { platform::WindowsNT , { 4,  0,  1381 /*,   133*/},        1381};
	const info_data Windows_NT_4_0_SP_5         { platform::WindowsNT , { 4,  0,  1381 /*,   204*/},        1381};
	const info_data Windows_NT_4_0_SP_6         { platform::WindowsNT , { 4,  0,  1381 /*,   335*/},        1381};
	const info_data Windows_NT_4_0_SP_6a        { platform::WindowsNT , { 4,  0,  1381 /*,   335*/},        1381};
	const info_data Windows_NT_5_0              { platform::WindowsNT , { 5,  0,  2195 /*,     1*/},        2195};
	const info_data Windows_2000_SP_1           { platform::WindowsNT , { 5,  0,  2195 /*,  1620*/},        2195};
	const info_data Windows_2000_SP_2           { platform::WindowsNT , { 5,  0,  2195 /*,  2951*/},        2195};
	const info_data Windows_2000_SP_3           { platform::WindowsNT , { 5,  0,  2195 /*,  5438*/},        2195};
	const info_data Windows_2000_SP_4           { platform::WindowsNT , { 5,  0,  2195 /*,  6717*/},        2195};
	const info_data Windows_NT_5_1              { platform::WindowsNT , { 5,  1,  2600 /*,     0*/},        2600};
	const info_data Windows_XP_SP_1             { platform::WindowsNT , { 5,  1,  2600 /*,  1106*/},        2600};
	const info_data Windows_XP_SP_2             { platform::WindowsNT , { 5,  1,  2600 /*,  2180*/},        2600};
	const info_data Windows_XP_SP_3             { platform::WindowsNT , { 5,  1,  2600 /*,  5512*/},        2600};
	const info_data Windows_NT_5_2              { platform::WindowsNT , { 5,  2,  3790 /*,  1830*/},        3790};
	const info_data Windows_XP_x64_SP_3         { platform::WindowsNT , { 5,  2,  3790 /*,  3959*/},        3790};
	const info_data Windows_NT_6_0              { platform::WindowsNT , { 6,  0,  6000 /*, 16386*/},        6000};
	const info_data Windows_Vista_SP_1          { platform::WindowsNT , { 6,  0,  6001 /*, 18000*/},        6001};
	const info_data Windows_Vista_SP_2          { platform::WindowsNT , { 6,  0,  6002 /*, 18005*/},        6002};
	const info_data Windows_Vista_SP_2_Update   { platform::WindowsNT , { 6,  0,  6003 /*, 20491*/},        6003};
	const info_data Windows_NT_6_1              { platform::WindowsNT , { 6,  1,  7600 /*, 16385*/},        7600};
	const info_data Windows_7_SP_1              { platform::WindowsNT , { 6,  1,  7601 /*, 17514*/},        7601};
	const info_data Windows_NT_6_2              { platform::WindowsNT , { 6,  2,  9200 /*, 16384*/},        9200};
	const info_data Windows_NT_6_3              { platform::WindowsNT , { 6,  3,  9600 /*, 16384*/},        9600};
	const info_data Windows_NT_10_0             { platform::WindowsNT , {10,  0, 10240 /*, 16384*/},       10240};
	const info_data Windows_10_1511             { platform::WindowsNT , {10,  0, 10586 /*,     0*/},       10586};
	const info_data Windows_10_1607             { platform::WindowsNT , {10,  0, 14393 /*,     0*/},       14393};
	const info_data Windows_10_1703             { platform::WindowsNT , {10,  0, 15063 /*,     0*/},       15063};
	const info_data Windows_10_1709             { platform::WindowsNT , {10,  0, 16299 /*,    15*/},       16299};
	const info_data Windows_10_1803             { platform::WindowsNT , {10,  0, 17134 /*,     1*/},       17134};
	const info_data Windows_10_1809             { platform::WindowsNT , {10,  0, 17763 /*,     1*/},       17763};
	const info_data Windows_10_1903             { platform::WindowsNT , {10,  0, 18362 /*,     1*/},       18362};
	const info_data Windows_10_1909             { platform::WindowsNT , {10,  0, 18363 /*,   418*/},       18363};
	const info_data Windows_10_2004             { platform::WindowsNT , {10,  0, 19041 /*,     1*/},       19041};
	const info_data Windows_10_20H2             { platform::WindowsNT , {10,  0, 19042 /*,   508*/},       19042};
	const info_data Windows_10_21H1             { platform::WindowsNT , {10,  0, 19043 /*,   928*/},       19043};
	const info_data Windows_10_21H2             { platform::WindowsNT , {10,  0, 19044 /*,  1348*/},       19044};
	const info_data Windows_10_22H2             { platform::WindowsNT , {10,  0, 19045 /*,  1949*/},       19045};
	const info_data Windows_11_21H2             { platform::WindowsNT , {10,  0, 22000 /*,   194*/},       22000};
	const info_data Windows_11_22H2             { platform::WindowsNT , {10,  0, 22621 /*,   586*/},       22621};

	const info_data Android_API_1               { platform::Android   , { 1                       },           1};
	const info_data Android_API_2               { platform::Android   , { 1,  1                   },           2};
	const info_data Android_API_3               { platform::Android   , { 1,  5                   },           3};
	const info_data Android_API_4               { platform::Android   , { 1,  6                   },           4};
	const info_data Android_API_5               { platform::Android   , { 2                       },           5};
	const info_data Android_API_6               { platform::Android   , { 2,  0,     1            },           6};
	const info_data Android_API_7               { platform::Android   , { 2,  1                   },           7};
	const info_data Android_API_8               { platform::Android   , { 2,  2                   },           8};
	const info_data Android_API_9               { platform::Android   , { 2,  3                   },           9};
	const info_data Android_API_10              { platform::Android   , { 2,  3,     3            },          10};
	const info_data Android_API_11              { platform::Android   , { 3                       },          11};
	const info_data Android_API_12              { platform::Android   , { 3,  1                   },          12};
	const info_data Android_API_13              { platform::Android   , { 3,  2                   },          13};
	const info_data Android_API_14              { platform::Android   , { 4                       },          14};
	const info_data Android_API_15              { platform::Android   , { 4,  0,     3            },          15};
	const info_data Android_API_16              { platform::Android   , { 4,  1                   },          16};
	const info_data Android_API_17              { platform::Android   , { 4,  2                   },          17};
	const info_data Android_API_18              { platform::Android   , { 4,  3                   },          18};
	const info_data Android_API_19              { platform::Android   , { 4,  4                   },          19};
	const info_data Android_API_20              { platform::Android   , { 4,  4                   },          20};
	const info_data Android_API_21              { platform::Android   , { 5                       },          21};
	const info_data Android_API_22              { platform::Android   , { 5,  1                   },          22};
	const info_data Android_API_23              { platform::Android   , { 6                       },          23};
	const info_data Android_API_24              { platform::Android   , { 7                       },          24};
	const info_data Android_API_25              { platform::Android   , { 7,  1                   },          25};
	const info_data Android_API_26              { platform::Android   , { 8                       },          26};
	const info_data Android_API_27              { platform::Android   , { 8,  1                   },          27};
	const info_data Android_API_28              { platform::Android   , { 9                       },          28};
	const info_data Android_API_29              { platform::Android   , {10                       },          29};
	const info_data Android_API_30              { platform::Android   , {11                       },          30};
	const info_data Android_API_31              { platform::Android   , {12                       },          31};
	const info_data Android_API_32              { platform::Android   , {12,  1                   },          32};
	const info_data Android_API_33              { platform::Android   , {13                       },          33};

	const info_data Mac_OS_X_10_0               { platform::macOS     , {10,  0                   }, avs(10,  0)};
	const info_data Mac_OS_X_10_1               { platform::macOS     , {10,  1                   }, avs(10,  1)};
	const info_data Mac_OS_X_10_2               { platform::macOS     , {10,  2                   }, avs(10,  2)};
	const info_data Mac_OS_X_10_3               { platform::macOS     , {10,  3                   }, avs(10,  3)};
	const info_data Mac_OS_X_10_4               { platform::macOS     , {10,  4                   }, avs(10,  4)};
	const info_data Mac_OS_X_10_5               { platform::macOS     , {10,  5                   }, avs(10,  5)};
	const info_data Mac_OS_X_10_6               { platform::macOS     , {10,  6                   }, avs(10,  6)};
	const info_data Mac_OS_X_10_7               { platform::macOS     , {10,  7                   }, avs(10,  7)};
	const info_data     OS_X_10_8               { platform::macOS     , {10,  8                   }, avs(10,  8)};
	const info_data     OS_X_10_9               { platform::macOS     , {10,  9                   }, avs(10,  9)};
	const info_data     OS_X_10_10              { platform::macOS     , {10, 10                   }, avs(10, 10)};
	const info_data     OS_X_10_11              { platform::macOS     , {10, 11                   }, avs(10, 11)};
	const info_data    macOS_10_12              { platform::macOS     , {10, 12                   }, avs(10, 12)};
	const info_data    macOS_10_13              { platform::macOS     , {10, 13                   }, avs(10, 13)};
	const info_data    macOS_10_14              { platform::macOS     , {10, 14                   }, avs(10, 14)};
	const info_data    macOS_10_15              { platform::macOS     , {10, 15                   }, avs(10, 15)};
	const info_data    macOS_11                 { platform::macOS     , {11                       }, avs(11,  0)};
	const info_data    macOS_11_1               { platform::macOS     , {11,  1                   }, avs(11,  1)};
	const info_data    macOS_11_2               { platform::macOS     , {11,  2                   }, avs(11,  2)};
	const info_data    macOS_11_3               { platform::macOS     , {11,  3                   }, avs(11,  3)};
	const info_data    macOS_11_4               { platform::macOS     , {11,  4                   }, avs(11,  4)};
	const info_data    macOS_11_5               { platform::macOS     , {11,  5                   }, avs(11,  5)};
	const info_data    macOS_11_6               { platform::macOS     , {11,  6                   }, avs(11,  6)};
	const info_data    macOS_12                 { platform::macOS     , {12                       }, avs(12,  0)};
	const info_data    macOS_12_1               { platform::macOS     , {12,  1                   }, avs(12,  1)};
	const info_data    macOS_12_2               { platform::macOS     , {12,  2                   }, avs(12,  2)};
	const info_data    macOS_12_3               { platform::macOS     , {12,  3                   }, avs(12,  3)};
	const info_data    macOS_12_4               { platform::macOS     , {12,  4                   }, avs(12,  4)};
	const info_data    macOS_12_5               { platform::macOS     , {12,  5                   }, avs(12,  5)};

	const info_data iPhone_OS_1_0               { platform::iOS       , { 1,  0                   }, avs( 1,  0)};
	const info_data iPhone_OS_1_1               { platform::iOS       , { 1,  1                   }, avs( 1,  1)};
	const info_data iPhone_OS_2_0               { platform::iOS       , { 2,  0                   }, avs( 2,  0)};
	const info_data iPhone_OS_2_1               { platform::iOS       , { 2,  1                   }, avs( 2,  1)};
	const info_data iPhone_OS_2_2               { platform::iOS       , { 2,  2                   }, avs( 2,  2)};
	const info_data iPhone_OS_3_0               { platform::iOS       , { 3,  0                   }, avs( 3,  0)};
	const info_data iPhone_OS_3_1               { platform::iOS       , { 3,  1                   }, avs( 3,  1)};
	const info_data iPhone_OS_3_2               { platform::iOS       , { 3,  2                   }, avs( 3,  2)};
	const info_data       iOS_4_0               { platform::iOS       , { 4,  0                   }, avs( 4,  0)};
	const info_data       iOS_4_1               { platform::iOS       , { 4,  1                   }, avs( 4,  1)};
	const info_data       iOS_5_0               { platform::iOS       , { 5,  0                   }, avs( 5,  0)};
	const info_data       iOS_5_1               { platform::iOS       , { 5,  1                   }, avs( 5,  1)};
	const info_data       iOS_6_0               { platform::iOS       , { 6,  0                   }, avs( 6,  0)};
	const info_data       iOS_6_1               { platform::iOS       , { 6,  1                   }, avs( 6,  1)};
	const info_data       iOS_7_0               { platform::iOS       , { 7,  0                   }, avs( 7,  0)};
	const info_data       iOS_7_1               { platform::iOS       , { 7,  1                   }, avs( 7,  1)};
	const info_data       iOS_8_0               { platform::iOS       , { 8,  0                   }, avs( 8,  0)};
	const info_data       iOS_8_1               { platform::iOS       , { 8,  1                   }, avs( 8,  1)};
	const info_data       iOS_9_0               { platform::iOS       , { 9,  0                   }, avs( 9,  0)};
	const info_data       iOS_9_1               { platform::iOS       , { 9,  1                   }, avs( 9,  1)};
	const info_data       iOS_9_2               { platform::iOS       , { 9,  2                   }, avs( 9,  2)};
	const info_data       iOS_9_3               { platform::iOS       , { 9,  3                   }, avs( 9,  3)};
	const info_data       iOS_9_4               { platform::iOS       , { 9,  4                   }, avs( 9,  4)};
	const info_data       iOS_10_0              { platform::iOS       , {10,  0                   }, avs(10,  0)};
	const info_data       iOS_10_1              { platform::iOS       , {10,  1                   }, avs(10,  1)};
	const info_data       iOS_10_2              { platform::iOS       , {10,  2                   }, avs(10,  2)};
	const info_data       iOS_10_3              { platform::iOS       , {10,  3                   }, avs(10,  3)};
	const info_data       iOS_11_0              { platform::iOS       , {11,  0                   }, avs(11,  0)};
	const info_data       iOS_11_1              { platform::iOS       , {11,  1                   }, avs(11,  1)};
	const info_data       iOS_11_2              { platform::iOS       , {11,  2                   }, avs(11,  2)};
	const info_data       iOS_11_3              { platform::iOS       , {11,  3                   }, avs(11,  3)};
	const info_data       iOS_12_0              { platform::iOS       , {12,  0                   }, avs(12,  0)};
	const info_data       iOS_12_1              { platform::iOS       , {12,  1                   }, avs(12,  1)};
	const info_data       iOS_12_2              { platform::iOS       , {12,  2                   }, avs(12,  2)};
	const info_data       iOS_12_3              { platform::iOS       , {12,  3                   }, avs(12,  3)};
	const info_data       iOS_12_4              { platform::iOS       , {12,  4                   }, avs(12,  4)};
	const info_data       iOS_13_0              { platform::iOS       , {13,  0                   }, avs(13,  0)};
	const info_data       iOS_13_1              { platform::iOS       , {13,  1                   }, avs(13,  1)};
	const info_data       iOS_13_2              { platform::iOS       , {13,  2                   }, avs(13,  2)};
	const info_data       iOS_13_3              { platform::iOS       , {13,  3                   }, avs(13,  3)};
	const info_data       iOS_13_4              { platform::iOS       , {13,  4                   }, avs(13,  4)};
	const info_data       iOS_13_5              { platform::iOS       , {13,  5                   }, avs(13,  5)};
	const info_data       iOS_13_6              { platform::iOS       , {13,  6                   }, avs(13,  6)};
	const info_data       iOS_13_7              { platform::iOS       , {13,  7                   }, avs(13,  7)};
	const info_data       iOS_14_0              { platform::iOS       , {14,  0                   }, avs(14,  0)};
	const info_data       iOS_14_1              { platform::iOS       , {14,  1                   }, avs(14,  1)};
	const info_data       iOS_14_2              { platform::iOS       , {14,  2                   }, avs(14,  2)};
	const info_data       iOS_14_3              { platform::iOS       , {14,  3                   }, avs(14,  3)};
	const info_data       iOS_14_4              { platform::iOS       , {14,  4                   }, avs(14,  4)};
	const info_data       iOS_14_5              { platform::iOS       , {14,  5                   }, avs(14,  5)};
	const info_data       iOS_14_6              { platform::iOS       , {14,  6                   }, avs(14,  6)};
	const info_data       iOS_14_7              { platform::iOS       , {14,  7                   }, avs(14,  7)};
	const info_data       iOS_14_8              { platform::iOS       , {14,  8                   }, avs(14,  8)};
	const info_data       iOS_15_0              { platform::iOS       , {15,  0                   }, avs(15,  0)};
	const info_data       iOS_15_1              { platform::iOS       , {15,  1                   }, avs(15,  1)};
	const info_data       iOS_15_2              { platform::iOS       , {15,  2                   }, avs(15,  2)};
	const info_data       iOS_15_3              { platform::iOS       , {15,  3                   }, avs(15,  3)};
	const info_data       iOS_15_4              { platform::iOS       , {15,  4                   }, avs(15,  4)};
	const info_data       iOS_15_5              { platform::iOS       , {15,  5                   }, avs(15,  5)};
	const info_data       iOS_15_6              { platform::iOS       , {15,  6                   }, avs(15,  6)};

	const info_data Windows_1_0                 = Windows_1_01   ;
	const info_data Windows_2_0                 = Windows_2_03   ;
	const info_data Windows_2000                = Windows_NT_5_0 ;
	const info_data Windows_XP                  = Windows_NT_5_1 ;
	const info_data Windows_XP_x64              = Windows_NT_5_2 ;
	const info_data Windows_Vista               = Windows_NT_6_0 ;
	const info_data Windows_7                   = Windows_NT_6_1 ;
	const info_data Windows_8                   = Windows_NT_6_2 ;
	const info_data Windows_8_1                 = Windows_NT_6_3 ;
	const info_data Windows_10                  = Windows_NT_10_0;
	const info_data Windows_10_1507             = Windows_NT_10_0;
	const info_data Windows_11                  = Windows_11_21H2;

	const info_data Android_1                   = Android_API_1  ;
	const info_data Android_1_1                 = Android_API_2  ;
	const info_data Android_1_5                 = Android_API_3  ;
	const info_data Android_1_6                 = Android_API_4  ;
	const info_data Android_2                   = Android_API_5  ;
	const info_data Android_2_0_1               = Android_API_6  ;
	const info_data Android_2_1                 = Android_API_7  ;
	const info_data Android_2_2                 = Android_API_8  ;
	const info_data Android_2_3                 = Android_API_9  ;
	const info_data Android_2_3_3               = Android_API_10 ;
	const info_data Android_3                   = Android_API_11 ;
	const info_data Android_3_1                 = Android_API_12 ;
	const info_data Android_3_2                 = Android_API_13 ;
	const info_data Android_4                   = Android_API_14 ;
	const info_data Android_4_0_3               = Android_API_15 ;
	const info_data Android_4_1                 = Android_API_16 ;
	const info_data Android_4_2                 = Android_API_17 ;
	const info_data Android_4_3                 = Android_API_18 ;
	const info_data Android_4_4                 = Android_API_19 ;
	const info_data Android_4_4W                = Android_API_20 ;
	const info_data Android_5                   = Android_API_21 ;
	const info_data Android_5_1                 = Android_API_22 ;
	const info_data Android_6                   = Android_API_23 ;
	const info_data Android_7                   = Android_API_24 ;
	const info_data Android_7_1                 = Android_API_25 ;
	const info_data Android_8                   = Android_API_26 ;
	const info_data Android_8_1                 = Android_API_27 ;
	const info_data Android_9                   = Android_API_28 ;
	const info_data Android_10                  = Android_API_29 ;
	const info_data Android_11                  = Android_API_30 ;
	const info_data Android_12                  = Android_API_31 ;
	const info_data Android_12L                 = Android_API_32 ;
	const info_data Android_13                  = Android_API_33 ;
	const info_data Android_Cupcake             = Android_API_3  ;
	const info_data Android_Donut               = Android_API_4  ;
	const info_data Android_Eclair              = Android_API_5  ;
	const info_data Android_Froyo               = Android_API_8  ;
	const info_data Android_Gingerbread         = Android_API_9  ;
	const info_data Android_Honeycomb           = Android_API_11 ;
	const info_data Android_Ice_Cream_Sandwich  = Android_API_14 ;
	const info_data Android_Jelly_Bean          = Android_API_16 ;
	const info_data Android_KitKat              = Android_API_19 ;
	const info_data Android_Lollipop            = Android_API_21 ;
	const info_data Android_Marshmallow         = Android_API_23 ;
	const info_data Android_Nougat              = Android_API_24 ;
	const info_data Android_Oreo                = Android_API_26 ;
	const info_data Android_Pie                 = Android_API_28 ;

	const info_data Mac_OS_X_Jaguar             = Mac_OS_X_10_2  ;
	const info_data Mac_OS_X_Panther            = Mac_OS_X_10_3  ;
	const info_data Mac_OS_X_Tiger              = Mac_OS_X_10_4  ;
	const info_data Mac_OS_X_Leopard            = Mac_OS_X_10_5  ;
	const info_data Mac_OS_X_Snow_Leopard       = Mac_OS_X_10_6  ;
	const info_data Mac_OS_X_Lion               = Mac_OS_X_10_7  ;
	const info_data     OS_X_Mountain_Lion      =     OS_X_10_8  ;
	const info_data     OS_X_Mavericks          =     OS_X_10_9  ;
	const info_data     OS_X_Yosemite           =     OS_X_10_10 ;
	const info_data     OS_X_El_Capitan         =     OS_X_10_11 ;
	const info_data    macOS_Sierra             =    macOS_10_12 ;
	const info_data    macOS_High_Sierra        =    macOS_10_13 ;
	const info_data    macOS_Mojave             =    macOS_10_14 ;
	const info_data    macOS_Catalina           =    macOS_10_15 ;
	const info_data    macOS_Big_Sur            =    macOS_11    ;
	const info_data    macOS_Monterey           =    macOS_12    ;
	
	const info_data iPadOS_13_1                 = iOS_13_1       ;
	const info_data iPadOS_13_2                 = iOS_13_2       ;
	const info_data iPadOS_13_3                 = iOS_13_3       ;
	const info_data iPadOS_13_4                 = iOS_13_4       ;
	const info_data iPadOS_13_5                 = iOS_13_5       ;
	const info_data iPadOS_13_6                 = iOS_13_6       ;
	const info_data iPadOS_13_7                 = iOS_13_7       ;
	const info_data iPadOS_14_0                 = iOS_14_0       ;
	const info_data iPadOS_14_1                 = iOS_14_1       ;
	const info_data iPadOS_14_2                 = iOS_14_2       ;
	const info_data iPadOS_14_3                 = iOS_14_3       ;
	const info_data iPadOS_14_4                 = iOS_14_4       ;
	const info_data iPadOS_14_5                 = iOS_14_5       ;
	const info_data iPadOS_14_6                 = iOS_14_6       ;
	const info_data iPadOS_14_7                 = iOS_14_7       ;
	const info_data iPadOS_14_8                 = iOS_14_8       ;
	const info_data iPadOS_15_0                 = iOS_15_0       ;
	const info_data iPadOS_15_1                 = iOS_15_1       ;
	const info_data iPadOS_15_2                 = iOS_15_2       ;
	const info_data iPadOS_15_3                 = iOS_15_3       ;
	const info_data iPadOS_15_4                 = iOS_15_4       ;
	const info_data iPadOS_15_5                 = iOS_15_5       ;
	const info_data iPadOS_15_6                 = iOS_15_6       ;
}
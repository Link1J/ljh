
//          Copyright Jared Irwin 2021-2023
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// undocumented.hpp - v1.0
// SPDX-License-Identifier: BSL-1.0
//
// Requires C++20
// Requires delay_loaded_functions.hpp
//
// Based on code from https://gist.github.com/sylveon/9c199bb6684fe7dffcba1e3d383fb609
// Based on code from https://github.com/ysc3839/win32-darkmode
//
// ABOUT
//     Undocumented windows functions. 
//
// USAGE
//
// Version History
//     1.0 Inital Version


#pragma once

#include <windows.h>
#include <uxtheme.h>

#include <ljh/delay_loaded_functions.hpp>

enum IMMERSIVE_HC_CACHE_MODE
{
    IHCM_USE_CACHED_VALUE,
    IHCM_REFRESH
};

// 1903 18362
enum PreferredAppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};

enum WINDOWCOMPOSITIONATTRIB
{
    WCA_UNDEFINED                     = 0,
    WCA_NCRENDERING_ENABLED           = 1,
    WCA_NCRENDERING_POLICY            = 2,
    WCA_TRANSITIONS_FORCEDISABLED     = 3,
    WCA_ALLOW_NCPAINT                 = 4,
    WCA_CAPTION_BUTTON_BOUNDS         = 5,
    WCA_NONCLIENT_RTL_LAYOUT          = 6,
    WCA_FORCE_ICONIC_REPRESENTATION   = 7,
    WCA_EXTENDED_FRAME_BOUNDS         = 8,
    WCA_HAS_ICONIC_BITMAP             = 9,
    WCA_THEME_ATTRIBUTES              = 10,
    WCA_NCRENDERING_EXILED            = 11,
    WCA_NCADORNMENTINFO               = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW     = 13,
    WCA_VIDEO_OVERLAY_ACTIVE          = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK                 = 16,
    WCA_CLOAK                         = 17,
    WCA_CLOAKED                       = 18,
    WCA_ACCENT_POLICY                 = 19,
    WCA_FREEZE_REPRESENTATION         = 20,
    WCA_EVER_UNCLOAKED                = 21,
    WCA_VISUAL_OWNER                  = 22,
    WCA_HOLOGRAPHIC                   = 23,
    WCA_EXCLUDED_FROM_DDA             = 24,
    WCA_PASSIVEUPDATEMODE             = 25,
    WCA_USEDARKMODECOLORS             = 26,
    WCA_LAST                          = 27
};

enum ACCENT_STATE
{
    ACCENT_DISABLED                   = 0, // Default value. Background is black.
    ACCENT_ENABLE_GRADIENT            = 1, // Background is GradientColor, alpha channel ignored.
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2, // Background is GradientColor.
    ACCENT_ENABLE_BLURBEHIND          = 3, // Background is GradientColor, with blur effect.
    ACCENT_ENABLE_ACRYLICBLURBEHIND   = 4, // Background is GradientColor, with acrylic blur effect.
    ACCENT_ENABLE_HOSTBACKDROP        = 5, // Enables Host Backdrop brush.
    ACCENT_INVALID_STATE              = 6  // Unknown. Seems to draw background fully transparent.
};

struct ACCENT_POLICY
{
    ACCENT_STATE AccentState;
    UINT         AccentFlags;
    COLORREF     GradientColor;
    LONG         AnimationId;
};

struct WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPOSITIONATTRIB Attrib;
    void*                   pvData;
    size_t                  cbData;
};

ljh::delay_load::function<"user32.dll", "SetWindowCompositionAttribute", BOOL(WINAPI)(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA*)> SetWindowCompositionAttribute;

// 1809 17763
ljh::delay_load::ordinal<"uxtheme.dll", 132, bool(WINAPI)()>                                 ShouldAppsUseDarkMode;
ljh::delay_load::ordinal<"uxtheme.dll", 133, bool(WINAPI)(HWND hWnd, bool allow)>            AllowDarkModeForWindow;
ljh::delay_load::ordinal<"uxtheme.dll", 135, bool(WINAPI)(bool allow)>                       AllowDarkModeForApp;
ljh::delay_load::ordinal<"uxtheme.dll", 136, void(WINAPI)()>                                 FlushMenuThemes;
ljh::delay_load::ordinal<"uxtheme.dll", 104, void(WINAPI)()>                                 RefreshImmersiveColorPolicyState;
ljh::delay_load::ordinal<"uxtheme.dll", 137, bool(WINAPI)(HWND hWnd)>                        IsDarkModeAllowedForWindow;
ljh::delay_load::ordinal<"uxtheme.dll", 106, bool(WINAPI)(IMMERSIVE_HC_CACHE_MODE mode)>     GetIsImmersiveColorUsingHighContrast;
ljh::delay_load::ordinal<"uxtheme.dll", 49, HTHEME(WINAPI)(HWND hWnd, LPCWSTR pszClassList)> OpenNcThemeData;

// 1903 18362
ljh::delay_load::ordinal<"uxtheme.dll", 138, bool(WINAPI)()>                                     ShouldSystemUseDarkMode;
ljh::delay_load::ordinal<"uxtheme.dll", 135, PreferredAppMode(WINAPI)(PreferredAppMode appMode)> SetPreferredAppMode;
ljh::delay_load::ordinal<"uxtheme.dll", 139, bool(WINAPI)()>                                     IsDarkModeAllowedForApp;
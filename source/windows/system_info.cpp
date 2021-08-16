
//          Copyright Jared Irwin 2020-2021
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/system_info.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winternl.h>
#include "ljh/windows/registry.hpp"
#include "ljh/function_pointer.hpp"
#if defined(LJH_TARGET_Windows_UWP)
using NTSTATUS = unsigned int;
#endif

static ljh::function_pointer<NTSTATUS WINAPI(POSVERSIONINFOW            )> RtlGetVersion        ;
static ljh::function_pointer<const char*    (                           )> wine_get_version     ;
static ljh::function_pointer<const char*    (                           )> wine_get_build_id    ;
static ljh::function_pointer<void           (const char**, const char **)> wine_get_host_version;

#if defined(LJH_TARGET_Windows_UWP)
static ljh::function_pointer<HMODULE WINAPI(LPCSTR)> LoadLibraryA = []{
	MEMORY_BASIC_INFORMATION info = {};
	VirtualQuery(VirtualQuery, &info, sizeof(info));
	return (decltype(LoadLibraryA)::type)GetProcAddress((HMODULE)info.AllocationBase, "LoadLibraryA");
}();
#endif

static ljh::windows::registry::key CurrentVersion;

using namespace ljh::int_types;

static void system_alerts();
static void init_static()
{
	static bool setup = false;
	if (setup) { return; }
	setup = true;

	HMODULE ntdll  = LoadLibraryA("ntdll.dll");

	RtlGetVersion         = GetProcAddress(ntdll , "RtlGetVersion"        );
	wine_get_version      = GetProcAddress(ntdll , "wine_get_version"     );
	wine_get_build_id     = GetProcAddress(ntdll , "wine_get_build_id"    );
	wine_get_host_version = GetProcAddress(ntdll , "wine_get_host_version");

	CurrentVersion = ljh::windows::registry::key::LOCAL_MACHINE[L"SOFTWARE"][L"Microsoft"][L"Windows NT"][L"CurrentVersion"];

	system_alerts();
}

static std::string to_utf8(std::wstring_view string)
{
	std::string output;
	int size = WideCharToMultiByte(CP_UTF8, 0, string.data(), -1, NULL, 0, NULL, NULL);
	output.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, string.data(), -1, (char*)output.data(), size, NULL, NULL);
	output.resize(size - 1);
	return output;
}

ljh::expected<ljh::system_info::platform, ljh::system_info::error> ljh::system_info::get_platform()
{
	init_static();
	return platform::Windows;
}

ljh::expected<ljh::version, ljh::system_info::error> ljh::system_info::get_version()
{
	init_static();
	version::value_type v_patch = 0;
	
	if (auto UBR = CurrentVersion.get_value(L"UBR"); UBR.has_value())
		v_patch = (version::value_type)UBR->get<DWORD>();

	OSVERSIONINFOW osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(osinfo);
	if (RtlGetVersion) { RtlGetVersion(&osinfo); }
	else               { GetVersionExW(&osinfo); }

	return version{osinfo.dwMajorVersion, osinfo.dwMinorVersion, osinfo.dwBuildNumber, v_patch};
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_string()
{
	init_static();
	std::string string = "";

	if (wine_get_version)
	{
		const char* sysname,* release;
		wine_get_host_version(&sysname, &release);
		string = std::string{wine_get_build_id()} + " on " + sysname + " " + release;
	}

	if (auto SYSTEM = windows::registry::key::LOCAL_MACHINE.get_key(L"SYSTEM"); SYSTEM.has_value())
		if (auto CurrentControlSet = SYSTEM->get_key(L"CurrentControlSet"); CurrentControlSet.has_value())
			if (auto Control = CurrentControlSet->get_key(L"Control"); Control.has_value())
				if (auto ReactOS = Control->get_key(L"ReactOS"); ReactOS.has_value())
				{
					// This works in some versions
					OSVERSIONINFOA osinfo;
					osinfo.dwOSVersionInfoSize = sizeof(osinfo);
					GetVersionExA(&osinfo);
					if (osinfo.szCSDVersion[strlen(osinfo.szCSDVersion) + 1] == L'R')
						string = &osinfo.szCSDVersion[strlen(osinfo.szCSDVersion) + 1 + 8];
					else
						string = "Unknown ReactOS Version";
				}

	if (!string.empty()) { string += " mimicking "; }

	auto os_name = to_utf8((std::wstring)CurrentVersion(L"ProductName"));

	if (os_name.starts_with("Microsoft "))
		os_name = os_name.substr(10);

	auto version = *ljh::system_info::get_version();
	auto line = os_name.find("Windows ") + 8;
	if (version.major() >= 10)
	{
		if (version.build() > 21390 && line != std::string::npos)
		{
			os_name.replace(line, 2, "11");
		}
		if (version.build() != 22000)
		{
			line += 2;
			if (auto version_display = CurrentVersion.get_value(L"DisplayVersion"); version_display.has_value())
				os_name = os_name.substr(0, line + 1) + to_utf8(version_display->get<std::wstring>()) + os_name.substr(line);
			else if (version_display = CurrentVersion.get_value(L"ReleaseId"); version_display.has_value())
				os_name = os_name.substr(0, line + 1) + to_utf8(version_display->get<std::wstring>()) + os_name.substr(line);
		}
	}
	else
	{
		line = os_name.find_first_of(' ', line + 1);
		if (auto version_display = CurrentVersion.get_value(L"CSDVersion"); version_display.has_value())
			os_name = os_name.substr(0, line + 1) + to_utf8(version_display->get<std::wstring>()) + os_name.substr(line);
	}
	string += os_name;

	return string;
}

ljh::expected<ljh::u32, ljh::system_info::error> ljh::system_info::get_sdk()
{
	init_static();
	auto info_version = get_version();
	if (!info_version)
		return unexpected{info_version.error()};
	return info_version->build();
}

static auto system_info_reg()
{
	if (*ljh::system_info::get_sdk() >= ljh::system_info::Windows_7)
		return ljh::windows::registry::key::LOCAL_MACHINE[L"HARDWARE"][L"DESCRIPTION"][L"System"][L"BIOS"];
	else
		return ljh::windows::registry::key::LOCAL_MACHINE[L"SYSTEM"][L"CurrentControlSet"][L"Control"][L"SystemInformation"];
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_model()
{
	init_static();
	std::string string;
	auto key = system_info_reg();
	if (auto info = key.get_value(L"SystemFamily"); info.has_value())
		string += to_utf8(info->get<std::wstring>()) + " ";
	return string + to_utf8((std::wstring)key(L"SystemProductName"));
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_manufacturer()
{
	init_static();
	return to_utf8((std::wstring)system_info_reg()(L"SystemManufacturer"));
}

static void system_alerts()
{
#undef MessageBox

	auto MessageBox = ljh::function_pointer<int WINAPI(HWND, LPCSTR, LPCSTR, UINT)>(GetProcAddress(LoadLibraryA("user32.dll"), "MessageBoxA"));
	[[unlikely]] if (MessageBox.empty()) return; // It isn't really a thing in UWP. But we don't care, so this should never happen.

	// Windows 10 Threshold
	auto version = *ljh::system_info::get_version();
	[[unlikely]] if (version.major() < 10 && version.build() > 9600)
	{
		std::string message_text = 
			"Windows " + std::string{version} + 
			" is not a version of Windows that should be used."
			" Please install a real version of Windows 10.";
		MessageBox(NULL, message_text.c_str(), "Not Windows 10", MB_OK | MB_TASKMODAL | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
	}

	// Windows Longhorn
	[[unlikely]] if (version.major() >= 6 && version.minor() == 0 && version.build() < 5000)
	{
		std::string message_text = 
			"Windows Vista's pre-reset builds are buggy and broken.\n"
			"Please install version of Windows that works correctly.";
		MessageBox(NULL, message_text.c_str(), "Not Windows Vista", MB_OK | MB_TASKMODAL | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
	}
	[[likely]] if (auto build_lab = CurrentVersion.get_value(L"BuildLab"); build_lab.has_value())
		[[unlikely]] if (build_lab->get<std::wstring>() == L"5.2.3790.1232.winmain.040819-1629")
		{
			MessageBox(NULL, "Why are you using this build?", "Really?", MB_OK | MB_TASKMODAL | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
			ExitProcess(1);
		}

	// Windows Neptune
	[[unlikely]] if (version.major() < 6 && version.build() >= 5000)
	{
		std::string message_text = 
			"Windows Neptune is not a version of Windows that should be used.\n"
			"Please install version of Windows that works correctly.";
		MessageBox(NULL, message_text.c_str(), "Windows Neptune", MB_OK | MB_TASKMODAL | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
	}

	// Windows 11 Leak
	[[unlikely]] if (version.major() == 10 && version.build() > 21390 && version.build() < 22000)
	{
		std::string message_text = 
			"Why are you using a leaked version of Windows 11 still?\n"
			"Please just install Windows 11 itself.";
		MessageBox(NULL, message_text.c_str(), "No Windows 11 Leaks", MB_OK | MB_TASKMODAL | MB_DEFAULT_DESKTOP_ONLY | MB_ICONERROR);
	}
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

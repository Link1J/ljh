#include "ljh/system_info.hpp"
#include "ljh/windows/registry.hpp"
#include <iostream>

int main()
{
	auto hel_platfrom = []() -> std::string
	{
		switch (*ljh::system_info::get_platform())
		{
			case ljh::system_info::platform::Windows: return "Windows";
			case ljh::system_info::platform::Unix   : return "Unix"   ;
			case ljh::system_info::platform::macOS  : return "macOS"  ;
			case ljh::system_info::platform::iOS    : return "iOS"    ;
			case ljh::system_info::platform::Linux  : return "Linux"  ;
			case ljh::system_info::platform::Android: return "Android";
		}
	};

	std::cout << "platform    : " <<                    hel_platfrom    () << "\n";
	std::cout << "version     : " << *ljh::system_info::get_version     () << "\n";
	std::cout << "string      : " << *ljh::system_info::get_string      () << "\n";
	std::cout << "sdk         : " << *ljh::system_info::get_sdk         () << "\n";
	std::cout << "model       : " << *ljh::system_info::get_model       () << "\n";
	std::cout << "manufacturer: " << *ljh::system_info::get_manufacturer() << "\n";

	std::cout << (ljh::system_info::iPhone_OS_1_0 .version.major() << 16 | ljh::system_info::iPhone_OS_1_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_1_1 .version.major() << 16 | ljh::system_info::iPhone_OS_1_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_2_0 .version.major() << 16 | ljh::system_info::iPhone_OS_2_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_2_1 .version.major() << 16 | ljh::system_info::iPhone_OS_2_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_2_2 .version.major() << 16 | ljh::system_info::iPhone_OS_2_2 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_3_0 .version.major() << 16 | ljh::system_info::iPhone_OS_3_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_3_1 .version.major() << 16 | ljh::system_info::iPhone_OS_3_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::iPhone_OS_3_2 .version.major() << 16 | ljh::system_info::iPhone_OS_3_2 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_4_0 .version.major() << 16 | ljh::system_info::      iOS_4_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_4_1 .version.major() << 16 | ljh::system_info::      iOS_4_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_5_0 .version.major() << 16 | ljh::system_info::      iOS_5_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_5_1 .version.major() << 16 | ljh::system_info::      iOS_5_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_6_0 .version.major() << 16 | ljh::system_info::      iOS_6_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_6_1 .version.major() << 16 | ljh::system_info::      iOS_6_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_7_0 .version.major() << 16 | ljh::system_info::      iOS_7_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_7_1 .version.major() << 16 | ljh::system_info::      iOS_7_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_8_0 .version.major() << 16 | ljh::system_info::      iOS_8_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_8_1 .version.major() << 16 | ljh::system_info::      iOS_8_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_9_0 .version.major() << 16 | ljh::system_info::      iOS_9_0 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_9_1 .version.major() << 16 | ljh::system_info::      iOS_9_1 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_9_2 .version.major() << 16 | ljh::system_info::      iOS_9_2 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_9_3 .version.major() << 16 | ljh::system_info::      iOS_9_3 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_9_4 .version.major() << 16 | ljh::system_info::      iOS_9_4 .version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_10_0.version.major() << 16 | ljh::system_info::      iOS_10_0.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_10_1.version.major() << 16 | ljh::system_info::      iOS_10_1.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_10_2.version.major() << 16 | ljh::system_info::      iOS_10_2.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_10_3.version.major() << 16 | ljh::system_info::      iOS_10_3.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_11_0.version.major() << 16 | ljh::system_info::      iOS_11_0.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_11_1.version.major() << 16 | ljh::system_info::      iOS_11_1.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_11_2.version.major() << 16 | ljh::system_info::      iOS_11_2.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_11_3.version.major() << 16 | ljh::system_info::      iOS_11_3.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_12_0.version.major() << 16 | ljh::system_info::      iOS_12_0.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_12_1.version.major() << 16 | ljh::system_info::      iOS_12_1.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_12_2.version.major() << 16 | ljh::system_info::      iOS_12_2.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_12_3.version.major() << 16 | ljh::system_info::      iOS_12_3.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_12_4.version.major() << 16 | ljh::system_info::      iOS_12_4.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_0.version.major() << 16 | ljh::system_info::      iOS_13_0.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_1.version.major() << 16 | ljh::system_info::      iOS_13_1.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_2.version.major() << 16 | ljh::system_info::      iOS_13_2.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_3.version.major() << 16 | ljh::system_info::      iOS_13_3.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_4.version.major() << 16 | ljh::system_info::      iOS_13_4.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_5.version.major() << 16 | ljh::system_info::      iOS_13_5.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_6.version.major() << 16 | ljh::system_info::      iOS_13_6.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_13_7.version.major() << 16 | ljh::system_info::      iOS_13_7.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_14_0.version.major() << 16 | ljh::system_info::      iOS_14_0.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_14_1.version.major() << 16 | ljh::system_info::      iOS_14_1.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_14_2.version.major() << 16 | ljh::system_info::      iOS_14_2.version.minor() << 8) << "\n";
	std::cout << (ljh::system_info::      iOS_14_3.version.major() << 16 | ljh::system_info::      iOS_14_3.version.minor() << 8) << "\n";

	return 0;
}

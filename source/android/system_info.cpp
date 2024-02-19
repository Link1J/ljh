
//          Copyright Jared Irwin 2020-2024
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "ljh/system_info.hpp"
#include "ljh/char_convertions.hpp"
#include <sys/system_properties.h>

using namespace ljh::int_types;

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#endif

static std::string get_system_prop(std::string_view string)
{
	char osVersion[PROP_VALUE_MAX+1];
	unsigned int osVersionLength = __system_property_get(string.data(), osVersion);
	return std::string{osVersion, osVersionLength};
}

ljh::expected<ljh::system_info::platform, ljh::system_info::error> ljh::system_info::get_platform()
{
	return platform::Android;
}

ljh::expected<ljh::version, ljh::system_info::error> ljh::system_info::get_version()
{
	return get_system_prop("ro.build.version.release");
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_string()
{
	std::string string = std::string{"Android "} + (std::string)*get_version();
	if (auto sdk = *get_sdk(); sdk >= 29) {}
	else if (sdk >= Android_Pie               ) { string += " Pie"               ; }
	else if (sdk >= Android_Oreo              ) { string += " Oreo"              ; }
	else if (sdk >= Android_Nougat            ) { string += " Nougat"            ; }
	else if (sdk >= Android_Marshmallow       ) { string += " Marshmallow"       ; }
	else if (sdk >= Android_Lollipop          ) { string += " Lollipop"          ; }
	else if (sdk >= Android_KitKat            ) { string += " KitKat"            ; }
	else if (sdk >= Android_Jelly_Bean        ) { string += " Jelly_Bean"        ; }
	else if (sdk >= Android_Ice_Cream_Sandwich) { string += " Ice_Cream_Sandwich"; }
	else if (sdk >= Android_Honeycomb         ) { string += " Honeycomb"         ; }
	else if (sdk >= Android_Gingerbread       ) { string += " Gingerbread"       ; }
	else if (sdk >= Android_Froyo             ) { string += " Froyo"             ; }
	else if (sdk >= Android_Eclair            ) { string += " Eclair"            ; }
	else if (sdk >= Android_Donut             ) { string += " Donut"             ; }
	else if (sdk >= Android_Cupcake           ) { string += " Cupcake"           ; }
	return string;
}

ljh::expected<ljh::u32, ljh::system_info::error> ljh::system_info::get_sdk()
{
	u32 value;
	from_string(value, get_system_prop("ro.build.version.sdk"));
	return value;
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_model()
{
	return get_system_prop("ro.product.model");
}

ljh::expected<std::string, ljh::system_info::error> ljh::system_info::get_manufacturer()
{
	return get_system_prop("ro.product.manufacturer");
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

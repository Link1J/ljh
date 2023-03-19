
#          Copyright Jared Irwin 2020-2023
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

project(ljh_tests LANGUAGES CXX VERSION 0.0.0)
include(FetchContent)

if(POLICY CMP0110)
	cmake_policy(SET CMP0110 NEW)
endif()

set(CATCH_BUILD_TESTING OFF)

FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)
FetchContent_GetProperties(Catch2 SOURCE_DIR CATCH2_SOURCE)
set(CMAKE_MODULE_PATH ${CATCH2_SOURCE}/extras ${CMAKE_MODULE_PATH})

if (MSVC)
	add_compile_options(
		/D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
		/Zc:twoPhase-
		$<$<NOT:$<BOOL:${AWAIT_NOT_NEEDED}>>:/await>
		/Zf /utf-8 /permissive- /Zc:inline /Zc:__cplusplus /Zc:__STDC__ /bigobj /Y-
	)
endif()

add_executable(tests_11
	system_directories.98.cpp
	function_pointer.11.cpp
	string_utils.11.cpp
)
set_target_properties(tests_11 PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_11 Catch2::Catch2WithMain ljh)
target_compile_definitions(tests_11 PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=500)

add_executable(tests_14
	compile_time_string.14.cpp
	char_convertions.14.cpp
)
set_target_properties(tests_14 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_14 Catch2::Catch2WithMain ljh)
target_compile_definitions(tests_14 PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=500)

add_executable(tests_17
	version.17.cpp
	memory_mapped_file.17.cpp
	function_pointer.17.cpp
	expected.17.cpp
	system_info.17.cpp
	string_utils.17.cpp
)
if (UNIX AND NOT APPLE)
	target_sources(tests_17 PRIVATE
		dbus.17.cpp
	)
endif()

set_target_properties(tests_17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_17 Catch2::Catch2WithMain ljh)
target_compile_definitions(tests_17 PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=500)

add_executable(tests_20
	expected.20.cpp
	version.20.cpp
	compile_time_string.20.cpp
	delay_loaded_functions.20.cpp
	concepts.20.cpp
	ranges.20.cpp
)
set_target_properties(tests_20 PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_20 Catch2::Catch2WithMain ljh)
target_compile_definitions(tests_20 PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=500)

add_executable(tests_info
	info.na.cpp
)
set_target_properties(tests_info PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED NO CXX_EXTENSIONS NO)
target_link_libraries(tests_info Catch2::Catch2 ljh)
target_compile_definitions(tests_info PRIVATE CATCH_CONFIG_CONSOLE_WIDTH=500)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(tests_11 PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS=ON)
ParseAndAddCatchTests(tests_14 PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS=ON)
ParseAndAddCatchTests(tests_17 PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS=ON)
ParseAndAddCatchTests(tests_20 PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS=ON)

#          Copyright Jared Irwin 2020-2024
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

project(ljh_tests LANGUAGES CXX VERSION 0.0.0)
include(FetchContent)

if(POLICY CMP0110)
	cmake_policy(SET CMP0110 NEW)
endif()

set(CATCH_CONFIG_CONSOLE_WIDTH 160)
set(CATCH_BUILD_TESTING OFF)

FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.7.1
)
FetchContent_Declare(fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG        10.2.1
)
FetchContent_MakeAvailable(Catch2 fmt)
FetchContent_GetProperties(Catch2 SOURCE_DIR CATCH2_SOURCE)
set(CMAKE_MODULE_PATH ${CATCH2_SOURCE}/extras ${CMAKE_MODULE_PATH})

if (MSVC)
	add_compile_options(
		/D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
		/Zc:twoPhase- /await:strict /utf-8 /permissive- /Zc:inline /Zc:__cplusplus /Zc:__STDC__ /bigobj /Y-
	)
endif()

add_executable(tests_11
	system_directories.98.cpp
	function_pointer.11.cpp
	string_utils.11.cpp
	defer.11.cpp
	function_traits.na.cpp
)
set_target_properties(tests_11 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_11 Catch2::Catch2WithMain ljh)

add_executable(tests_14
	compile_time_string.14.cpp
	char_convertions.14.cpp
)
set_target_properties(tests_14 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_14 Catch2::Catch2WithMain ljh)

add_executable(tests_17
	version.17.cpp
	memory_mapped_file.17.cpp
	function_pointer.17.cpp
	expected.17.cpp
	system_info.17.cpp
	string_utils.17.cpp
	enum_array.17.cpp
	defer.11.cpp
	function_traits.na.cpp
)
set_target_properties(tests_17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_17 Catch2::Catch2WithMain ljh)

add_executable(tests_20
	expected.17.cpp
	version.20.cpp
	compile_time_string.20.cpp
	delay_loaded_functions.20.cpp
	concepts.20.cpp
	ranges.20.cpp
	generator.20.cpp
	coroutine.20.cpp
)
set_target_properties(tests_20 PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_20 Catch2::Catch2WithMain ljh fmt::fmt)

try_compile(SUPPORTS_explicit_this
	"${PROJECT_BINARY_DIR}/_build_tests"
	"${PROJECT_SOURCE_DIR}/cmake.__cpp_explicit_this_parameter.cpp"
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

add_executable(tests_23
	empty.na.cpp
)
if (SUPPORTS_explicit_this)
	target_sources(tests_23 PRIVATE
		smarc.23.cpp
		rect.23.cpp
		region.23.cpp
		size.23.cpp
		point.23.cpp
	)
endif()
set_target_properties(tests_23 PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_23 Catch2::Catch2WithMain ljh fmt::fmt)

add_executable(tests_info
	info.na.cpp
)
set_target_properties(tests_info PROPERTIES CXX_STANDARD 26 CXX_STANDARD_REQUIRED NO CXX_EXTENSIONS NO)

if (TARGET_ARCH EQUAL CMAKE_HOST_SYSTEM_PROCESSOR)
include(CTest)
include(Catch)
catch_discover_tests(tests_11)
catch_discover_tests(tests_14)
catch_discover_tests(tests_17)
catch_discover_tests(tests_20)
catch_discover_tests(tests_23)
endif ()
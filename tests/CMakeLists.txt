cmake_minimum_required(VERSION 3.5)
project(ljh_tests LANGUAGES CXX VERSION 0.0.0)
include(FetchContent)

FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)
FetchContent_GetProperties(Catch2 SOURCE_DIR CATCH2_SOURCE)
set(CMAKE_MODULE_PATH ${CATCH2_SOURCE}/extras ${CMAKE_MODULE_PATH})

add_executable(tests_11
	system_directories.98.cpp
)
set_target_properties(tests_11 PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_11 Catch2::Catch2WithMain ljh)

add_executable(tests_17
	version.17.cpp
	memory_mapped_file.17.cpp
)
set_target_properties(tests_17 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_17 Catch2::Catch2WithMain ljh)

add_executable(tests_20
	expected.20.cpp
	version.20.cpp
)
set_target_properties(tests_20 PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_link_libraries(tests_20 Catch2::Catch2WithMain ljh)

include(CTest)
include(Catch)
catch_discover_tests(tests_11)
catch_discover_tests(tests_17)
catch_discover_tests(tests_20)
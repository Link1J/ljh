cmake_minimum_required(VERSION 3.11.0)
option(ljh_BUILD_TESTS "Build the Self Tests" OFF)
project(ljh)

add_library(ljh INTERFACE)
target_include_directories(ljh
	INTERFACE
		include
)

add_library(system_directories STATIC source/system_directories.cpp)
target_include_directories(system_directories PRIVATE include/ljh)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (ljh_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
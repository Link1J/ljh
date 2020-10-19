cmake_minimum_required(VERSION 3.11.0)

option(ljh_BUILD_TESTS "Build the Self Tests" OFF)

project(ljh)
add_library(ljh INTERFACE)
target_include_directories(ljh INTERFACE include)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (ljh_BUILD_TESTS)
	add_subdirectory(tests)
endif()
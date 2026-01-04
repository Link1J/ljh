
#          Copyright Jared Irwin 2020-2026
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.26.0)
set(CMAKE_SYSTEM_VERSION 10.0)
project(ljh)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
set(LJH_SUB_PROJECT ON)
else()
set(LJH_SUB_PROJECT OFF)
endif()

if (NOT CMAKE_VS_PLATFORM_NAME STREQUAL "")
set(TARGET_ARCH ${CMAKE_VS_PLATFORM_NAME})
else ()
set(TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif ()

if (TARGET_ARCH STREQUAL "ARM64" OR TARGET_ARCH STREQUAL "ARM64EC")
set(IS_ARM64 ON)
else ()
set(IS_ARM64 OFF)
endif ()

try_compile(AWAIT_NOT_NEEDED
	SOURCES "${PROJECT_SOURCE_DIR}/tests/cmake.cppwinrt_await.cpp"
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

block(SCOPE_FOR VARIABLES PROPAGATE MODULES_ARE_SUPPORTED)
	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
	try_compile(MODULES_ARE_SUPPORTED
		SOURCES_TYPE CXX_MODULE
		SOURCES "${PROJECT_SOURCE_DIR}/tests/cmake.modules_support.ixx"
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)
endblock()

option(ljh_BUILD_TESTS "Build the Self Tests" ${LJH_SUB_PROJECT})
option(ljh_INSTALL "Install" ${LJH_SUB_PROJECT})
option(ljh_BUILD_MODULES "Build LJH Modules" ${MODULES_ARE_SUPPORTED})

message("-- ljh build modules: ${ljh_BUILD_MODULES}")

add_library(ljh STATIC
	source/system_directories.cpp
	source/memory_mapped_file.cpp
	source/os_versions.cpp
	source/dummy_system_info.cpp
)
add_library(ljh::ljh ALIAS ljh)
set_target_properties(ljh PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)
target_include_directories(ljh
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

if (ljh_BUILD_MODULES)
	set_target_properties(ljh PROPERTIES
		CXX_STANDARD 23
		CXX_SCAN_FOR_MODULES ON
	)
	target_sources(ljh PUBLIC FILE_SET CXX_MODULES FILES
		modules/main.ixx
		modules/smarc.ixx
		modules/string.ixx
		modules/coroutine.ixx
		modules/math.ixx
		modules/os.ixx
	)
endif ()


if (WIN32)
	target_sources(ljh PRIVATE
		source/windows/system_info.cpp
		source/windows/registry.cpp
		source/windows/wmi.cpp
		source/windows/named_mutex.cpp
		source/windows/thread_name.cpp
	)
	target_compile_options(ljh PRIVATE
		/D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
		/D_CRT_SECURE_NO_WARNINGS
		$<$<CXX_COMPILER_ID:MSVC>:/await:strict>
		$<$<CXX_COMPILER_ID:Clang>:-Wno-delayed-template-parsing-in-cxx20>
	)
	target_link_libraries(ljh PRIVATE
		wbemuuid
		#windowsapp
	)
	if (IS_ARM64)
		enable_language(ASM_MARMASM)
		add_library(ljh_asm OBJECT
			source/windows/checked_math/arm64.asm
		)
		target_sources(ljh PRIVATE $<TARGET_OBJECTS:ljh_asm>)
	endif ()
elseif (APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_sources(ljh PRIVATE
		source/macos/system_info.mm
		source/unix/named_mutex.cpp
		source/macos/thread_name.cpp
	)
	target_link_libraries(ljh PRIVATE
		"-framework CoreFoundation"
		"-framework Foundation"
		"-framework CoreServices"
		Threads::Threads
	)
elseif (UNIX)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_sources(ljh PRIVATE
		source/unix/system_info.cpp
		source/unix/named_mutex.cpp
		source/unix/thread_name.cpp
	)
	target_link_libraries(ljh PRIVATE
		dl
		Threads::Threads
	)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (ljh_BUILD_TESTS)
	add_subdirectory(tests)
endif()

if (ljh_INSTALL)
	install(TARGETS ljh EXPORT ljh
		LIBRARY              DESTINATION lib
		ARCHIVE              DESTINATION lib
		RUNTIME              DESTINATION bin
		PUBLIC_HEADER        DESTINATION include
		BUNDLE               DESTINATION bin
		FILE_SET CXX_MODULES DESTINATION lib/cmake/ljh/src
	)
	install(EXPORT ljh NAMESPACE ljh:: DESTINATION lib/cmake/ljh FILE ljh-config.cmake)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
endif()

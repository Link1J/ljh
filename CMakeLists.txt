
#          Copyright Jared Irwin 2020-2024
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.11.0)
project(ljh)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
set(LJH_SUB_PROJECT ON)
else()
set(LJH_SUB_PROJECT OFF)
endif()

option(ljh_BUILD_TESTS "Build the Self Tests" ${LJH_SUB_PROJECT})
option(ljh_INSTALL "Install" ${LJH_SUB_PROJECT})

add_library(ljh STATIC
	source/system_directories.cpp
	source/memory_mapped_file.cpp
	source/os_versions.cpp
	source/dummy_system_info.cpp
	$<$<PLATFORM_ID:Darwin>:
		source/macos/system_info.mm
		source/unix/named_mutex.cpp
		source/macos/thread_name.cpp
	>
	$<$<PLATFORM_ID:Windows>:
		source/windows/system_info.cpp
		source/windows/registry.cpp
		source/windows/wmi.cpp
		source/windows/named_mutex.cpp
		source/windows/thread_name.cpp
	>
	$<$<PLATFORM_ID:Linux>:
		source/unix/system_info.cpp
		source/unix/named_mutex.cpp
		source/unix/thread_name.cpp
	>
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

try_compile(AWAIT_NOT_NEEDED
	"${PROJECT_BINARY_DIR}/_build_tests"
	"${PROJECT_SOURCE_DIR}/tests/cmake.cppwinrt_await.cpp"
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)
message(${AWAIT_NOT_NEEDED})

if (WIN32)
	target_link_libraries(ljh PRIVATE
		wbemuuid
		#windowsapp
	)
elseif (APPLE)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(ljh PRIVATE
		"-framework CoreFoundation"
		"-framework Foundation"
		"-framework CoreServices"
		Threads::Threads
	)
elseif (UNIX)
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
	target_link_libraries(ljh PRIVATE
		dl
		Threads::Threads
	)
endif()

target_compile_options(ljh
	PRIVATE
		$<$<PLATFORM_ID:Windows,WindowsStore>:
			/D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
			/Zc:twoPhase-
			$<$<NOT:$<BOOL:${AWAIT_NOT_NEEDED}>>:/await>
		>
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (ljh_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

if (ljh_INSTALL)
	install(TARGETS ljh EXPORT ljh
		LIBRARY       DESTINATION lib
		ARCHIVE       DESTINATION lib
		RUNTIME       DESTINATION bin
		PUBLIC_HEADER DESTINATION include
		BUNDLE        DESTINATION bin
	)
	install(EXPORT ljh NAMESPACE ljh:: DESTINATION lib/cmake/ljh FILE ljh-config.cmake)
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
endif()

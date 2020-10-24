cmake_minimum_required(VERSION 3.11.0)
option(ljh_BUILD_TESTS "Build the Self Tests" OFF)
project(ljh)

add_library(ljh INTERFACE)
target_include_directories(ljh
	INTERFACE
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

add_library(system_directories STATIC source/system_directories.cpp)
target_include_directories(system_directories PRIVATE include/ljh)

add_library(complete INTERFACE)
target_link_libraries(complete
	INTERFACE
		ljh
		system_directories
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (ljh_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

install(TARGETS ljh EXPORT ljh
	LIBRARY       DESTINATION lib
	ARCHIVE       DESTINATION lib
	RUNTIME       DESTINATION bin
	PUBLIC_HEADER DESTINATION include
	BUNDLE        DESTINATION bin
)
install(TARGETS system_directories EXPORT system_directories
	LIBRARY       DESTINATION lib
	ARCHIVE       DESTINATION lib
	RUNTIME       DESTINATION bin
	PUBLIC_HEADER DESTINATION include
	BUNDLE        DESTINATION bin
)
install(TARGETS complete EXPORT all
	LIBRARY       DESTINATION lib
	ARCHIVE       DESTINATION lib
	RUNTIME       DESTINATION bin
	PUBLIC_HEADER DESTINATION include
	BUNDLE        DESTINATION bin
)

install(EXPORT ljh                NAMESPACE ljh DESTINATION lib/cmake/ljh)
install(EXPORT system_directories NAMESPACE ljh DESTINATION lib/cmake/ljh)
install(EXPORT all                NAMESPACE ljh DESTINATION lib/cmake/ljh)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
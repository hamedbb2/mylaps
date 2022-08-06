cmake_minimum_required(VERSION 3.16.3)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

project(RCSServer)

if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.2)
		message(FATAL_ERROR "GCC version at least 10.2 is required; current version is " ${CMAKE_CXX_COMPILER_VERSION})
	endif ()
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -rdynamic -Og -fno-inline")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g -rdynamic -DNDEBUG")

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(sodium_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS TRUE)

enable_testing()

set(EXTERNAL_LIB_INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/build)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

list(APPEND CMAKE_PREFIX_PATH
    ${EXTERNAL_LIB_INSTALL_DIR}/pistache
    ${EXTERNAL_LIB_INSTALL_DIR}/json
)

find_package(Pistache REQUIRED)
set_target_properties(pistache_static PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Pistache_INCLUDE_DIRS}"
)
find_package(nlohmann_json REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

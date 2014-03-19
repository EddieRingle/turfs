project(turfs C)

cmake_minimum_required(VERSION 2.8.12.2)

#
# Set up some variables
#

set(TURFS_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(TURFS_SCRIPTS_DIR "${TURFS_ROOT_DIR}/scripts")
set(TURFS_SOURCE_DIR "${TURFS_ROOT_DIR}/src")
set(TURFS_INCLUDE_DIR "${TURFS_ROOT_DIR}/include")
set(TURFS_OUT_DIR "${TURFS_ROOT_DIR}/out")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(TURFS_PLATFORM_ANDROID 1)
set(TURFS_PLATFORM_DREAMCAST 2)
set(TURFS_PLATFORM_IOS 3)
set(TURFS_PLATFORM_OSX 4)
set(TURFS_PLATFORM_WINDOWS 5)
set(TURFS_PLATFORM_UNIX 6)

if(NOT TURFS_TARGET_PLATFORM)
    if(APPLE)
        set(TURFS_TARGET_PLATFORM TURFS_TARGET_PLATFORM_OSX)
    elseif(UNIX)
        set(TURFS_TARGET_PLATFORM TURFS_TARGET_PLATFORM_UNIX)
    elseif(WIN32)
        set(TURFS_TARGET_PLATFORM TURFS_TARGET_PLATFORM_WINDOWS)
    else()
        message(FATAL_ERROR "Unknown target platform; Please set TURFS_TARGET_PLATFORM")
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${TURFS_SCRIPTS_DIR}/cmake/modules")

#
# Modules & macros
#

include(GetGitRevisionDescription)
include(GroupByFolder)
include(TargetArch)

#
# Target Arch/OS Detection
#

if(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_ANDROID)
    set(TURFS_TARGET_OS "android")
elseif(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_DREAMCAST)
    set(TURFS_TARGET_OS "dreamcast")
elseif(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_IOS)
    set(TURFS_TARGET_OS "ios")
elseif(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_OSX)
    set(TURFS_TARGET_OS "darwin")
elseif(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_UNIX)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(TURFS_TARGET_OS "linux")
    else()
        set(TURFS_TARGET_OS "unix")
    endif()
elseif(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_WINDOWS)
    set(TURFS_TARGET_OS "windows")
endif()

target_architecture(TURFS_TARGET_ARCH)

set(TURFS_TARGET_OUT_DIR "${TURFS_OUT_DIR}/${TURFS_TARGET_OS}_${TURFS_TARGET_ARCH}")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${TURFS_TARGET_OUT_DIR}" CACHE PATH "Default install path" FORCE)
endif()

#
# Generate version information from git-describe
#

git_describe(gitdesc)

if("${gitdesc}" MATCHES "NOTFOUND")
    message(FATAL_ERROR "${PROJECT_NAME} must be built within a live Git root with at least one tag")
    return()
endif()

if(gitdesc MATCHES "^v?(([0-9]+)\\.([0-9]+)\\.([0-9]+)-?([0-9]+-g[0-9a-f]+)?)$")
    set(TURFS_VERSION_FULL "${CMAKE_MATCH_1}")
    set(TURFS_VERSION_MAJOR "${CMAKE_MATCH_2}")
    set(TURFS_VERSION_MINOR "${CMAKE_MATCH_3}")
    set(TURFS_VERSION_PATCH "${CMAKE_MATCH_4}")
    set(TURFS_VERSION_EXTRA "${CMAKE_MATCH_5}")
endif()

message(STATUS "Building version: ${TURFS_VERSION_FULL}")
configure_file("${TURFS_SOURCE_DIR}/turfs_version.c.in" "${CMAKE_CURRENT_BINARY_DIR}/turfs_version.c")

#
# turfs
#

add_definitions(-DTURFS_TARGET_PLATFORM=${TURFS_TARGET_PLATFORM})
message(STATUS "TURFS_TARGET_PLATFORM = ${TURFS_TARGET_PLATFORM}")

set(turfs_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/turfs_version.c
    ${TURFS_SOURCE_DIR}/turfs_common.c
    ${TURFS_SOURCE_DIR}/turfs_strings.c
    ${TURFS_SOURCE_DIR}/internal.h
)

if(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_OSX)
    set(turfs_SOURCES
        ${turfs_SOURCES}
        ${TURFS_SOURCE_DIR}/turfs_osx.c
    )
elseif(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_UNIX)
    set(turfs_SOURCES
        ${turfs_SOURCES}
        ${TURFS_SOURCE_DIR}/turfs_unix.c
    )
endif()

# stdio platforms
if(NOT TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_ANDROID)
    set(turfs_SOURCES
        ${turfs_SOURCES}
        ${TURFS_SOURCE_DIR}/turfs_file_stdio.c
    )
endif()

set(turfs_INCLUDES
    ${TURFS_INCLUDE_DIR}/turfs.h
    ${TURFS_INCLUDE_DIR}/turfs_version.h
)

include_directories(SYSTEM ${TURFS_INCLUDE_DIR})

add_library(turfs STATIC ${turfs_SOURCES} ${turfs_INCLUDES})

add_subdirectory(examples)

install(TARGETS turfs
    ARCHIVE DESTINATION lib
)
install(FILES ${turfs_INCLUDES}
    DESTINATION include/turfs
)

group_by_folder(${TURFS_ROOT_DIR} "${turfs_SOURCES}")
group_by_folder(${TURFS_ROOT_DIR} "${turfs_INCLUDES}")

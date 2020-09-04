# ------------------------------------------------------------
# This file is the main CMakeLists of memtool of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

project(memtool C CXX)
cmake_minimum_required(VERSION 3.10.2)

cmake_policy(SET CMP0057 NEW)
if (${CMAKE_VERSION} VERSION_EQUAL "3.12.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.12.0")
    cmake_policy(SET CMP0075 NEW)
endif (${CMAKE_VERSION} VERSION_EQUAL "3.12.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.12.0")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif ()
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

ENABLE_TESTING()


# ------------------------------------------------------------
# Project Version

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")


# ------------------------------------------------------------
# prime CMake includes

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(CheckLibraryExists)


# ------------------------------------------------------------
# profiling for coverage tests

option(PROFILING_MODE_ENABLED "enable profiling mode" off)
if (PROFILING_MODE_ENABLED)
    message(STATUS "Profiling mode enabled")
    set(CMAKE_BUILD_TYPE "Debug")
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(COMPILER_FLAGS_PROFILING "-fprofile-instr-generate -fcoverage-mapping")
    else ()
        set(COMPILER_FLAGS_PROFILING "-pg --coverage")
        set(LINKER_FLAGS_PROFILING "-lgcov")
        add_custom_target(run-gcovr
                COMMAND gcovr -r ${CMAKE_SOURCE_DIR} -x -o ${CMAKE_BINARY_DIR}/report.xml
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Running gcovr")
    endif ()
else ()
    message(STATUS "Profiling mode disabled")
endif ()


# ------------------------------------------------------------
# set global compiler flags

add_definitions(-DVERSION=\"${VERSION}\")
add_definitions(-D_GNU_SOURCE)
add_definitions(-D_DEFAULT_SOURCE)

# define __FILENAME__ as macro like __FILE__ but relativ to CMAKE_SOURCE_DIR
# note: $(subst ....) and $(abspath ....) are GNU Make commands (!) passed on 1:1 by CMake
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

# ---- fix C++ standard used ----

set(CMAKE_CXX_STANDARD 17)
if ((CMAKE_COMPILER_IS_GNUCC) OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
    set(COMPILER_WARNINGS "-Wall -Wextra -pedantic -pedantic-errors")
    set(COMPILER_FLAGS_DEBUG "-O0 -g -ggdb3 -fno-builtin -fPIC")
    set(COMPILER_FLAGS_RELEASE "-O3 -Werror -Werror -fno-builtin -fPIC")
    set(LINKER_FLAGS "-rdynamic")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Turning on compiler command export for CLang: ${CMAKE_BINARY_DIR}/compile_commands.json")
endif ()

# ---- callgrind integration ----

option(CALLGRIND_ENABLED "enable callgrind" off)
if (CALLGRIND_ENABLED)
    message(STATUS "callgrind enabled")
    message(WARNING "callgrind enabled: make package will create defective package!")
    set(VALGRIND_NEEDED "1")
    set(CALLGRIND_OPTIONS "")
else ()
    message(STATUS "callgrind disabled")
endif ()

# ---- final compiler settings ----

set(CMAKE_C_FLAGS_DEBUG "${COMPILER_WARNINGS} ${COMPILER_FLAGS_DEBUG} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMPILER_WARNINGS} ${COMPILER_FLAGS_DEBUG} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_C_FLAGS_RELEASE "${COMPILER_WARNINGS} ${COMPILER_FLAGS_RELEASE} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMPILER_WARNINGS} ${COMPILER_FLAGS_RELEASE} ${COMPILER_FLAGS_PROFILING}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${LINKER_FLAGS_PROFILING}")


# ------------------------------------------------------------
# googletest (internally handled as git submodule)

check_library_exists(pthread pthread_kill "/usr/local/lib;/usr/lib;/lib" HAVE_PTHREAD_LIB)
if (HAVE_PTHREAD_LIB)
    set(CMAKE_REQUIRED_LIBRARIES "pthread;${CMAKE_REQUIRED_LIBRARIES}")
endif ()

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/googletest/CMakeLists.txt")
    message(FATAL_ERROR "The git submodule for googletest has not been downloaded!")
endif ()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/3rd/googletest")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" "${CMAKE_SOURCE_DIR}/3rd/googletest"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/3rd/googletest"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/3rd/googletest"
)
set(GTEST_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/3rd/googletest/googletest/include"
    "${CMAKE_SOURCE_DIR}/3rd/googletest/googlemock/include"
)
set(GTEST_LIBRARY_PATH  "${CMAKE_BINARY_DIR}/3rd/googletest/lib")
set(GTEST_LIBRARY       "${GTEST_LIBRARY_PATH}/libgtest.a")
set(GTEST_MAIN_LIBRARY  "${GTEST_LIBRARY_PATH}/libgtest_main.a")
set(GMOCK_LIBRARY       "${GTEST_LIBRARY_PATH}/libgmock.a")
set(GMOCK_MAIN_LIBRARY  "${GTEST_LIBRARY_PATH}/libgmock_main.a")

find_package(GTest REQUIRED)


# ------------------------------------------------------------
# Doxygen

find_package(Doxygen)
if (DOXYGEN_FOUND STREQUAL "NO")
    message(FATAL_ERROR "Doxygen not found. Please get a copy http://www.doxygen.org")
endif ()

configure_file(${CMAKE_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)
add_custom_target(doxygen ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        COMMENT "Doxygen running ...")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES api-doc)

add_custom_target(doc)
add_dependencies(doc doxygen)


# ------------------------------------------------------------ 
# Workload

add_subdirectory(test)


# ---- CPack global definitions ----

set(CPACK_PACKAGE_NAME "hcs-memtool")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

set(CPACK_PACKAGE_DESCRIPTION "Small memory handling tool in C++.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This is a small tiny tool which enhances some memory handling stuff in C++.")
set(CPACK_PACKAGE_CONTACT "Oliver Maurhart <oliver.maurhart@headcode.space>")
set(CPACK_PACKAGE_VENDOR "headcode.space")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}")

set(CPACK_SOURCE_IGNORE_FILES "./build/*;/.git/;/.idea/;/.scannerwork/;/.sonar/;/sonar-project.properties")
set(CPACK_SOURCE_GENERATOR "TGZ")

if ("${CPACK_GENERATOR}" STREQUAL "DEB")
    include(cmake/deb-package.cmake)
endif()

if ("${CPACK_GENERATOR}" STREQUAL "RPM")
    include(cmake/rpm-package.cmake)
endif()

include(CPack)


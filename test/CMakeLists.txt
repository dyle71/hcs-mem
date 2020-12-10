# ------------------------------------------------------------
# This file is the main CMakeLists of mem of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

# activate Google Tests
set(GTEST_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rd/googletest")
include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)


# ------------------------------------------------------------ 
# Workload

set(TEST_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(benchmark)
add_subdirectory(unit)

# ------------------------------------------------------------
# This file is the main CMakeLists of memtool of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

# ------------------------------------------------------------ 
# Workload

include_directories(${CMAKE_SOURCE_DIR}/include ${GTEST_INCLUDE_DIR} ${CMAKE_BINARY_DIR})
link_directories("${GTEST_LIBRARY_PATH};${GMOCK_LIBRARY_PATH}")
set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES}")

set(BENCHMARK_TEST_SRC
    test_manipulator.cpp
)

add_executable(benchmark-tests ${BENCHMARK_TEST_SRC})
target_link_libraries(benchmark-tests gtest gtest_main gmock gmock_main ${CMAKE_REQUIRED_LIBRARIES})
gtest_add_tests(benchmark-tests "" AUTO)

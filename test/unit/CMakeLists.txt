# ------------------------------------------------------------
# This file is the main CMakeLists of memtool of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include ${GTEST_INCLUDE_DIR} ${CMAKE_BINARY_DIR})
link_directories("${GTEST_LIBRARY_PATH};${GMOCK_LIBRARY_PATH}")
set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES}")

set(UNIT_TEST_SRC
    #test_buffer.cpp
    #test_memory.cpp
    test_version.cpp
)

add_executable(unit-tests ${UNIT_TEST_SRC})
target_link_libraries(unit-tests gtest gtest_main gmock gmock_main ${CMAKE_REQUIRED_LIBRARIES})
gtest_add_tests(unit-tests "" AUTO)

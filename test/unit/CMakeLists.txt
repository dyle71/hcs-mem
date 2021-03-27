# ------------------------------------------------------------
# This file is the main CMakeLists of mem of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.  
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include ${TEST_BASE_DIR} ${CMAKE_BINARY_DIR})
set(UNIT_TEST_SRC
    test_manipulator.cpp
    test_memory.cpp
    test_version.cpp
)

add_executable(unit-tests ${UNIT_TEST_SRC})
target_link_libraries(unit-tests ${CONAN_LIBS_GTEST} ${CMAKE_REQUIRED_LIBRARIES})
gtest_add_tests(unit-tests "" AUTO)

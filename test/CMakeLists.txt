# ------------------------------------------------------------
# This file is the main CMakeLists of mem of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.  
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

include(GoogleTest)

set(TEST_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(benchmark)
add_subdirectory(unit)

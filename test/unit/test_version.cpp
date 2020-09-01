/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <cstdint>

#include <gtest/gtest.h>

#include <headcode/memtool/version.hpp>


TEST(Version, regular) {
    std::uint32_t version = MAKE_VERSION(0, 1, 0);
    EXPECT_EQ(headcode::memtool::GetCurrentVersion(), version);
    EXPECT_STREQ(headcode::memtool::GetVersionString().c_str(), VERSION);
}


TEST(Version, V0_1_0) {
    std::uint32_t version = MAKE_VERSION(0, 1, 0);
    EXPECT_EQ(GetHCSMemtoolVersion_0_1_0(), version);
}

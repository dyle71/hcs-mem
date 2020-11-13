/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <gtest/gtest.h>

#include <headcode/memtool/version.hpp>


TEST(Version, regular) {
    std::uint32_t version = MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    EXPECT_EQ(headcode::memtool::GetCurrentVersion(), version);
    EXPECT_STREQ(headcode::memtool::GetVersionString().c_str(), VERSION);
}


TEST(Version, V1_0_0) {
    std::uint32_t version = MAKE_VERSION(1, 0, 0);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (0) << 16 | (0)), version);
    EXPECT_EQ(GetHCSMemtoolVersion_1_0_0(), version);
}


TEST(Version, V1_0_1) {
    std::uint32_t version = MAKE_VERSION(1, 0, 1);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (0) << 16 | (1)), version);
    EXPECT_EQ(GetHCSMemtoolVersion_1_0_1(), version);
}


TEST(Version, V1_0_2) {
    std::uint32_t version = MAKE_VERSION(1, 0, 2);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (0) << 16 | (2)), version);
    EXPECT_EQ(GetHCSMemtoolVersion_1_0_2(), version);
}


TEST(Version, V1_0_3) {
    std::uint32_t version = MAKE_VERSION(1, 0, 3);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (0) << 16 | (3)), version);
    EXPECT_EQ(GetHCSMemtoolVersion_1_0_3(), version);
}

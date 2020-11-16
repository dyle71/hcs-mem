/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <gtest/gtest.h>

#include <headcode/mem/version.hpp>


TEST(Version, regular) {
    std::uint32_t version = MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    EXPECT_EQ(headcode::mem::GetCurrentVersion(), version);
    EXPECT_STREQ(headcode::mem::GetVersionString().c_str(), VERSION);
}


TEST(Version, V1_1_0) {
    std::uint32_t version = MAKE_VERSION(1, 1, 0);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (1) << 16 | (0)), version);
    EXPECT_EQ(GetHCSMemVersion_1_1_0(), version);
}

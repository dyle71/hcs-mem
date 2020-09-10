/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <chrono>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <headcode/memtool/memtool.hpp>

#include <shared/ipsum_lorem.hpp>

#include "benchmark_tool.hpp"


TEST(BenchmarkByteToHex, ByteToHex1000000) {
    
    auto time_start = std::chrono::high_resolution_clock::now();
    
    std::string hex;
    std::byte value{42};
    for (std::uint64_t i = 0; i < 1'000'000u; ++i) {
        hex = headcode::memtool::ByteToHex(value);
    }
    
    std::cout << "BenchmarkByteToHex::ByteToHex1000000 " << GetElapsed(time_start).count() << "us" << std::endl;
}

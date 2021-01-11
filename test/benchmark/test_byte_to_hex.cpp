/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <chrono>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <headcode/benchmark/benchmark.hpp>
#include <headcode/mem/mem.hpp>

#include <shared/ipsum_lorem.hpp>


TEST(BenchmarkByteToHex, ByteToHex1000000) {

    auto loop_count = 1'000'000u;
    std::string hex;
    std::byte value{42};

    auto time_start = std::chrono::high_resolution_clock::now();
    for (std::uint64_t i = 0; i < loop_count; ++i) {
        hex = headcode::mem::ByteToHex(value);
    }
    
    headcode::benchmark::Throughput throughput{headcode::benchmark::GetElapsedMicroSeconds(time_start), loop_count};
    std::cout << StreamPerformanceIndicators(throughput, "BenchmarkByteToHex::ByteToHex1000000 ");
}

/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include <gtest/gtest.h>

#include <headcode/memtool/memtool.hpp>

#include <shared/ipsum_lorem.hpp>

#include "benchmark_tool.hpp"


TEST(BenchmarkCanonical, IpsumLorem1000) {

    auto loop_count = 1000u;

    auto time_start = std::chrono::high_resolution_clock::now();
    auto mem = headcode::memtool::StringToMemory(IPSUM_LOREM_TEXT);
    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::memtool::MemoryToCanonicalString(mem, "ipsum-lorem: ");
    }

    Throughput throughput{GetElapsedMicroSeconds(time_start), IPSUM_LOREM_TEXT.size() * loop_count};
    std::cout << StreamPerformanceIndicators(throughput, "BenchmarkCanonical::IpsumLorem1000 ");
}

/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.  
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include <gtest/gtest.h>

#include <headcode/benchmark/benchmark.hpp>
#include <headcode/mem/mem.hpp>

#include <shared/ipsum_lorem.hpp>


TEST(BenchmarkCanonical, IpsumLorem1000) {

    auto loop_count = 1000u;

    auto time_start = std::chrono::high_resolution_clock::now();
    auto mem = headcode::mem::StringToMemory(IPSUM_LOREM_TEXT);
    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::mem::MemoryToCanonicalString(mem, "ipsum-lorem: ");
    }

    headcode::benchmark::Throughput throughput{headcode::benchmark::GetElapsedMicroSeconds(time_start),
                                               IPSUM_LOREM_TEXT.size() * loop_count};
    std::cout << StreamPerformanceIndicators(throughput, "BenchmarkCanonical::IpsumLorem1000 ");
}

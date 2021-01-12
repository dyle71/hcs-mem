/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.  
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <chrono>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <headcode/benchmark/benchmark.hpp>
#include <headcode/mem/mem.hpp>

#include <shared/ipsum_lorem.hpp>


TEST(BenchmarkManipulator, IpsumLorem1000) {

    auto loop_count = 1000u;
    std::vector<std::byte> data;

    auto time_start = std::chrono::high_resolution_clock::now();
    headcode::mem::MemoryManipulator manipulator{data};
    for (std::uint64_t i = 0; i < loop_count; ++i) {
        manipulator << IPSUM_LOREM_TEXT;
    }

    headcode::benchmark::Throughput throughput{headcode::benchmark::GetElapsedMicroSeconds(time_start),
                                               IPSUM_LOREM_TEXT.size() * loop_count};
    std::cout << StreamPerformanceIndicators(throughput, "BenchmarkManipulator::IpsumLorem1000 ");
}


TEST(BenchmarkManipulator, IpsumLorem1000PreReserve) {

    auto loop_count = 1000u;

    std::vector<std::byte> data;
    data.reserve(1000u * (IPSUM_LOREM_TEXT.size() + sizeof(std::uint64_t)));

    auto time_start = std::chrono::high_resolution_clock::now();

    headcode::mem::MemoryManipulator manipulator{data};
    for (std::uint64_t i = 0; i < loop_count; ++i) {
        manipulator << IPSUM_LOREM_TEXT;
    }
    headcode::benchmark::Throughput throughput{headcode::benchmark::GetElapsedMicroSeconds(time_start),
                                               IPSUM_LOREM_TEXT.size() * loop_count};
    std::cout << StreamPerformanceIndicators(throughput, "BenchmarkManipulator::IpsumLorem1000PreReserve ");
}

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


TEST(BenchmarkManipulator, IpsumLorem1000) {

    std::vector<std::byte> data;

    auto time_start = std::chrono::high_resolution_clock::now();

    headcode::memtool::MemoryManipulator manipulator{data};
    for (std::uint64_t i = 0; i < 1000u; ++i) {
        manipulator << IPSUM_LOREM_TEXT;
    }

    std::cout << "BenchmarkManipulator::IpsumLorem1000 " << GetElapsed(time_start).count() << "ms" << std::endl;
}


TEST(BenchmarkManipulator, IpsumLorem1000PreReserve) {

    std::vector<std::byte> data;
    data.reserve(1000u * (IPSUM_LOREM_TEXT.size() + sizeof(std::uint64_t)));

    auto time_start = std::chrono::high_resolution_clock::now();

    headcode::memtool::MemoryManipulator manipulator{data};
    for (std::uint64_t i = 0; i < 1000u; ++i) {
        manipulator << IPSUM_LOREM_TEXT;
    }

    std::cout << "BenchmarkManipulator::IpsumLorem1000PreReserve " << GetElapsed(time_start).count() << "ms"
              << std::endl;
}

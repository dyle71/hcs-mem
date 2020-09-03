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

#include "ipsum_lorem.hpp"

std::uint64_t const LOOP_COUNT = 1'000'000u;


TEST(BenchmarkManipulator, Regular) {
    
    std::vector<std::byte> data;

    auto time_start = std::chrono::high_resolution_clock::now();
    
    headcode::memtool::MemoryManipulator manipulator{data};
    for (std::uint64_t i = 0; i < LOOP_COUNT; ++i) {
        manipulator << ipsum_lorem_text;
    }
    
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    std::cout << "BenchmarkManipulator::Regular " << duration_ms << "ms" << std::endl;
}

TEST(BenchmarkManipulator, NoExtraGrowth) {
    
    std::vector<std::byte> data;
    
    auto time_start = std::chrono::high_resolution_clock::now();
    
    headcode::memtool::MemoryManipulator manipulator{data, 0.0f, 0};
    for (std::uint64_t i = 0; i < LOOP_COUNT; ++i) {
        manipulator << ipsum_lorem_text;
    }
    
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    std::cout << "BenchmarkManipulator::NoExtraGrowth " << duration_ms << "ms" << std::endl;
}


TEST(BenchmarkManipulator, MassiveGrowth) {
    
    std::vector<std::byte> data;
    
    auto time_start = std::chrono::high_resolution_clock::now();
    
    headcode::memtool::MemoryManipulator manipulator{data, 0.5f, 100000};
    for (std::uint64_t i = 0; i < LOOP_COUNT; ++i) {
        manipulator << ipsum_lorem_text;
    }
    
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    std::cout << "BenchmarkManipulator::NoExtraGrowth " << duration_ms << "ms" << std::endl;
}



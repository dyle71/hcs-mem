/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#ifndef HEADCODE_SPACE_MEMTOOL_TEST_BENCHMARK_BENCHMARK_TOOL_HPP
#define HEADCODE_SPACE_MEMTOOL_TEST_BENCHMARK_BENCHMARK_TOOL_HPP

#include <chrono>


/**
 * @brief   Returns the elapsed milliseconds since a point in time.
 * @param   since       the reference point in time
 * @return  microseconds passed since the reference point
 */
inline std::chrono::microseconds GetElapsed(std::chrono::high_resolution_clock::time_point const & since) {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - since);
}


#endif

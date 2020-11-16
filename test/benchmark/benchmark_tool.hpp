/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#ifndef HEADCODE_SPACE_MEM_TEST_BENCHMARK_BENCHMARK_TOOL_HPP
#define HEADCODE_SPACE_MEM_TEST_BENCHMARK_BENCHMARK_TOOL_HPP

#include <chrono>


/**
 * @brief Throughput data.
 */
struct Throughput {
    std::chrono::microseconds elapsed_;    /**< @brief Number of microseconds elapsed. */
    std::uint64_t bytes_;                  /**< @brief Number of bytes processed. */
};

/**
 * @brief   Returns the elapsed milliseconds since a point in time.
 * @param   since       the reference point in time
 * @return  microseconds passed since the reference point
 */
inline std::chrono::microseconds GetElapsedMicroSeconds(std::chrono::high_resolution_clock::time_point const & since) {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - since);
}

/**
 * @brief   Returns the bits per second
 * @param   throughput          the throughput data
 * @return  bps
 */
inline double GetBitsPerSecond(Throughput const & throughput) {
    float elapsed_us = throughput.elapsed_.count();
    if (elapsed_us != 0) {
        return static_cast<float>(throughput.bytes_ << 3) / (elapsed_us / 1'000'000.0);
    }
    return 0;
}

/**
 * @brief   Returns the kilo-bits per second
 * @param   throughput          the throughput data
 * @return  kbps
 */
inline double GetKiloBitsPerSecond(Throughput const & throughput) {
    return GetBitsPerSecond(throughput) / 1'000.0;
}

/**
 * @brief   Returns the Mega-bits per second
 * @param   throughput          the throughput data
 * @return  Mbps
 */
inline double GetMegaBitsPerSecond(Throughput const & throughput) {
    return GetBitsPerSecond(throughput) / 1'000'000.0;
}

/**
 * @brief   Returns the Giga-bits per second
 * @param   throughput          the throughput data
 * @return  Gbps
 */
inline double GetGigaBitsPerSecond(Throughput const & throughput) {
    return GetBitsPerSecond(throughput) / 1'000'000'000.0;
}

/**
 * @brief   Create a stream output of the throughput numbers.
 * @param   throughput      the throughput data
 * @param   indent          the indent in each line
 * @return  the stream object.
 */
inline std::string StreamPerformanceIndicators(Throughput const & throughput, std::string const & indent = {}) {
    std::stringstream ss;
    ss << indent << throughput.elapsed_.count() << " us" << std::endl;
    ss << indent << std::fixed << std::setprecision(3) << GetBitsPerSecond(throughput) << " bps" << std::endl;
    ss << indent << std::fixed << std::setprecision(3) << GetKiloBitsPerSecond(throughput) << " kbps" << std::endl;
    ss << indent << std::fixed << std::setprecision(3) << GetMegaBitsPerSecond(throughput) << " Mbps" << std::endl;
    ss << indent << std::fixed << std::setprecision(3) << GetGigaBitsPerSecond(throughput) << " Gbps" << std::endl;
    return ss.str();
}


#endif

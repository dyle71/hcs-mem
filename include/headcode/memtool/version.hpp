/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#ifndef HEADCODE_SPACE_MEMTOOL_VERSION_HPP
#define HEADCODE_SPACE_MEMTOOL_VERSION_HPP

#include <cstdint>
#include <string>


#ifndef MAKE_VERSION
#define MAKE_VERSION(x, y, z) ((x) << 24 | (y) << 16 | (z))
#endif


/**
 * @brief   Version check for 1.0.0
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSMemtoolVersion_1_0_0() {
    return MAKE_VERSION(1, 0, 0);
}

/**
 * @brief   Version check for 1.0.1
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSMemtoolVersion_1_0_1() {
    return MAKE_VERSION(1, 0, 1);
}

/**
 * @brief   Version check for 1.0.2
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSMemtoolVersion_1_0_2() {
    return MAKE_VERSION(1, 0, 2);
}

/**
 * @brief   Version check for 1.0.3
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSMemtoolVersion_1_0_3() {
    return MAKE_VERSION(1, 0, 3);
}

/**
 * @brief   The headcode memtool namespace
 */
namespace headcode::memtool {

/**
 * @brief   Returns the current version of the headcode-memtool.
 * @return  A value, representing the current version.
 */
inline std::uint32_t GetCurrentVersion() {
    return GetHCSMemtoolVersion_1_0_3();
}

/**
 * @brief   Returns a version as string
 * @return  a string holding the current version.
 */
inline std::string GetVersionString() {
    return "1.0.3";
}

}


#endif

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


namespace headcode::memtool {

/**
 * @brief   Returns the current version of the libait-memory.
 * @return  A value, representing the current version.
 */
std::uint32_t GetCurrentVersion();

/**
 * @brief   Returns a version as string
 * @return  a string holding the current version.
 */
std::string GetVersionString();

}

/**
 * @brief   Version check for 0.1.0
 * @return  A value, representing the version.
 */
std::uint32_t GetHCSMemtoolVersion_0_1_0();


#endif

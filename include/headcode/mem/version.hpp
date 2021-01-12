/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.  
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_MEM_VERSION_HPP
#define HEADCODE_SPACE_MEM_VERSION_HPP

#include <cstdint>
#include <string>


#ifndef MAKE_VERSION
#define MAKE_VERSION(x, y, z) ((x) << 24 | (y) << 16 | (z))
#endif


/**
 * @brief   Version check for 1.1.0
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSMemVersion_1_1_0() {
    return MAKE_VERSION(1, 1, 0);
}

/**
 * @brief   Version check for 1.1.0
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSMemVersion_1_1_1() {
    return MAKE_VERSION(1, 1, 1);
}

/**
 * @brief   The headcode mem namespace
 */
namespace headcode::mem {

/**
 * @brief   Returns the current version of the headcode-mem.
 * @return  A value, representing the current version.
 */
inline std::uint32_t GetCurrentVersion() {
    return GetHCSMemVersion_1_1_1();
}

/**
 * @brief   Returns a version as string
 * @return  a string holding the current version.
 */
inline std::string GetVersionString() {
    return "1.1.1";
}

}


#endif

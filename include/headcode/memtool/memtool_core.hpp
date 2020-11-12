/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#ifndef HEADCODE_SPACE_MEMTOOL_MEMTOOL_CORE_HPP
#define HEADCODE_SPACE_MEMTOOL_MEMTOOL_CORE_HPP

#include <cstddef>
#include <string>
#include <vector>


/**
 * @brief   The headcode memtool namespace
 */
namespace headcode::memtool {

/**
 * @brief   Convenient function to quickly convert a char array to a memory block.
 * @param   array       the char array
 * @param   size        size of the char array.
 * @return  A memory block based on this hex string.
 */
inline std::vector<std::byte> CharArrayToMemory(char const * array, std::uint64_t size);

/**
 * @brief   Gives a canonical representation of the memory.
 * The canonical representation is separated in different columns.
 *      - offset
 *      - first 8 bytes as hex " " (space) second 8 bytes as hex
 *      - ASCII representation (if printable)
 * @code
 *       00000000   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f   |........ ........|
 *       00000010   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f   |........ ........|
 *       00000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
 *       ...
 * @endcode
 * @param   array       the char array to show.
 * @param   size        size of the char array.
 * @param   indent      indent of each line
 * @return  a string containing the canonical representation of the memory.
 */
inline std::string CharArrayToCanonicalString(char const * array, std::uint64_t size, std::string const & indent = {});

/**
 * @brief   Converts a hex string to a memory.
 * All invalid characters in the given string will be set to 0,
 * e.g. 'feR2016X' will yield: 0xfe, 0x02, 0x01, 0x60.
 * The conversion will run on multiples of 2 characters.
 * @param   hex     the hex string describing a memory.
 * @return  A memory block based on this hex string.
 */
inline std::vector<std::byte> HexToMemory(std::string const & hex);

/**
 * @brief   Gives a canonical representation of the memory.
 * The canonical representation is separated in different columns.
 *      - offset
 *      - first 8 bytes as hex " " (space) second 8 bytes as hex
 *      - ASCII representation (if printable)
 * @code
 *       00000000   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f   |........ ........|
 *       00000010   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f   |........ ........|
 *       00000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
 *       ...
 * @endcode
 * @param   memory      the memory to show.
 * @param   indent      indent of each line
 * @return  a string containing the canonical representation of the memory.
 */
inline std::string MemoryToCanonicalString(std::vector<std::byte> const & memory, std::string const & indent = {});

/**
 * @brief   Converts a memory area to a hex-string.
 * @param   memory      the memory to convert.
 * @return  A hex string representing this memory.
 */
inline std::string MemoryToHex(std::vector<std::byte> const & memory);

/**
 * @brief   Convenient function to quickly convert a string to a memory block.
 * @param   str         the string to convert
 * @return  A memory block based on this hex string.
 */
inline std::vector<std::byte> StringToMemory(std::string const & str);

}


#include "memtool_core_bits.hpp"


#endif

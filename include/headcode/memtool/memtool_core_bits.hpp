/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#ifndef HEADCODE_SPACE_MEMTOOL_MEMTOOL_CORE_BITS_HPP
#define HEADCODE_SPACE_MEMTOOL_MEMTOOL_CORE_BITS_HPP

#ifndef HEADCODE_SPACE_MEMTOOL_MEMTOOL_CORE_HPP
#error "Do not include this file directly."
#endif

#include <cstddef>
#include <cstring>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


/**
 * @brief   The headcode memtool namespace
 */
namespace headcode::memtool {

/**
 * @brief   Converts a single byte to its hex representation.
 * @param   c       The char to convert.
 * @return  The hex value of this byte.
 */
inline std::string CharToHex(unsigned char c) {

    static bool initialized = false;
    static std::array<std::string, 256> table;
    if (!initialized) {

        auto CharToHexString = [](unsigned char c) -> std::string {
            std::string res{"00"};
            static char const * table = "0123456789abcdef";
            res[0] = table[c >> 4u];
            res[1] = table[c & 0x0fu];
            return res;
        };

        for (int i = 0; i < 256; ++i) {
            table[i] = CharToHexString(static_cast<unsigned char>(i));
        }

        initialized = true;
    }

    return table[c];
}

/**
 * @brief   Converts a single byte to its hex representation.
 * @param   b       The Byte.
 * @return  The hex value of this byte.
 */
inline std::string ByteToHex(std::byte b) {
    return CharToHex(static_cast<unsigned char>(b));
}

/**
 * @brief   Convert a hex string to a byte.
 * @param   sv      the string view
 * @return  the byte identified by the string.
 */
inline std::byte HexToByte(std::string_view const & sv) {

    static std::map<char, unsigned char> const hex_to_byte{
            {'0', 0x0}, {'1', 0x1}, {'2', 0x2}, {'3', 0x3}, {'4', 0x4}, {'5', 0x5}, {'6', 0x6}, {'7', 0x7},
            {'8', 0x8}, {'9', 0x9}, {'a', 0xa}, {'b', 0xb}, {'c', 0xc}, {'d', 0xd}, {'e', 0xe}, {'f', 0xf},
            {'A', 0xa}, {'B', 0xb}, {'C', 0xc}, {'D', 0xd}, {'F', 0xe}, {'F', 0xf}};

    if (sv.size() < 2) {
        return std::byte{0};
    }

    unsigned char value{0};

    auto iter_first = hex_to_byte.find(sv[0]);
    if (iter_first != hex_to_byte.end()) {
        value = iter_first->second << 4u;
    }

    auto iter_second = hex_to_byte.find(sv[1]);
    if (iter_second != hex_to_byte.end()) {
        value = value | iter_second->second;
    }

    return std::byte{value};
}

}

inline std::vector<std::byte> headcode::memtool::CharArrayToMemory(char const * array, std::uint64_t size) {

    std::vector<std::byte> res{size};
    if (array) {
        std::memcpy(res.data(), array, size);
    }
    return res;
}


inline std::string headcode::memtool::CharArrayToCanonicalString(char const * array, std::uint64_t size,
                                                                 std::string const & indent) {

    auto GetASCIIChar = [](char c) -> char { return (c >= ' ') && (c <= '~') ? c : '.'; };

    /*
        auto stream_memory = [&](std::vector<std::byte> const & memory, std::uint64_t i, std::stringstream & ss_hex,
                                 std::stringstream & ss_ascii) {
            if (i < memory.size()) {
                ss_hex << ByteToHex(memory[i]) << ' ';
                ss_ascii << GetASCIIChar(memory[i]);
            } else {
                ss_hex << "   ";
                ss_ascii << ' ';
            }
        };
    */

    // structure of a single line:
    // e.g.
    //   00000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
    //
    // from left to right

    static unsigned int const offset_size = 8;
    static std::string const gap_to_data{"   "};
    static unsigned int const word_size = 8 * 2 + 7;
    static std::string const word_gap{"  "};
    static std::string const gap_to_ascii{"   "};
    static std::string const ascii_gap{" "};
    static unsigned int const ascii_size = 8;
    static std::string const ascii_delimiter{"|"};
    static std::string const new_line{"\n"};

    // construct total length of single line
    unsigned int static_length_of_line = offset_size + gap_to_data.size() + word_size + word_gap.size() + word_size +
                                         gap_to_ascii.size() + ascii_delimiter.size() + ascii_size + ascii_gap.size() +
                                         ascii_size + ascii_delimiter.size() + new_line.size();

    auto length_of_full_line = indent.size() + static_length_of_line;


    auto lines = (size + 0x0ful) >> 4;
    std::string res;

    // resize to needed space + 1 for the snprintf (this always adds a 0 as final char).
    res.resize(lines * length_of_full_line + 1);


    // memcpy each line
    char * p = res.data();
    std::memset(p, ' ', res.size());
    for (std::uint64_t l = 0; l < lines; ++l) {

        // pos is the current position in the data array
        std::uint64_t pos = (l << 4);

        std::array<std::string, 16> data;
        std::array<char, 16> ascii;
        for (std::uint64_t i = 0; i < 16; ++i) {
            if (pos + i < size) {
                data[i] = CharToHex(array[pos + i]);
                ascii[i] = GetASCIIChar(array[pos + i]);
            } else {
                data[i] = "  ";
                ascii[i] = ' ';
            }
        }

        std::snprintf(p, length_of_full_line + 1,
                      "%s%08lx%s"
                      "%s %s %s %s %s %s %s %s%s%s %s %s %s %s %s %s %s%s"
                      "%s%c%c%c%c%c%c%c%c%s%c%c%c%c%c%c%c%c%s\n",
                      indent.data(), pos, gap_to_data.data(), data[0].data(), data[1].data(), data[2].data(),
                      data[3].data(), data[4].data(), data[5].data(), data[6].data(), data[7].data(), word_gap.data(),
                      data[8].data(), data[9].data(), data[10].data(), data[11].data(), data[12].data(),
                      data[13].data(), data[14].data(), data[15].data(), gap_to_ascii.data(), ascii_delimiter.data(),
                      ascii[0], ascii[1], ascii[2], ascii[3], ascii[4], ascii[5], ascii[6], ascii[7], ascii_gap.data(),
                      ascii[8], ascii[9], ascii[10], ascii[11], ascii[12], ascii[13], ascii[14], ascii[15],
                      ascii_delimiter.data());

        p += length_of_full_line;
    }

    // shrink to correct size (-1, since the last 0 has been added by snprintf)
    res.resize(lines * length_of_full_line);
    return res;
}


inline std::vector<std::byte> headcode::memtool::HexToMemory(std::string const & hex) {

    if (hex.empty()) {
        return {};
    }

    std::vector<std::byte> res{hex.size() / 2};
    for (size_t i = 0; i < hex.size() / 2; ++i) {
        res[i] = HexToByte(std::string_view{hex.data() + i * 2, 2});
    }

    return res;
}


inline std::string headcode::memtool::MemoryToCanonicalString(std::vector<std::byte> const & memory,
                                                              std::string const & indent) {

    return CharArrayToCanonicalString(reinterpret_cast<char const *>(memory.data()), memory.size(), indent);
}


inline std::string headcode::memtool::MemoryToHex(std::vector<std::byte> const & memory) {

    static std::map<std::byte, std::string> byte_to_hex;
    if (byte_to_hex.empty()) {
        for (int i = 0; i < 256; ++i) {
            auto b = std::byte{static_cast<unsigned char>(i)};
            byte_to_hex[b] = ByteToHex(b);
        }
    }

    std::stringstream ss;
    for (auto const & b : memory) {
        ss << byte_to_hex[b];
    }
    return ss.str();
}


inline std::vector<std::byte> headcode::memtool::StringToMemory(std::string const & str) {
    std::vector<std::byte> res{str.size()};
    std::memcpy(res.data(), str.data(), res.size());
    return res;
}


#endif

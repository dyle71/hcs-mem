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

namespace headcode::memtool {

/**
 * @brief   Converts a single byte to its hex representation.
 * @param   b       The Byte.
 * @return  The hex value of this byte.
 */
inline std::string ByteToHex(std::byte b) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << std::hex << std::noshowbase << std::to_integer<int>(b);
    return ss.str();
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
        value = iter_first->second << 4;
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

    auto ascii_char = [](std::byte b) -> char {
        auto c = std::to_integer<char>(b);
        return (c >= ' ') && (c <= '~') ? c : '.';
    };

    std::stringstream ss;

    for (std::uint64_t i = 0; i < memory.size(); i += 16) {

        if (i) {
            ss << "\n";
        }

        std::stringstream ss_hex;
        std::stringstream ss_ascii;

        // TODO: Refactor to simplify this

        // lower 8 bytes
        for (std::uint64_t j = i; j < i + 8; ++j) {
            if (j < memory.size()) {
                ss_hex << ByteToHex(memory[j]) << ' ';
                ss_ascii << ascii_char(memory[j]);
            } else {
                ss_hex << "   ";
                ss_ascii << ' ';
            }
        }

        ss_hex << ' ';
        ss_ascii << ' ';

        // upper 8 bytes
        for (std::uint64_t j = i + 8; j < i + 16; ++j) {

            if (j < memory.size()) {
                ss_hex << ByteToHex(memory[j]) << ' ';
                ss_ascii << ascii_char(memory[j]);
            } else {
                ss_hex << "   ";
                ss_ascii << ' ';
            }
        }

        ss << indent << std::setfill('0') << std::setw(8) << std::hex << std::noshowbase << i;
        ss << "   " << std::setw(-49) << ss_hex.str() << "  |" << std::setw(-17) << ss_ascii.str() << "|";
    }

    return ss.str();
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

/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_MEM_MEM_CORE_BITS_HPP
#define HEADCODE_SPACE_MEM_MEM_CORE_BITS_HPP

#ifndef HEADCODE_SPACE_MEM_MEM_CORE_HPP
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
 * @brief   The headcode mem namespace
 */
namespace headcode::mem {

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
            static char const * field = "0123456789abcdef";
            res[0] = field[c >> 4u];
            res[1] = field[c & 0x0fu];
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
        return static_cast<std::byte>(0);
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

    return static_cast<std::byte>(value);
}

/**
 * @brief   Write a single line of memory as ascii
 * Writes a memory line like:
 *      The quic k brown
 * @param   dst             dst to write at least 16 bytes + gap_size large
 * @param   src             the memory to write
 * @param   src_size        size of memory to write
 * @param   gap_size        size of the gap
 */
inline void DumpAsciiLine(char * dst, char const * src, std::uint64_t src_size, std::uint64_t gap_size) {

    std::uint64_t pos = 0;
    for (std::uint64_t i = 0; i < src_size; ++i) {

        bool printable = ((src[i] & 0x80) == 0x00) && (src[i] >= 0x20);
        dst[pos] = printable ? src[i] : '.';
        pos++;

        if (i == 7) {
            pos += gap_size;
        }
    }
}


/**
 * @brief   Write a single line of memory
 * Writes a memory line like:
 *      20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f
 * @param   dst             dst to write at least 46 bytes + gap_size large
 * @param   src             the memory to write
 * @param   src_size        size of memory to write
 * @param   gap_size        size of the gap
 */
inline void DumpHexLine(char * dst, char const * src, std::uint64_t src_size, std::uint64_t gap_size) {

    static char const * table = "0123456789abcdef";

    // places in the dst to write chars to (2 bytes each) including a space in between
    std::uint64_t pos[16] = {0x00, 0x03, 0x06, 0x09, 0x0c, 0x0f, 0x12, 0x15,
                             0x17, 0x1a, 0x1d, 0x20, 0x23, 0x26, 0x29, 0x2c};

    // add gap between the words
    if (src_size > 8) {
        for (std::uint64_t i = 8; i < 16; ++i) {
            pos[i] += gap_size;
        }
    }

    for (std::uint64_t i = 0; i < src_size; ++i) {
        dst[pos[i]] = table[(src[i] & 0xf0) >> 4];
        dst[pos[i] + 1] = table[src[i] & 0x0f];
    }
}

/**
 * @brief   Convert a number to a hex presentation.
 * @param   array       the char array to write must be a minimum of 16 bytes
 * @param   number      the number to write
 */
inline void HexNumberToCharArray(char * array, std::uint64_t number) {
    array[0xf] = static_cast<char>(number & 0x000000000000000ful) + '0';
    array[0xe] = static_cast<char>((number & 0x00000000000000f0ul) >> 4) + '0';
    array[0xd] = static_cast<char>((number & 0x0000000000000f00ul) >> 8) + '0';
    array[0xc] = static_cast<char>((number & 0x000000000000f000ul) >> 12) + '0';
    array[0xb] = static_cast<char>((number & 0x00000000000f0000ul) >> 16) + '0';
    array[0xa] = static_cast<char>((number & 0x0000000000f00000ul) >> 20) + '0';
    array[0x9] = static_cast<char>((number & 0x000000000f000000ul) >> 24) + '0';
    array[0x8] = static_cast<char>((number & 0x00000000f0000000ul) >> 28) + '0';
    array[0x7] = static_cast<char>((number & 0x0000000f00000000ul) >> 32) + '0';
    array[0x6] = static_cast<char>((number & 0x000000f000000000ul) >> 34) + '0';
    array[0x5] = static_cast<char>((number & 0x00000f0000000000ul) >> 38) + '0';
    array[0x4] = static_cast<char>((number & 0x0000f00000000000ul) >> 42) + '0';
    array[0x3] = static_cast<char>((number & 0x000f000000000000ul) >> 46) + '0';
    array[0x2] = static_cast<char>((number & 0x00f0000000000000ul) >> 50) + '0';
    array[0x1] = static_cast<char>((number & 0x0f00000000000000ul) >> 54) + '0';
    array[0x0] = static_cast<char>((number & 0xf000000000000000ul) >> 60) + '0';
}

}

inline std::vector<std::byte> headcode::mem::CharArrayToMemory(char const * array, std::uint64_t size) {

    std::vector<std::byte> res{size};
    if (array) {
        std::memcpy(res.data(), array, size);
    }
    return res;
}


inline std::string headcode::mem::CharArrayToCanonicalString(char const * array, std::uint64_t size,
                                                             std::string const & indent) {

    // structure of a single line:
    // e.g.
    //   0x0123456789012345   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !"#$%&' ()*+,-./|
    //
    // from left to right

    static std::uint64_t const offset_size = std::string{"0x"}.size() + 16;
    static std::string const gap_to_data{"   "};
    static unsigned int const word_size = 8 * 2 + 7;
    static std::string const word_gap{"  "};
    static std::string const gap_to_ascii{"   "};
    static std::string const ascii_gap{" "};
    static unsigned int const ascii_size = 8;
    static std::string const ascii_delimiter{"|"};
    static std::string const new_line{"\n"};

    // construct total length of single line
    std::uint64_t static_length_of_line = offset_size + gap_to_data.size() + word_size + word_gap.size() + word_size +
                                          gap_to_ascii.size() + ascii_delimiter.size() + ascii_size + ascii_gap.size() +
                                          ascii_size + ascii_delimiter.size() + new_line.size();

    auto length_of_full_line = indent.size() + static_length_of_line;

    std::string res;
    auto lines = (size + 0x0ful) >> 4;
    res.resize(lines * length_of_full_line);

    // prepare a template for each line
    std::string templ;
    templ.resize(length_of_full_line);
    auto templ_pointer = templ.data();
    std::memset(templ_pointer, ' ', length_of_full_line);

    if (!indent.empty()) {
        std::memcpy(templ_pointer, indent.c_str(), indent.size());
        templ_pointer += indent.size();
    }

    templ_pointer[0] = '0';
    templ_pointer[1] = 'x';
    templ_pointer += offset_size;

    std::memcpy(templ_pointer, gap_to_data.data(), gap_to_data.size());
    templ_pointer += gap_to_data.size();
    templ_pointer += word_size;
    std::memcpy(templ_pointer, word_gap.data(), word_gap.size());
    templ_pointer += word_gap.size();
    templ_pointer += word_size;
    std::memcpy(templ_pointer, gap_to_ascii.data(), gap_to_ascii.size());
    templ_pointer += gap_to_ascii.size();
    std::memcpy(templ_pointer, ascii_delimiter.data(), ascii_delimiter.size());
    templ_pointer += ascii_delimiter.size();
    templ_pointer += ascii_size;
    std::memcpy(templ_pointer, ascii_gap.data(), ascii_gap.size());
    templ_pointer += ascii_gap.size();
    templ_pointer += ascii_size;
    std::memcpy(templ_pointer, ascii_delimiter.data(), ascii_delimiter.size());
    templ_pointer += ascii_delimiter.size();
    *templ_pointer = '\n';

    for (std::uint64_t l = 0; l < lines; ++l) {

        auto dst = res.data() + length_of_full_line * l;
        std::memcpy(dst, templ.data(), length_of_full_line);

        auto to_offset = dst + indent.size();
        auto to_data = to_offset + offset_size + gap_to_data.size();
        auto to_ascii = to_data + word_size + word_gap.size() + word_size;
        to_ascii += gap_to_ascii.size() + ascii_delimiter.size();

        std::uint64_t pos = (l << 4);
        HexNumberToCharArray(to_offset + 2, pos);
        DumpHexLine(to_data, array + pos, std::min<std::uint64_t>(16, size - pos), word_gap.size());
        DumpAsciiLine(to_ascii, array + pos, std::min<std::uint64_t>(16, size - pos), ascii_gap.size());
    }

    return res;
}


inline std::vector<std::byte> headcode::mem::HexToMemory(std::string const & hex) {

    if (hex.empty()) {
        return {};
    }

    std::vector<std::byte> res{hex.size() / 2};
    for (size_t i = 0; i < hex.size() / 2; ++i) {
        res[i] = HexToByte(std::string_view{hex.data() + i * 2, 2});
    }

    return res;
}


inline std::string headcode::mem::MemoryToCanonicalString(std::vector<std::byte> const & memory,
                                                          std::string const & indent) {

    return CharArrayToCanonicalString(reinterpret_cast<char const *>(memory.data()), memory.size(), indent);
}


inline std::string headcode::mem::MemoryToHex(std::vector<std::byte> const & memory) {
    return MemoryToHex(reinterpret_cast<char const *>(memory.data()), memory.size());
}


inline std::string headcode::mem::MemoryToHex(char const * memory, std::uint64_t size) {

    static char char_to_hex[256][2] = {
            {'0', '0'}, {'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '4'}, {'0', '5'}, {'0', '6'}, {'0', '7'}, {'0', '8'},
            {'0', '9'}, {'0', 'a'}, {'0', 'b'}, {'0', 'c'}, {'0', 'd'}, {'0', 'e'}, {'0', 'f'}, {'1', '0'}, {'1', '1'},
            {'1', '2'}, {'1', '3'}, {'1', '4'}, {'1', '5'}, {'1', '6'}, {'1', '7'}, {'1', '8'}, {'1', '9'}, {'1', 'a'},
            {'1', 'b'}, {'1', 'c'}, {'1', 'd'}, {'1', 'e'}, {'1', 'f'}, {'2', '0'}, {'2', '1'}, {'2', '2'}, {'2', '3'},
            {'2', '4'}, {'2', '5'}, {'2', '6'}, {'2', '7'}, {'2', '8'}, {'2', '9'}, {'2', 'a'}, {'2', 'b'}, {'2', 'c'},
            {'2', 'd'}, {'2', 'e'}, {'2', 'f'}, {'3', '0'}, {'3', '1'}, {'3', '2'}, {'3', '3'}, {'3', '4'}, {'3', '5'},
            {'3', '6'}, {'3', '7'}, {'3', '8'}, {'3', '9'}, {'3', 'a'}, {'3', 'b'}, {'3', 'c'}, {'3', 'd'}, {'3', 'e'},
            {'3', 'f'}, {'4', '0'}, {'4', '1'}, {'4', '2'}, {'4', '3'}, {'4', '4'}, {'4', '5'}, {'4', '6'}, {'4', '7'},
            {'4', '8'}, {'4', '9'}, {'4', 'a'}, {'4', 'b'}, {'4', 'c'}, {'4', 'd'}, {'4', 'e'}, {'4', 'f'}, {'5', '0'},
            {'5', '1'}, {'5', '2'}, {'5', '3'}, {'5', '4'}, {'5', '5'}, {'5', '6'}, {'5', '7'}, {'5', '8'}, {'5', '9'},
            {'5', 'a'}, {'5', 'b'}, {'5', 'c'}, {'5', 'd'}, {'5', 'e'}, {'5', 'f'}, {'6', '0'}, {'6', '1'}, {'6', '2'},
            {'6', '3'}, {'6', '4'}, {'6', '5'}, {'6', '6'}, {'6', '7'}, {'6', '8'}, {'6', '9'}, {'6', 'a'}, {'6', 'b'},
            {'6', 'c'}, {'6', 'd'}, {'6', 'e'}, {'6', 'f'}, {'7', '0'}, {'7', '1'}, {'7', '2'}, {'7', '3'}, {'7', '4'},
            {'7', '5'}, {'7', '6'}, {'7', '7'}, {'7', '8'}, {'7', '9'}, {'7', 'a'}, {'7', 'b'}, {'7', 'c'}, {'7', 'd'},
            {'7', 'e'}, {'7', 'f'}, {'8', '0'}, {'8', '1'}, {'8', '2'}, {'8', '3'}, {'8', '4'}, {'8', '5'}, {'8', '6'},
            {'8', '7'}, {'8', '8'}, {'8', '9'}, {'8', 'a'}, {'8', 'b'}, {'8', 'c'}, {'8', 'd'}, {'8', 'e'}, {'8', 'f'},
            {'9', '0'}, {'9', '1'}, {'9', '2'}, {'9', '3'}, {'9', '4'}, {'9', '5'}, {'9', '6'}, {'9', '7'}, {'9', '8'},
            {'9', '9'}, {'9', 'a'}, {'9', 'b'}, {'9', 'c'}, {'9', 'd'}, {'9', 'e'}, {'9', 'f'}, {'a', '0'}, {'a', '1'},
            {'a', '2'}, {'a', '3'}, {'a', '4'}, {'a', '5'}, {'a', '6'}, {'a', '7'}, {'a', '8'}, {'a', '9'}, {'a', 'a'},
            {'a', 'b'}, {'a', 'c'}, {'a', 'd'}, {'a', 'e'}, {'a', 'f'}, {'b', '0'}, {'b', '1'}, {'b', '2'}, {'b', '3'},
            {'b', '4'}, {'b', '5'}, {'b', '6'}, {'b', '7'}, {'b', '8'}, {'b', '9'}, {'b', 'a'}, {'b', 'b'}, {'b', 'c'},
            {'b', 'd'}, {'b', 'e'}, {'b', 'f'}, {'c', '0'}, {'c', '1'}, {'c', '2'}, {'c', '3'}, {'c', '4'}, {'c', '5'},
            {'c', '6'}, {'c', '7'}, {'c', '8'}, {'c', '9'}, {'c', 'a'}, {'c', 'b'}, {'c', 'c'}, {'c', 'd'}, {'c', 'e'},
            {'c', 'f'}, {'d', '0'}, {'d', '1'}, {'d', '2'}, {'d', '3'}, {'d', '4'}, {'d', '5'}, {'d', '6'}, {'d', '7'},
            {'d', '8'}, {'d', '9'}, {'d', 'a'}, {'d', 'b'}, {'d', 'c'}, {'d', 'd'}, {'d', 'e'}, {'d', 'f'}, {'e', '0'},
            {'e', '1'}, {'e', '2'}, {'e', '3'}, {'e', '4'}, {'e', '5'}, {'e', '6'}, {'e', '7'}, {'e', '8'}, {'e', '9'},
            {'e', 'a'}, {'e', 'b'}, {'e', 'c'}, {'e', 'd'}, {'e', 'e'}, {'e', 'f'}, {'f', '0'}, {'f', '1'}, {'f', '2'},
            {'f', '3'}, {'f', '4'}, {'f', '5'}, {'f', '6'}, {'f', '7'}, {'f', '8'}, {'f', '9'}, {'f', 'a'}, {'f', 'b'},
            {'f', 'c'}, {'f', 'd'}, {'f', 'e'}, {'f', 'f'}};

    std::string res;
    res.resize(size * 2);
    for (std::uint64_t i = 0; i < size; ++i) {
        int value = static_cast<unsigned char>(memory[i]);
        res[i * 2] = char_to_hex[value][0];
        res[i * 2 + 1] = char_to_hex[value][1];
    }

    return res;
}


inline std::vector<std::byte> headcode::mem::StringToMemory(std::string const & str) {
    std::vector<std::byte> res{str.size()};
    std::memcpy(res.data(), str.data(), res.size());
    return res;
}


#endif

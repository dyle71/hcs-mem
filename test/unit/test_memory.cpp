/*
 * This file is part of the headcode.space mem.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <gtest/gtest.h>

#include <headcode/mem/mem.hpp>

using namespace headcode::mem;


TEST(Memory, MemoryToHex) {

    std::vector<std::byte> memory;
    std::string hex;

    memory.resize(16);
    memory[0] = std::byte{0};
    memory[1] = std::byte{1};
    memory[2] = std::byte{2};
    memory[3] = std::byte{3};
    memory[4] = std::byte{4};
    memory[5] = std::byte{5};
    memory[6] = std::byte{6};
    memory[7] = std::byte{7};
    memory[8] = std::byte{8};
    memory[9] = std::byte{9};
    memory[10] = std::byte{10};
    memory[11] = std::byte{11};
    memory[12] = std::byte{12};
    memory[13] = std::byte{13};
    memory[14] = std::byte{14};
    memory[15] = std::byte{15};

    hex = MemoryToHex(memory);
    EXPECT_STREQ(hex.c_str(), "000102030405060708090a0b0c0d0e0f");

    memory[0] = std::byte{0x11};
    memory[1] = std::byte{0x6f};
    memory[2] = std::byte{0x7f};
    memory[3] = std::byte{0x44};
    memory[4] = std::byte{0xbd};
    memory[5] = std::byte{0xdc};
    memory[6] = std::byte{0x10};
    memory[7] = std::byte{0x81};
    memory[8] = std::byte{0x29};
    memory[9] = std::byte{0xe6};
    memory[10] = std::byte{0xf9};
    memory[11] = std::byte{0x5e};
    memory[12] = std::byte{0x7e};
    memory[13] = std::byte{0x54};
    memory[14] = std::byte{0x20};
    memory[15] = std::byte{0xda};

    hex = MemoryToHex(memory);
    EXPECT_STREQ(hex.c_str(), "116f7f44bddc108129e6f95e7e5420da");
}


TEST(Memory, HexToMemory) {

    std::vector<std::byte> memory;
    std::string hex;

    memory.resize(16);
    memory[0] = std::byte{0};
    memory[1] = std::byte{1};
    memory[2] = std::byte{2};
    memory[3] = std::byte{3};
    memory[4] = std::byte{4};
    memory[5] = std::byte{5};
    memory[6] = std::byte{6};
    memory[7] = std::byte{7};
    memory[8] = std::byte{8};
    memory[9] = std::byte{9};
    memory[10] = std::byte{10};
    memory[11] = std::byte{11};
    memory[12] = std::byte{12};
    memory[13] = std::byte{13};
    memory[14] = std::byte{14};
    memory[15] = std::byte{15};

    hex = "000102030405060708090a0b0c0d0e0f";
    EXPECT_EQ(memory, HexToMemory(hex));

    memory[0] = std::byte{0x11};
    memory[1] = std::byte{0x6f};
    memory[2] = std::byte{0x7f};
    memory[3] = std::byte{0x44};
    memory[4] = std::byte{0xbd};
    memory[5] = std::byte{0xdc};
    memory[6] = std::byte{0x10};
    memory[7] = std::byte{0x81};
    memory[8] = std::byte{0x29};
    memory[9] = std::byte{0xe6};
    memory[10] = std::byte{0xf9};
    memory[11] = std::byte{0x5e};
    memory[12] = std::byte{0x7e};
    memory[13] = std::byte{0x54};
    memory[14] = std::byte{0x20};
    memory[15] = std::byte{0xda};

    hex = "116f7f44bddc108129e6f95e7e5420da";
    EXPECT_EQ(memory, HexToMemory(hex));
}


TEST(Memory, HexToMemorySmall) {
    std::string hex{"f"};
    EXPECT_EQ(0, static_cast<unsigned char>(HexToByte(hex)));
}


TEST(Memory, HexToMemToHex) {
    std::string hex{"116f7f44bddc108129e6f95e7e5420da"};
    EXPECT_STREQ(hex.c_str(), MemoryToHex(HexToMemory(hex)).c_str());
}


TEST(Memory, HexToMemory_illegal) {

    std::vector<std::byte> memory;
    std::string hex;

    hex = "116ffoo";
    memory.resize(3);
    memory[0] = std::byte{0x11};
    memory[1] = std::byte{0x6f};
    memory[2] = std::byte{0xf0};
    EXPECT_EQ(memory, HexToMemory(hex));

    hex = "BAr116fx";
    memory.resize(4);
    memory[0] = std::byte{0xba};
    memory[1] = std::byte{0x01};
    memory[2] = std::byte{0x16};
    memory[3] = std::byte{0xf0};
    EXPECT_EQ(memory, HexToMemory(hex));

    hex.clear();
    memory.clear();
    EXPECT_EQ(memory, HexToMemory(hex));
}


TEST(Memory, CharArrayToMemory) {

    auto text = "The quick brown fox jumps over the lazy dog";
    auto memory = CharArrayToMemory(text, strlen(text));

    auto canonical = MemoryToCanonicalString(memory);
    auto expected =
            "\
0x0000000000000000   54 68 65 20 71 75 69 63  6b 20 62 72 6f 77 6e 20   |The quic k brown |\n\
0x0000000000000010   66 6f 78 20 6a 75 6d 70  73 20 6f 76 65 72 20 74   |fox jump s over t|\n\
0x0000000000000020   68 65 20 6c 61 7a 79 20  64 6f 67                  |he lazy  dog     |\n";

    EXPECT_STREQ(canonical.c_str(), expected);
}


TEST(Memory, StringToMemory) {

    std::string text{"The quick brown fox jumps over the lazy dog"};
    auto memory = StringToMemory(text);

    auto canonical = MemoryToCanonicalString(memory);
    auto expected =
            "\
0x0000000000000000   54 68 65 20 71 75 69 63  6b 20 62 72 6f 77 6e 20   |The quic k brown |\n\
0x0000000000000010   66 6f 78 20 6a 75 6d 70  73 20 6f 76 65 72 20 74   |fox jump s over t|\n\
0x0000000000000020   68 65 20 6c 61 7a 79 20  64 6f 67                  |he lazy  dog     |\n";

    EXPECT_STREQ(canonical.c_str(), expected);
}

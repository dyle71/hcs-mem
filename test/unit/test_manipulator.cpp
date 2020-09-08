/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <gtest/gtest.h>

#include <headcode/memtool/memtool.hpp>

#include <shared/ipsum_lorem.hpp>

using namespace headcode::memtool;


TEST(TestManipulator, RegularStreamInto) {

    std::vector<std::byte> data{10};

    headcode::memtool::MemoryManipulator manipulator{data};
    for (std::uint64_t i = 0; i < data.size(); ++i) {
        manipulator << static_cast<std::byte>(i);
    }
    for (std::uint64_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(data[i], static_cast<std::byte>(i));
    }
}


TEST(TestManipulator, ShallowCopy) {

    std::vector<std::byte> data{10};
    headcode::memtool::MemoryManipulator manipulator{data};
    headcode::memtool::MemoryManipulator buffer_copy{manipulator};
    EXPECT_EQ(buffer_copy.GetMemory().data(), manipulator.GetMemory().data());
}


TEST(TestManipulator, WriteReadPOD) {

    std::vector<std::byte> data{10};

    char src_c = -98;
    unsigned char src_uc = 211;
    std::int16_t src_s = -9887;
    std::uint16_t src_us = 1337;
    std::int32_t src_i = -888912;
    std::uint32_t src_ui = 19883563;
    std::int64_t src_l = -913918938;
    std::uint64_t src_ul = 132176734452;
    float src_f = 3.1415926535897932384626433832f;
    double src_d = 2.718281828459045235360287471352662497757247093699959;
    std::string src_str{"The quick brown fox jumped over the lazy dog."};

    headcode::memtool::MemoryManipulator manipulator{data};

    manipulator.Write(src_c);
    manipulator.Write(src_uc);
    manipulator.Write(src_s);
    manipulator.Write(src_us);
    manipulator.Write(src_i);
    manipulator.Write(src_ui);
    manipulator.Write(src_l);
    manipulator.Write(src_ul);
    manipulator.Write(src_f);
    manipulator.Write(src_d);
    manipulator.Write(src_str);

    char dst_c{0};
    unsigned char dst_uc{0};
    std::int16_t dst_s{0};
    std::uint16_t dst_us{0};
    std::int32_t dst_i{0};
    std::uint32_t dst_ui{0};
    std::int64_t dst_l{0};
    std::uint64_t dst_ul{0};
    float dst_f{0.0f};
    double dst_d{0.0};
    std::string dst_str;

    manipulator.Reset();

    manipulator.Read(dst_c);
    manipulator.Read(dst_uc);
    manipulator.Read(dst_s);
    manipulator.Read(dst_us);
    manipulator.Read(dst_i);
    manipulator.Read(dst_ui);
    manipulator.Read(dst_l);
    manipulator.Read(dst_ul);
    manipulator.Read(dst_f);
    manipulator.Read(dst_d);
    manipulator.Read(dst_str);

    EXPECT_EQ(dst_c, src_c);
    EXPECT_EQ(dst_uc, src_uc);
    EXPECT_EQ(dst_s, src_s);
    EXPECT_EQ(dst_us, src_us);
    EXPECT_EQ(dst_i, src_i);
    EXPECT_EQ(dst_ui, src_ui);
    EXPECT_EQ(dst_l, src_l);
    EXPECT_EQ(dst_ul, src_ul);
    EXPECT_EQ(dst_f, src_f);
    EXPECT_EQ(dst_d, src_d);
    EXPECT_STREQ(dst_str.c_str(), src_str.c_str());
}


TEST(TestManipulator, StreamPOD) {

    std::vector<std::byte> data{10};

    char src_c = -98;
    unsigned char src_uc = 211;
    std::int16_t src_s = -9887;
    std::uint16_t src_us = 1337;
    std::int32_t src_i = -888912;
    std::uint32_t src_ui = 19883563;
    std::int64_t src_l = -913918938;
    std::uint64_t src_ul = 132176734452;
    float src_f = 3.1415926535897932384626433832f;
    double src_d = 2.718281828459045235360287471352662497757247093699959;
    std::string src_str{"The quick brown fox jumped over the lazy dog."};

    headcode::memtool::MemoryManipulator manipulator{data};

    manipulator << src_c;
    manipulator << src_uc;
    manipulator << src_s;
    manipulator << src_us;
    manipulator << src_i;
    manipulator << src_ui;
    manipulator << src_l;
    manipulator << src_ul;
    manipulator << src_f;
    manipulator << src_d;
    manipulator << src_str;

    char dst_c{0};
    unsigned char dst_uc{0};
    std::int16_t dst_s{0};
    std::uint16_t dst_us{0};
    std::int32_t dst_i{0};
    std::uint32_t dst_ui{0};
    std::int64_t dst_l{0};
    std::uint64_t dst_ul{0};
    float dst_f{0.0f};
    double dst_d{0.0};
    std::string dst_str;

    manipulator.Reset();

    manipulator >> dst_c;
    manipulator >> dst_uc;
    manipulator >> dst_s;
    manipulator >> dst_us;
    manipulator >> dst_i;
    manipulator >> dst_ui;
    manipulator >> dst_l;
    manipulator >> dst_ul;
    manipulator >> dst_f;
    manipulator >> dst_d;
    manipulator >> dst_str;

    EXPECT_EQ(dst_c, src_c);
    EXPECT_EQ(dst_uc, src_uc);
    EXPECT_EQ(dst_s, src_s);
    EXPECT_EQ(dst_us, src_us);
    EXPECT_EQ(dst_i, src_i);
    EXPECT_EQ(dst_ui, src_ui);
    EXPECT_EQ(dst_l, src_l);
    EXPECT_EQ(dst_ul, src_ul);
    EXPECT_EQ(dst_f, src_f);
    EXPECT_EQ(dst_d, src_d);
    EXPECT_STREQ(dst_str.c_str(), src_str.c_str());
}


TEST(TestManipulator, WriteReadMemory) {

    auto ipsum_lorem_src = headcode::memtool::StringToMemory(IPSUM_LOREM_TEXT);

    std::vector<std::byte> data;
    headcode::memtool::MemoryManipulator manipulator{data};
    manipulator.Write(ipsum_lorem_src);

    headcode::memtool::MemoryManipulator manipulator_copy{manipulator};
    std::vector<std::byte> ipsum_lorem_dst;
    manipulator_copy.Read(ipsum_lorem_dst);

    EXPECT_EQ(ipsum_lorem_src, ipsum_lorem_dst);
    EXPECT_NE(ipsum_lorem_src.data(), ipsum_lorem_dst.data());
}


TEST(TestManipulator, StreamMemory) {

    auto ipsum_lorem_src = headcode::memtool::StringToMemory(IPSUM_LOREM_TEXT);

    std::vector<std::byte> data;
    headcode::memtool::MemoryManipulator manipulator{data};
    manipulator << ipsum_lorem_src;

    manipulator.Reset();
    std::vector<std::byte> ipsum_lorem_dst;
    manipulator >> ipsum_lorem_dst;

    EXPECT_EQ(ipsum_lorem_src, ipsum_lorem_dst);
    EXPECT_NE(ipsum_lorem_src.data(), ipsum_lorem_dst.data());
}


TEST(TestManipulator, WriteReadList) {

    std::list<uint64_t> list_src{7, 1337, 0xdead, 9990999};
    std::list<uint64_t> list_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.Write(list_src);
    manipulator.Reset();
    manipulator.Read(list_dst);

    auto iter = list_dst.begin();
    EXPECT_EQ(*iter++, 7u);
    EXPECT_EQ(*iter++, 1337u);
    EXPECT_EQ(*iter++, 0xdeadu);
    EXPECT_EQ(*iter++, 9990999u);
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(std::uint64_t) * list_src.size());
}


TEST(TestManipulator, StreamList) {

    std::list<uint64_t> list_src{7, 1337, 0xdead, 9990999};
    std::list<uint64_t> list_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator << list_src;
    manipulator.Reset();
    manipulator >> list_dst;

    auto iter = list_dst.begin();
    EXPECT_EQ(*iter++, 7u);
    EXPECT_EQ(*iter++, 1337u);
    EXPECT_EQ(*iter++, 0xdeadu);
    EXPECT_EQ(*iter++, 9990999u);
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(std::uint64_t) * list_src.size());
}


TEST(TestManipulator, WriteReadMap) {

    std::map<std::string, std::string> map_src{{"one", "eins"}, {"two", "zwei"}, {"three", "drei"}, {"four", "vier"}};
    std::map<std::string, std::string> map_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.Write(map_src);
    manipulator.Reset();
    manipulator.Read(map_dst);

    EXPECT_EQ(map_src.size(), map_dst.size());
    EXPECT_STREQ(map_src.at("one").c_str(), map_dst.at("one").c_str());
    EXPECT_STREQ(map_src.at("two").c_str(), map_dst.at("two").c_str());
    EXPECT_STREQ(map_src.at("three").c_str(), map_dst.at("three").c_str());
    EXPECT_STREQ(map_src.at("four").c_str(), map_dst.at("four").c_str());
}


TEST(TestManipulator, StreamMap) {

    std::map<std::string, std::string> map_src{{"one", "eins"}, {"two", "zwei"}, {"three", "drei"}, {"four", "vier"}};
    std::map<std::string, std::string> map_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator << map_src;
    manipulator.Reset();
    manipulator >> map_dst;

    EXPECT_EQ(map_src.size(), map_dst.size());
    EXPECT_STREQ(map_src.at("one").c_str(), map_dst.at("one").c_str());
    EXPECT_STREQ(map_src.at("two").c_str(), map_dst.at("two").c_str());
    EXPECT_STREQ(map_src.at("three").c_str(), map_dst.at("three").c_str());
    EXPECT_STREQ(map_src.at("four").c_str(), map_dst.at("four").c_str());
}


TEST(TestManipulator, WriteReadSet) {

    std::set<char> set_src{'d', 'y', 'l', 'e', '7', '1'};
    std::set<char> set_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.Write(set_src);
    manipulator.Reset();
    manipulator.Read(set_dst);

    EXPECT_NE(set_dst.find('d'), set_dst.end());
    EXPECT_NE(set_dst.find('y'), set_dst.end());
    EXPECT_NE(set_dst.find('l'), set_dst.end());
    EXPECT_NE(set_dst.find('e'), set_dst.end());
    EXPECT_NE(set_dst.find('7'), set_dst.end());
    EXPECT_NE(set_dst.find('1'), set_dst.end());
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + set_src.size() * sizeof(char));
}


TEST(TestManipulator, StreamSet) {

    std::set<char> set_src{'d', 'y', 'l', 'e', '7', '1'};
    std::set<char> set_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator << set_src;
    manipulator.Reset();
    manipulator >> set_dst;

    EXPECT_NE(set_dst.find('d'), set_dst.end());
    EXPECT_NE(set_dst.find('y'), set_dst.end());
    EXPECT_NE(set_dst.find('l'), set_dst.end());
    EXPECT_NE(set_dst.find('e'), set_dst.end());
    EXPECT_NE(set_dst.find('7'), set_dst.end());
    EXPECT_NE(set_dst.find('1'), set_dst.end());
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + set_src.size() * sizeof(char));
}


TEST(TestManipulator, WriteReadVector) {

    std::vector<std::string> vector_src{"apple", "banana", "lemon", "orange", "pineapple", "sopophorous bean"};
    std::vector<std::string> vector_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.Write(vector_src);
    manipulator.Reset();
    manipulator.Read(vector_dst);

    auto iter = vector_dst.begin();
    EXPECT_EQ(*iter++, "apple");
    EXPECT_EQ(*iter++, "banana");
    EXPECT_EQ(*iter++, "lemon");
    EXPECT_EQ(*iter++, "orange");
    EXPECT_EQ(*iter++, "pineapple");
    EXPECT_EQ(*iter++, "sopophorous bean");

    auto item_size_sum = 0ul;
    std::for_each(vector_src.begin(), vector_src.end(), [&](auto const & s) { item_size_sum += s.size(); });
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(std::uint64_t) * vector_src.size() + item_size_sum);
}


TEST(TestManipulator, StreamVector) {

    std::vector<std::string> vector_src{"apple", "banana", "lemon", "orange", "pineapple", "sopophorous bean"};
    std::vector<std::string> vector_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator << vector_src;
    manipulator.Reset();
    manipulator >> vector_dst;

    auto iter = vector_dst.begin();
    EXPECT_EQ(*iter++, "apple");
    EXPECT_EQ(*iter++, "banana");
    EXPECT_EQ(*iter++, "lemon");
    EXPECT_EQ(*iter++, "orange");
    EXPECT_EQ(*iter++, "pineapple");
    EXPECT_EQ(*iter++, "sopophorous bean");

    auto item_size_sum = 0ul;
    std::for_each(vector_src.begin(), vector_src.end(), [&](auto const & s) { item_size_sum += s.size(); });
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(std::uint64_t) * vector_src.size() + item_size_sum);
}


TEST(TestManipulator, WriteReadStringAndFloat) {

    std::vector<std::byte> memory{128};
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.SetPosition(0);
    manipulator.Write(std::string{"The brown fox jumped over the lazy dog."});
    manipulator.Write(3.1514f);

    std::string s;
    float f;

    manipulator.SetPosition(0);
    EXPECT_STREQ(manipulator.Read(s).c_str(), "The brown fox jumped over the lazy dog.");
    EXPECT_EQ(manipulator.Read(f), 3.1514f);
}


TEST(TestManipulator, StreamStringAndFloatAndRead) {

    std::vector<std::byte> memory{128};
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.SetPosition(0);
    manipulator << std::string{"The brown fox jumped over the lazy dog."};
    manipulator << 3.1514f;

    std::string s;
    float f;

    manipulator.SetPosition(0);
    EXPECT_STREQ(manipulator.Read(s).c_str(), "The brown fox jumped over the lazy dog.");
    EXPECT_EQ(manipulator.Read(f), 3.1514f);
}


TEST(TestManipulator, WriteReadComplexStructure) {

    std::map<std::string, std::list<int>> complex_src{
            {"abc", {1, 2, 3}}, {"def", {-1, -2, -3}}, {"odd", {1, 3, 5, 7, 9, 11}}, {"even", {2, 4, 6, 8, 10, 12}}};
    std::map<std::string, std::list<int>> complex_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator.Write(complex_src);
    manipulator.Reset();
    manipulator.Read(complex_dst);

    EXPECT_EQ(complex_src.size(), complex_dst.size());
    EXPECT_EQ(complex_src.at("abc"), complex_dst.at("abc"));
    EXPECT_EQ(complex_src.at("def"), complex_dst.at("def"));
    EXPECT_EQ(complex_src.at("odd"), complex_dst.at("odd"));
    EXPECT_EQ(complex_src.at("even"), complex_dst.at("even"));
}


TEST(TestManipulator, StreamComplexStructure) {

    std::map<std::string, std::list<int>> complex_src{
            {"abc", {1, 2, 3}}, {"def", {-1, -2, -3}}, {"odd", {1, 3, 5, 7, 9, 11}}, {"even", {2, 4, 6, 8, 10, 12}}};
    std::map<std::string, std::list<int>> complex_dst;

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator << complex_src;
    manipulator.Reset();
    manipulator >> complex_dst;

    EXPECT_EQ(complex_src.size(), complex_dst.size());
    EXPECT_EQ(complex_src.at("abc"), complex_dst.at("abc"));
    EXPECT_EQ(complex_src.at("def"), complex_dst.at("def"));
    EXPECT_EQ(complex_src.at("odd"), complex_dst.at("odd"));
    EXPECT_EQ(complex_src.at("even"), complex_dst.at("even"));
}


TEST(TestManipulator, EOFofManipulator) {

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};

    manipulator << IPSUM_LOREM_TEXT;

    manipulator.Reset();
    std::uint64_t i = 0;
    while (!manipulator.IsEOF()) {
        char c;
        manipulator.Read(c);
        ++i;
    }

    EXPECT_EQ(i, IPSUM_LOREM_TEXT.size() + sizeof(std::uint64_t));
}


TEST(TestManipulator, EndianAwareness) {

    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator manipulator{memory};
    manipulator.SetEndianAware(true);

    union EndianTest {
        std::uint32_t v;
        unsigned char c[4];
    } endian_test{};
    endian_test.v = 0x1337dead;
    bool this_cpu_is_little_endian = (endian_test.c[3] == 0x13);

    manipulator << static_cast<std::uint32_t>(0x1337dead);
    if (this_cpu_is_little_endian) {
        EXPECT_EQ(memory.at(0), static_cast<std::byte>(0x13));
        EXPECT_EQ(memory.at(1), static_cast<std::byte>(0x37));
        EXPECT_EQ(memory.at(2), static_cast<std::byte>(0xde));
        EXPECT_EQ(memory.at(3), static_cast<std::byte>(0xad));
    } else {
        EXPECT_EQ(memory.at(0), static_cast<std::byte>(0xad));
        EXPECT_EQ(memory.at(1), static_cast<std::byte>(0xde));
        EXPECT_EQ(memory.at(2), static_cast<std::byte>(0x37));
        EXPECT_EQ(memory.at(3), static_cast<std::byte>(0x13));
    }

    manipulator.Reset();
    manipulator.SetEndianAware(false);
    manipulator << static_cast<std::uint32_t>(0x1337dead);

    if (this_cpu_is_little_endian) {
        EXPECT_EQ(memory.at(0), static_cast<std::byte>(0xad));
        EXPECT_EQ(memory.at(1), static_cast<std::byte>(0xde));
        EXPECT_EQ(memory.at(2), static_cast<std::byte>(0x37));
        EXPECT_EQ(memory.at(3), static_cast<std::byte>(0x13));
    } else {
        EXPECT_EQ(memory.at(0), static_cast<std::byte>(0xad));
        EXPECT_EQ(memory.at(1), static_cast<std::byte>(0xde));
        EXPECT_EQ(memory.at(2), static_cast<std::byte>(0x37));
        EXPECT_EQ(memory.at(3), static_cast<std::byte>(0x13));
    }
}

/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#include <gtest/gtest.h>

#include <headcode/memtool/memtool.hpp>

using namespace headcode::memtool;


/**
 * @brief   Test manipulator capabilities fixture class.
 */
class TestManipulatorFixture : public testing::Test {

protected:
    std::vector<std::byte> memory_;
    headcode::memtool::MemoryManipulator manipulator_{memory_};
    
    char c_ = -12;
    unsigned char uc_ = 37;
    std::int16_t s_ = -678;
    std::uint16_t us_ = 5096;
    std::int32_t i_ = -3209;
    std::uint32_t ui_ = 93458;
    std::int64_t l_ = -4328498989;
    std::uint64_t ul_ = 133847382;
    float f_ = 0.13f;
    double d_ = 13.430490903652;
    std::string str_{"The quick brown fox jumped over the lazy dog."};
    
    std::vector<std::byte> raw_memory_;
    
    /**
     * @brief   Setup the fixture.
     */
    void SetUp() override {
        
        manipulator_.Write(c_);
        manipulator_.Write(uc_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 2u);
        
        manipulator_.Write(s_);
        manipulator_.Write(us_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 6u);
        
        manipulator_.Write(i_);
        manipulator_.Write(ui_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 14u);
        
        manipulator_.Write(l_);
        manipulator_.Write(ul_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 30u);
        
        manipulator_.Write(f_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 34u);
        
        manipulator_.Write(d_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 42u);
        
        manipulator_.Write(str_);
        ASSERT_EQ(manipulator_.GetMemory().size(), 95u);
        
        std::vector<std::byte> memory{1000};
        memory[0] = std::byte{97};
        memory[100] = std::byte{138};
        memory[200] = std::byte{14};
        memory[500] = std::byte{234};
        memory[999] = std::byte{1};
        
        manipulator_.Write(memory);
        
        ASSERT_EQ(manipulator_.GetMemory().size(), 1103u);
        
        raw_memory_.resize(128);
        for (auto i = 0; i < 128; ++i) {
            raw_memory_[i] = std::byte{static_cast<unsigned char>(i)};
        }
        
        ASSERT_STREQ(headcode::memtool::MemoryToCanonicalString(raw_memory_).c_str(),
"\
00000000   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f   |........ ........|\n\
00000010   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f   |........ ........|\n\
00000020   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f   | !\"#$%&' ()*+,-./|\n\
00000030   30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f   |01234567 89:;<=>?|\n\
00000040   40 41 42 43 44 45 46 47  48 49 4a 4b 4c 4d 4e 4f   |@ABCDEFG HIJKLMNO|\n\
00000050   50 51 52 53 54 55 56 57  58 59 5a 5b 5c 5d 5e 5f   |PQRSTUVW XYZ[\\]^_|\n\
00000060   60 61 62 63 64 65 66 67  68 69 6a 6b 6c 6d 6e 6f   |`abcdefg hijklmno|\n\
00000070   70 71 72 73 74 75 76 77  78 79 7a 7b 7c 7d 7e 7f   |pqrstuvw xyz{|}~.|");
    }
    
    /**
     * @brief   Wind down the test fixture.
     */
    void TearDown() override {
    }
};


TEST_F(TestManipulatorFixture, ShallowCopy) {
    headcode::memtool::MemoryManipulator buffer_copy = headcode::memtool::MemoryManipulator{manipulator_};
    EXPECT_EQ(buffer_copy.GetMemory().data(), manipulator_.GetMemory().data());
}


TEST_F(TestManipulatorFixture, Pop) {
    
    auto buffer_copy = headcode::memtool::MemoryManipulator{manipulator_};
    
    char c;
    unsigned char uc;
    std::int16_t s;
    std::uint16_t us;
    std::int32_t i;
    std::uint32_t ui;
    std::int64_t l;
    std::uint64_t ul;
    float f;
    double d;
    std::string str;
    
    EXPECT_EQ(buffer_copy.Read(c), c_);
    EXPECT_EQ(buffer_copy.Read(uc), uc_);
    EXPECT_EQ(buffer_copy.Read(s), s_);
    EXPECT_EQ(buffer_copy.Read(us), us_);
    EXPECT_EQ(buffer_copy.Read(i), i_);
    EXPECT_EQ(buffer_copy.Read(ui), ui_);
    EXPECT_EQ(buffer_copy.Read(l), l_);
    EXPECT_EQ(buffer_copy.Read(ul), ul_);
    EXPECT_EQ(buffer_copy.Read(f), f_);
    EXPECT_EQ(buffer_copy.Read(d), d_);
    EXPECT_EQ(buffer_copy.Read(str), str_);

    std::vector<std::byte> memory;
    buffer_copy.Read(memory);
    EXPECT_EQ(std::to_integer<unsigned char>(memory[0]), 97u);
    EXPECT_EQ(std::to_integer<unsigned char>(memory[100]), 138u);
    EXPECT_EQ(std::to_integer<unsigned char>(memory[200]), 14u);
    EXPECT_EQ(std::to_integer<unsigned char>(memory[500]), 234u);
    EXPECT_EQ(std::to_integer<unsigned char>(memory[999]), 1u);
}


TEST_F(TestManipulatorFixture, list) {
    
    std::list<uint64_t> list_src{1, 4, 2000, 39898};
    std::list<uint64_t> list_dst;
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.Write(list_src);
    buffer.Reset();
    buffer.Read(list_dst);
    
    auto iter = list_dst.begin();
    EXPECT_EQ(*iter++, 1u);
    EXPECT_EQ(*iter++, 4u);
    EXPECT_EQ(*iter++, 2000u);
    EXPECT_EQ(*iter++, 39898u);
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(std::uint64_t) * list_src.size());
}


TEST_F(TestManipulatorFixture, map) {
    
    std::map<std::string, std::string> map_src{
            {"one", "eins"},
            {"two", "zwei"},
            {"three", "drei"},
            {"four", "vier"}
    };
    std::map<std::string, std::string> map_dst;
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.Write(map_src);
    buffer.Reset();
    buffer.Read(map_dst);
    
    EXPECT_EQ(map_src.size(), map_dst.size());
    EXPECT_STREQ(map_src.at("one").c_str(), map_dst.at("one").c_str());
    EXPECT_STREQ(map_src.at("two").c_str(), map_dst.at("two").c_str());
    EXPECT_STREQ(map_src.at("three").c_str(), map_dst.at("three").c_str());
    EXPECT_STREQ(map_src.at("four").c_str(), map_dst.at("four").c_str());
}


TEST_F(TestManipulatorFixture, set) {
    
    std::set<char> set_src{'q', 'k', 'd'};
    std::set<char> set_dst;
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.Write(set_src);
    buffer.Reset();
    buffer.Read(set_dst);
    
    EXPECT_NE(set_dst.find('q'), set_dst.end());
    EXPECT_NE(set_dst.find('k'), set_dst.end());
    EXPECT_NE(set_dst.find('d'), set_dst.end());
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + set_src.size() * sizeof(char));
}


TEST_F(TestManipulatorFixture, vector) {
    
    std::vector<std::string> vector_src{"blue", "green", "red", "yellow", "white", "black"};
    std::vector<std::string> vector_dst;
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.Write(vector_src);
    buffer.Reset();
    buffer.Read(vector_dst);
    
    auto iter = vector_dst.begin();
    EXPECT_EQ(*iter++, "blue");
    EXPECT_EQ(*iter++, "green");
    EXPECT_EQ(*iter++, "red");
    EXPECT_EQ(*iter++, "yellow");
    EXPECT_EQ(*iter++, "white");
    EXPECT_EQ(*iter++, "black");
    
    auto item_size_sum = 0ul;
    std::for_each(vector_src.begin(), vector_src.end(), [&](auto const & s) { item_size_sum += s.size(); });
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(std::uint64_t) * vector_src.size() + item_size_sum);
}


TEST_F(TestManipulatorFixture, valarray) {
    
    std::valarray<float> valarray_src{3.1415f, -0.00001f, 123456789.1234f};
    std::valarray<float> valarray_dst;
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.Write(valarray_src);
    buffer.Reset();
    buffer.Read(valarray_dst);
    
    EXPECT_EQ(valarray_src.size(), valarray_dst.size());
    EXPECT_EQ(valarray_src[0], valarray_dst[0]);
    EXPECT_EQ(valarray_src[1], valarray_dst[1]);
    EXPECT_EQ(valarray_src[2], valarray_dst[2]);
    EXPECT_EQ(memory.size(), sizeof(std::uint64_t) + sizeof(float) * valarray_src.size());
}


TEST_F(TestManipulatorFixture, string) {
    
    std::vector<std::byte> memory{128};
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.SetPosition(0);
    buffer.Write(std::string{"The brown fox jumped over the lazy dog."});
    buffer.Write(3.1514f);
    
    std::string s;
    float f;
    
    buffer.SetPosition(0);
    EXPECT_STREQ(buffer.Read(s).c_str(), "The brown fox jumped over the lazy dog.");
    EXPECT_EQ(buffer.Read(f), 3.1514f);
}


TEST_F(TestManipulatorFixture, complex) {
    
    std::map<std::string, std::list<int>> complex_src{
            {"abc", {1, 2, 3}},
            {"def", {-1, -2, -3}},
            {"odd", {1, 3, 5, 7, 9, 11}},
            {"even", {2, 4, 6, 8, 10, 12}}
    };
    std::map<std::string, std::list<int>> complex_dst;
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer.Write(complex_src);
    buffer.Reset();
    buffer.Read(complex_dst);
    
    EXPECT_EQ(complex_src.size(), complex_dst.size());
    EXPECT_EQ(complex_src.at("abc"), complex_dst.at("abc"));
    EXPECT_EQ(complex_src.at("def"), complex_dst.at("def"));
    EXPECT_EQ(complex_src.at("odd"), complex_dst.at("odd"));
    EXPECT_EQ(complex_src.at("even"), complex_dst.at("even"));
}


TEST_F(TestManipulatorFixture, streaming) {
    
    std::vector<std::byte> memory;
    headcode::memtool::MemoryManipulator buffer{memory};
    
    buffer << c_ << uc_ << s_ << us_ << i_ << ui_ << l_ << ul_ << f_ << d_ << str_;
    
    char c;
    unsigned char uc;
    int16_t s;
    uint16_t us;
    int32_t i;
    uint32_t ui;
    int64_t l;
    uint64_t ul;
    float f;
    double d;
    std::string str;
    
    buffer.Reset();
    buffer >> c >> uc >> s >> us >> i >> ui >> l >> ul >> f >> d >> str;
    
    EXPECT_EQ(c, c_);
    EXPECT_EQ(uc, uc_);
    EXPECT_EQ(s, s_);
    EXPECT_EQ(us, us_);
    EXPECT_EQ(i, i_);
    EXPECT_EQ(ui, ui_);
    EXPECT_EQ(l, l_);
    EXPECT_EQ(ul, ul_);
    EXPECT_EQ(f, f_);
    EXPECT_EQ(d, d_);
    EXPECT_STREQ(str.c_str(), str_.c_str());
    
    std::vector<std::byte> raw_memory;
    buffer.Reset();
    buffer << raw_memory_;
    buffer.Reset();
    buffer >> raw_memory;
    EXPECT_EQ(raw_memory_, raw_memory);
    
    std::list<uint64_t> list_src{1, 4, 2000, 39898};
    std::list<uint64_t> list_dst;
    buffer.Reset();
    buffer << list_src;
    buffer.Reset();
    buffer >> list_dst;
    EXPECT_EQ(list_src, list_dst);
    
    std::set<char> set_src{'q', 'k', 'd'};
    std::set<char> set_dst;
    buffer.Reset();
    buffer << set_src;
    buffer.Reset();
    buffer >> set_dst;
    EXPECT_EQ(set_src, set_dst);
    
    std::vector<std::string> vector_src{"blue", "green", "red", "yellow", "white", "black"};
    std::vector<std::string> vector_dst;
    buffer.Reset();
    buffer << vector_src;
    buffer.Reset();
    buffer >> vector_dst;
    EXPECT_EQ(vector_src, vector_dst);
    
    std::valarray<float> valarray_src{3.1415f, -0.00001f, 123456789.1234f};
    std::valarray<float> valarray_dst;
    buffer.Reset();
    buffer << valarray_src;
    buffer.Reset();
    buffer >> valarray_dst;
    EXPECT_EQ(valarray_src.size(), valarray_dst.size());
    EXPECT_EQ(valarray_src[0], valarray_dst[0]);
    EXPECT_EQ(valarray_src[1], valarray_dst[1]);
    EXPECT_EQ(valarray_src[2], valarray_dst[2]);
}

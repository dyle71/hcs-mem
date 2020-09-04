/*
 * This file is part of the headcode.space memtool.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020 headcode.space
 * https://www.headcode.space, <info@headcode.space>
 */

#ifndef HEADCODE_SPACE_MEMTOOL_MEMTOOL_MANIPULATOR_HPP
#define HEADCODE_SPACE_MEMTOOL_MEMTOOL_MANIPULATOR_HPP

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <list>
#include <map>
#include <set>
#include <string>
#include <valarray>
#include <vector>

#include <endian.h>


/**
 * @brief   The headcode memtool namespace
 */
namespace headcode::memtool {

/**
 * @brief   This is a read/write mechanism of arbitrary data on a memory area which can handle endian encoding.
 * The MemoryManipulator **does not take ownership** of the memory area managed. It works like this:
 * @code
 *      std::vector<std::byte> blob;
 *      {
 *          headcode::memtool::MemoryManipulator buffer{blob};
 *          buffer << "foo";
 *          buffer << 42ul;
 *      }
 *      std::cout << blob.size() << std::end;       // yields 19 = 8 (size of uint64_t) + 3 ('foo') + 8 (size of ul)
 * @endcode
 */
class MemoryManipulator {

    bool endian_aware_ = false;                 //!< @brief Enforces endian conversion on POD input.
    mutable std::uint64_t position_ = 0;        //!< @brief read/write position.
    std::vector<std::byte> & memory_;           //!< @brief The memory we work on.

public:
    /**
     * @brief   Constructor
     * @param   memory              the memory managed.
     */
    explicit MemoryManipulator(std::vector<std::byte> & memory) : memory_{memory} {
    }

    /**
     * @brief   Copy constructor.
     * This is a deep copy but with rw position set to 0.
     * @param   rhs         right hand side manipulator
     */
    MemoryManipulator(MemoryManipulator const & rhs)
            : endian_aware_(rhs.endian_aware_), position_(0), memory_{rhs.memory_} {
    }

    /**
     * @brief   Destructor
     */
    virtual ~MemoryManipulator() = default;

    /**
     * @brief   Puts another data particle on top of the manipulator at the read/write position.
     * @param   data        pointer to data
     * @param   size        size of data
     * @return  new position index value after add
     */
    std::uint64_t Add(void const * data, std::uint64_t size) {
        Grow(size);
        if (GetFree() >= size) {
            std::memcpy(GetPositionPointer(), data, size);
            position_ += size;
        }
        return position_;
    }

    /**
     * @brief   Advances read position by some bytes.
     * This will move to eof if the new position is out of bounds.
     * @param   bytes       number of bytes to advance the read position
     * @return  new position index value
     */
    std::uint64_t Advance(std::uint64_t bytes) const {
        if (GetPosition() + bytes > memory_.size()) {
            position_ = memory_.size();
        } else {
            position_ += bytes;
        }
        return position_;
    }

    /**
     * @brief   Returns the free space left for write operations w.r.t. the write position.
     * @return  The number of bytes still left.
     */
    std::uint64_t GetFree() const {
        return memory_.size() - position_;
    }

    /**
     * @brief   Gets the current read/write position.
     * @return  the current read/write position
     */
    std::uint64_t GetPosition() const {
        return position_;
    }

    /**
     * @brief   Gets the amount of remaining bytes not yet read in the manipulator.
     * @return  the amount of valid bytes still ahead from the current read position
     */
    std::uint64_t GetRemaining() const {
        return memory_.size() - GetPosition();
    }

    /**
     * @brief   Returns the managed memory.
     * @return  The memory.
     */
    std::vector<std::byte> const & GetMemory() const {
        return memory_;
    }

    /**
     * @brief   Checks if this manipulator does endian conversion.
     * @return  true, if it does
     */
    bool IsEndianAware() const {
        return endian_aware_;
    }

    /**
     * @brief   Checks if we are at end of field (at the end of available data to read).
     * @return  true, if there is no more left to be read.
     */
    bool IsEOF() const {
        return GetPosition() == memory_.size();
    }

    /**
     * @brief   Picks data from the manipulator.
     * @param   data       pointer to receiving data
     * @param   size       size of data
     */
    void Pick(void * data, std::uint64_t size) const {
        if (data && (position_ + size <= memory_.size())) {
            std::memcpy(data, GetPositionPointer(), size);
            position_ += size;
        }
    }

    /**
     * @brief   Reads a bool from the current read/write position.
     * @param   b       the bool to read
     * @return  b read
     */
    bool const & Read(bool & b) const {
        Pick(&b, sizeof(b));
        return b;
    }

    /**
     * @brief   Reads a byte from the current read/write position.
     * @param   b       the byte to read
     * @return  b read
     */
    std::byte const & Read(std::byte & b) const {
        Pick(&b, sizeof(b));
        return b;
    }

    /**
     * @brief   Reads a char from the current read/write position.
     * @param   c       the char to read
     * @return  c read
     */
    char const & Read(char & c) const {
        Pick(&c, sizeof(c));
        return c;
    }

    /**
     * @brief   Reads an unsigned char from the current read/write position.
     * @param   c       the char to read
     * @return  c
     */
    unsigned char const & Read(unsigned char & c) const {
        Pick(&c, sizeof(c));
        return c;
    }

    /**
     * @brief   Reads an int16_t from the current read/write position.
     * @param   i       the int16_t to read
     * @return  i read
     */
    std::int16_t const & Read(std::int16_t & i) const {
        Pick(&i, sizeof(i));
        if (IsEndianAware()) {
            i = be16toh(i);
        }
        return i;
    }

    /**
     * @brief   Reads an uint16_t from the current read/write position.
     * @param   u       the uint16_t to read
     * @return  u read
     */
    std::uint16_t const & Read(std::uint16_t & u) const {
        Pick(&u, sizeof(u));
        if (IsEndianAware()) {
            u = be16toh(u);
        }
        return u;
    }

    /**
     * @brief   Reads an int32_t from the current read/write position.
     * @param   i       the int32_t to read
     * @return  i read
     */
    std::int32_t const & Read(int32_t & i) const {
        Pick(&i, sizeof(i));
        if (IsEndianAware()) {
            i = be32toh(i);
        }
        return i;
    }

    /**
     * @brief   Reads an uint32_t from the current read/write position.
     * @param   u       the uint32_t to read
     * @return  u read
     */
    std::uint32_t const & Read(std::uint32_t & u) const {
        Pick(&u, sizeof(u));
        if (IsEndianAware()) {
            u = be32toh(u);
        }
        return u;
    }

    /**
     * @brief   Reads an int64_t from the current read/write position.
     * @param   i       the int64_t to read
     * @return  i read
     */
    std::int64_t const & Read(std::int64_t & i) const {
        Pick(&i, sizeof(i));
        if (IsEndianAware()) {
            i = be64toh(i);
        }
        return i;
    }

    /**
     * @brief   Reads an uint64_t from the current read/write position.
     * @param   u       the uint64_t to read
     * @return  u read
     */
    std::uint64_t const & Read(std::uint64_t & u) const {
        Pick(&u, sizeof(u));
        if (IsEndianAware()) {
            u = be64toh(u);
        }
        return u;
    }

    /**
     * @brief   Reads an float from the current read/write position.
     * @param   f       the float to read
     * @return  f read
     */
    float const & Read(float & f) const {
        Pick(&f, sizeof(f));
        return f;
    }

    /**
     * @brief   Reads an double from the current read/write position.
     * @param   d       the double to read
     * @return  d read
     */
    double const & Read(double & d) const {
        Pick(&d, sizeof(d));
        return d;
    }

    /**
     * @brief   Gets a memory from the current read/write position.
     * @param   m      the memory to get
     * @return  m read
     */
    std::vector<std::byte> const & Read(std::vector<std::byte> & m) const {
        std::uint64_t size{0};
        Read(size);
        if (GetRemaining() < size) {
            size = GetRemaining();
        }
        m.resize(size);
        Pick(m.data(), size);

        return m;
    }

    /**
     * @brief   Gets a string from the current read/write position.
     * @param   s       the string to get
     * @return  s read
     */
    std::string const & Read(std::string & s) const {
        std::uint64_t size{0};
        Read(size);
        if (GetRemaining() < size) {
            size = GetRemaining();
        }
        s = std::string{GetPositionPointer(), size};
        Advance(size);
        return s;
    }

    /**
     * @brief   Gets a list of items.
     * @param   l       the list to get
     * @return  l read
     */
    template <class T>
    std::list<T> const & Read(std::list<T> & l) const {
        l.clear();
        std::uint64_t size{0};
        Read(size);
        for (std::uint64_t i = 0; i < size; ++i) {
            T e{};
            Read(e);
            l.push_back(e);
        }
        return l;
    }

    /**
     * @brief   Gets a map of items.
     * @param   m       the map to get
     * @return  m read
     */
    template <class K, class T>
    std::map<K, T> const & Read(std::map<K, T> & m) const {
        m.clear();
        std::uint64_t size{0};
        Read(size);
        for (std::uint64_t i = 0; i < size; ++i) {
            K key;
            Read(key);
            T value;
            Read(value);
            m[key] = value;
        }
        return m;
    }

    /**
     * @brief   Gets a set of items.
     * @param   s       the set to get
     * @return  s read
     */
    template <class T>
    std::set<T> const & Read(std::set<T> & s) const {
        s.clear();
        std::uint64_t size{0};
        Read(size);
        for (std::uint64_t i = 0; i < size; ++i) {
            T e;
            Read(e);
            s.insert(e);
        }
        return s;
    }

    /**
     * @brief   Gets a valarray of items.
     * @param   v       the valarray to get
     * @return  v read
     */
    template <class T>
    std::valarray<T> const & Read(std::valarray<T> & v) const {
        std::uint64_t size{0};
        Read(size);
        v.resize(size);
        for (std::uint64_t i = 0; i < size; ++i) {
            T e;
            Read(e);
            v[i] = e;
        }
        return v;
    }

    /**
     * @brief   Gets a vector of items.
     * @param   v       the vector to get
     * @return  v read
     */
    template <class T>
    std::vector<T> const & Read(std::vector<T> & v) const {
        v.clear();
        std::uint64_t size{0};
        Read(size);
        v.reserve(size);
        for (std::uint64_t i = 0; i < size; ++i) {
            T e;
            Read(e);
            v.push_back(e);
        }
        return v;
    }

    /**
     * @brief   Resets read/write position.
     * This does not discard, free or delete any memory already held within the manipulator.
     * It just repositions the read/write header to the beginning of the manipulator.
     */
    void Reset() const {
        SetPosition(0);
    }

    /**
     * @brief   Sets this manipulator to do endian conversion.
     * @param   endian_aware        endian conversion flag
     */
    void SetEndianAware(bool endian_aware) {
        endian_aware_ = endian_aware;
    }

    /**
     * @brief   Sets the read/write position.
     * Does nothing, if the position is out of bounds.
     * @param   position     the new read/write position
     */
    void SetPosition(std::uint64_t position) const {
        if (position <= memory_.size()) {
            position_ = position;
        }
    }

    /**
     * @brief   Adds a bool at the current read/write position.
     * @param   b       the bool to add
     */
    void Write(bool b) {
        Add(&b, sizeof(b));
    }

    /**
     * @brief   Adds a byte at the current read/write position.
     * @param   b       the byte to add
     */
    void Write(std::byte b) {
        Add(&b, sizeof(b));
    }

    /**
     * @brief   Adds a char at the current read/write position.
     * @param   c       the char to add
     */
    void Write(char c) {
        Add(&c, sizeof(c));
    }

    /**
     * @brief   Adds an unsigned char at the current read/write position.
     * @param   c       the char to add
     */
    void Write(unsigned char c) {
        Add(&c, sizeof(c));
    }

    /**
     * @brief   Adds an int16_t at the current read/write position.
     * @param   i       the int16_t to add
     */
    void Write(std::int16_t i) {
        if (IsEndianAware()) {
            i = htobe16(i);
        }
        Add(&i, sizeof(i));
    }

    /**
     * @brief   Adds an uint16_t at the current read/write position.
     * @param   u       the uint16_t to add
     */
    void Write(std::uint16_t u) {
        if (IsEndianAware()) {
            u = htobe16(u);
        }
        Add(&u, sizeof(u));
    }

    /**
     * @brief   Adds an int32_t at the current read/write position.
     * @param   i       the int32_t to add
     */
    void Write(std::int32_t i) {
        if (IsEndianAware()) {
            i = htobe32(i);
        }
        Add(&i, sizeof(i));
    }

    /**
     * @brief   Adds an uint32_t at the current read/write position.
     * @param   u       the uint32_t to add
     */
    void Write(std::uint32_t u) {
        if (IsEndianAware()) {
            u = htobe32(u);
        }
        Add(&u, sizeof(u));
    }

    /**
     * @brief   Adds an int64_t at the current read/write position.
     * @param   i       the int64_t to add
     */
    void Write(std::int64_t i) {
        if (IsEndianAware()) {
            i = htobe64(i);
        }
        Add(&i, sizeof(i));
    }

    /**
     * @brief   Adds an uint64_t at the current read/write position.
     * @param   u       the uint64_t to add
     */
    void Write(std::uint64_t u) {
        if (IsEndianAware()) {
            u = htobe64(u);
        }
        Add(&u, sizeof(u));
    }

    /**
     * @brief   Adds a float at the current read/write position.
     * @param   f       the float to add
     */
    void Write(float f) {
        Add(&f, sizeof(f));
    }

    /**
     * @brief   Adds a double at the current read/write position.
     * @param   d       the double to add
     */
    void Write(double d) {
        Add(&d, sizeof(d));
    }

    /**
     * @brief   Adds a memory at the current read/write position.
     * @param   m       the memory to add
     */
    void Write(std::vector<std::byte> const & m) {
        std::uint64_t size = m.size();
        Write(size);
        Add(reinterpret_cast<char const *>(m.data()), size);
    }

    /**
     * @brief   Adds a string at the current read/write position.
     * @param   s       the string to add
     */
    void Write(std::string const & s) {
        std::uint64_t size = s.size();
        Write(size);
        Add(s.data(), size);
    }

    /**
     * @brief   Writes a list of items.
     * @param   l       the list to write
     */
    template <class T>
    void Write(std::list<T> const & l) {
        Write(static_cast<std::uint64_t>(l.size()));
        for (auto e : l) {
            Write(e);
        }
    }

    /**
     * @brief   Writes a map of items.
     * @param   m       the map to write
     */
    template <class K, class T>
    void Write(std::map<K, T> const & m) {
        Write(static_cast<std::uint64_t>(m.size()));
        for (auto p : m) {
            Write(p.first);
            Write(p.second);
        }
    }

    /**
     * @brief   Writes a set of items.
     * @param   s       the set to write
     */
    template <class T>
    void Write(std::set<T> const & s) {
        Write(static_cast<std::uint64_t>(s.size()));
        for (auto e : s) {
            Write(e);
        }
    }

    /**
     * @brief   Writes a valarray of items.
     * @param   v       the valarray to write
     */
    template <class T>
    void Write(std::valarray<T> const & v) {
        Write(static_cast<std::uint64_t>(v.size()));
        for (auto e : v) {
            Write(e);
        }
    }

    /**
     * @brief   Writes a vector of items.
     * @param   v       the vector to write
     */
    template <class T>
    void Write(std::vector<T> const & v) {
        Write(static_cast<std::uint64_t>(v.size()));
        for (auto e : v) {
            Write(e);
        }
    }

private:
    /**
     * @brief   Gets current read/write position as memory pointer.
     * @return  current read write position into memory
     */
    char * GetPositionPointer() {
        return reinterpret_cast<char *>(memory_.data()) + position_;
    }

    /**
     * @brief   Gets current read/write position as const memory pointer (const version).
     * @return  current read write position into memory
     */
    char const * GetPositionPointer() const {
        return reinterpret_cast<char const *>(memory_.data()) + position_;
    }

    /**
     * @brief   Grows the memory managed (if necessary).
     * @param   needed_space    amount of needed free size within the memory managed
     */
    void Grow(std::uint64_t needed_space) {

        if (position_ + needed_space <= memory_.size()) {
            return;
        }

        std::uint64_t new_size = position_ + needed_space;
        if (memory_.capacity() < new_size) {
            memory_.reserve(new_size);
        }
        memory_.resize(new_size);
    }
};

}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   c           char
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, char c) {
    lhs.Write(c);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   uc          unsigned char
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, unsigned char uc) {
    lhs.Write(uc);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   b           byte
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::byte b) {
    lhs.Write(b);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   i           int16_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::int16_t i) {
    lhs.Write(i);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   ui          uint16_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::uint16_t ui) {
    lhs.Write(ui);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   i           int32_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::int32_t i) {
    lhs.Write(i);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   ui          uint32_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::uint32_t ui) {
    lhs.Write(ui);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   i           int64_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::int64_t i) {
    lhs.Write(i);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   ui          uint64_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::uint64_t ui) {
    lhs.Write(ui);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   f           float
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, float f) {
    lhs.Write(f);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   d           double
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, double d) {
    lhs.Write(d);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   m           memory
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs,
                                                         std::vector<std::byte> const & m) {
    lhs.Write(m);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   s           string
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs,
                                                         std::string const & s) {
    lhs.Write(s);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   l           list
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::list<T> const & l) {
    lhs.Write(l);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   m           map
 * @return  lhs
 */
template <class K, class T>
headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs,
                                                  std::map<K, T> const & m) {
    lhs.Write(m);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   s           set
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs, std::set<T> const & s) {
    lhs.Write(s);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   v           valarray
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs,
                                                  std::valarray<T> const & v) {
    lhs.Write(v);
    return lhs;
}

/**
 * @brief   Stream in
 * @param   lhs         left-hand-side manipulator
 * @param   v           vector
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator<<(headcode::memtool::MemoryManipulator & lhs,
                                                  std::vector<T> const & v) {
    lhs.Write(v);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   c           char
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, char & c) {
    lhs.Read(c);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   uc          unsigned char
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs,
                                                         unsigned char & uc) {
    lhs.Read(uc);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   b           byte
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::byte & b) {
    lhs.Read(b);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   i           int16_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::int16_t & i) {
    lhs.Read(i);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   ui          uint16_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs,
                                                         std::uint16_t & ui) {
    lhs.Read(ui);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   i           int32_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::int32_t & i) {
    lhs.Read(i);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   ui          uint32_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs,
                                                         std::uint32_t & ui) {
    lhs.Read(ui);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   i           int64_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::int64_t & i) {
    lhs.Read(i);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   ui          uint64_t
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs,
                                                         std::uint64_t & ui) {
    lhs.Read(ui);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   f           float
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, float & f) {
    lhs.Read(f);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   d           double
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, double & d) {
    lhs.Read(d);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   m           memory
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs,
                                                         std::vector<std::byte> & m) {
    lhs.Read(m);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   s           string
 * @return  lhs
 */
inline headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::string & s) {
    lhs.Read(s);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   l           list
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::list<T> & l) {
    lhs.Read(l);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   m           map
 * @return  lhs
 */
template <class K, class T>
headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::map<K, T> & m) {
    lhs.Read(m);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   s           set
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::set<T> & s) {
    lhs.Read(s);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   v           valarray
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::valarray<T> & v) {
    lhs.Read(v);
    return lhs;
}

/**
 * @brief   Stream out
 * @param   lhs         left-hand-side manipulator
 * @param   v           vector
 * @return  lhs
 */
template <class T>
headcode::memtool::MemoryManipulator & operator>>(headcode::memtool::MemoryManipulator & lhs, std::vector<T> & v) {
    lhs.Read(v);
    return lhs;
}


#endif

//
// Created by james on 01/09/25.
//

#pragma once

#include <cstdint>
#include <string>
#include <ostream>
#include <type_traits>

namespace ut::test
{

    template<typename IntType> class BitType
    {
    public:
        IntType value;

        constexpr BitType() : value(0) {}
        constexpr explicit BitType(IntType x) : value(x) {}

        constexpr operator IntType() const { return value; }

        // Assignment
        constexpr BitType& operator=(IntType x) { value = x; return *this; }
        constexpr BitType& operator=(BitType const& x) { value = x.value; return *this; }

        // Arithmetic operations
        constexpr BitType operator+ (BitType const& x) const { return BitType(value + x.value); }
        constexpr BitType operator- (BitType const& x) const { return BitType(value - x.value); }
        constexpr BitType operator* (BitType const& x) const { return BitType(value * x.value); }
        constexpr BitType operator/ (BitType const& x) const { return BitType(value / x.value); }

        constexpr BitType& operator+=(BitType const& x) { value += x.value; return *this; }
        constexpr BitType& operator-=(BitType const& x) { value -= x.value; return *this; }

        // Bitwise operations
        constexpr BitType operator& (BitType const& x) const { return BitType(value & x.value); }
        constexpr BitType operator| (BitType const& x) const { return BitType(value | x.value); }
        constexpr BitType operator^ (BitType const& x) const { return BitType(value ^ x.value); }

        constexpr BitType operator~ () const { return BitType(~value); }

        constexpr BitType operator<<(int shift) const { return BitType(value << shift); }
        constexpr BitType operator>>(int shift) const { return BitType(value >> shift); }

        constexpr BitType& operator&=(BitType const& x) { value &= x.value; return *this; }
        constexpr BitType& operator|=(BitType const& x) { value |= x.value; return *this; }
        constexpr BitType& operator^=(BitType const& x) { value ^= x.value; return *this; }

        constexpr BitType& operator<<=(int shift) { value <<= shift; return *this; }
        constexpr BitType& operator>>=(int shift) { value >>= shift; return *this; }

        // Comparison operators
        constexpr bool operator==    (BitType const& x) const { return value == x.value; }
        constexpr bool operator!=    (BitType const& x) const { return value != x.value; }
        constexpr bool operator<     (BitType const& x) const { return value <  x.value; }
        constexpr bool operator>     (BitType const& x) const { return value >  x.value; }
        constexpr bool operator<=    (BitType const& x) const { return value <= x.value; }
        constexpr bool operator>=    (BitType const& x) const { return value >= x.value; }
    };

    // Specialized bit manipulation classes
    class Byte : public BitType<std::uint8_t>
    {
    public:
        using BitType::BitType;

        // Nibble extraction
        [[nodiscard]] constexpr Byte nybl0() const { return Byte((value >> 0) & 0x0F); }
        [[nodiscard]] constexpr Byte nybl1() const { return Byte((value >> 4) & 0x0F); }

        // Bit operations
        [[nodiscard]] constexpr bool getBit(size_t n) const { return ((value >> n) & 0x01); }
        [[nodiscard]] constexpr bool get0() const { return (value & 0b00000001) > 0; }
        [[nodiscard]] constexpr bool get1() const { return (value & 0b00000010) > 0; }
        [[nodiscard]] constexpr bool get2() const { return (value & 0b00000100) > 0; }
        [[nodiscard]] constexpr bool get3() const { return (value & 0b00001000) > 0; }
        [[nodiscard]] constexpr bool get4() const { return (value & 0b00010000) > 0; }
        [[nodiscard]] constexpr bool get5() const { return (value & 0b00100000) > 0; }
        [[nodiscard]] constexpr bool get6() const { return (value & 0b01000000) > 0; }
        [[nodiscard]] constexpr bool get7() const { return (value & 0b10000000) > 0; }

        [[nodiscard]] constexpr Byte flipBit(size_t n) const { return Byte(value ^ (1 << n)); }
        [[nodiscard]] constexpr Byte flip0() const { return Byte(value ^ 0b00000001); }
        [[nodiscard]] constexpr Byte flip1() const { return Byte(value ^ 0b00000010); }
        [[nodiscard]] constexpr Byte flip2() const { return Byte(value ^ 0b00000100); }
        [[nodiscard]] constexpr Byte flip3() const { return Byte(value ^ 0b00001000); }
        [[nodiscard]] constexpr Byte flip4() const { return Byte(value ^ 0b00010000); }
        [[nodiscard]] constexpr Byte flip5() const { return Byte(value ^ 0b00100000); }
        [[nodiscard]] constexpr Byte flip6() const { return Byte(value ^ 0b01000000); }
        [[nodiscard]] constexpr Byte flip7() const { return Byte(value ^ 0b10000000); }

        [[nodiscard]] constexpr Byte setBit(size_t n) const { return Byte(value | (1 << n)); }
        [[nodiscard]] constexpr Byte set0() const { return Byte(value | 0b00000001); }
        [[nodiscard]] constexpr Byte set1() const { return Byte(value | 0b00000010); }
        [[nodiscard]] constexpr Byte set2() const { return Byte(value | 0b00000100); }
        [[nodiscard]] constexpr Byte set3() const { return Byte(value | 0b00001000); }
        [[nodiscard]] constexpr Byte set4() const { return Byte(value | 0b00010000); }
        [[nodiscard]] constexpr Byte set5() const { return Byte(value | 0b00100000); }
        [[nodiscard]] constexpr Byte set6() const { return Byte(value | 0b01000000); }
        [[nodiscard]] constexpr Byte set7() const { return Byte(value | 0b10000000); }

        [[nodiscard]] constexpr Byte clearBit(size_t n) const { return Byte(value & ~(1 << n)); }
        [[nodiscard]] constexpr Byte clear0() const { return Byte( value & 0b11111110 ); }
        [[nodiscard]] constexpr Byte clear1() const { return Byte( value & 0b11111101 ); }
        [[nodiscard]] constexpr Byte clear2() const { return Byte( value & 0b11111011 ); }
        [[nodiscard]] constexpr Byte clear3() const { return Byte( value & 0b11110111 ); }
        [[nodiscard]] constexpr Byte clear4() const { return Byte( value & 0b11101111 ); }
        [[nodiscard]] constexpr Byte clear5() const { return Byte( value & 0b11011111 ); }
        [[nodiscard]] constexpr Byte clear6() const { return Byte( value & 0b10111111 ); }
        [[nodiscard]] constexpr Byte clear7() const { return Byte( value & 0b01111111 ); }

        // Rotation and shifting
        [[nodiscard]] constexpr Byte rotateLeft() const { return Byte((value << 1) | (value >> 7)); }
        [[nodiscard]] constexpr Byte rotateRight() const { return Byte((value >> 1) | (value << 7)); }

        [[nodiscard]] constexpr Byte shiftRight() const { return Byte(value >> 1); }
        [[nodiscard]] constexpr Byte shiftLeft() const { return Byte(value << 1); }

        [[nodiscard]] constexpr Byte shiftRightArithmetic() const
        {
            union { std::uint8_t b; std::int8_t i; } u{value};
            u.i >>= 1;
            return Byte(u.b);
        }

        [[nodiscard]] constexpr Byte swapNibbles() const { return Byte((value << 4) | (value >> 4)); }

        // Overflow detection
        [[nodiscard]] constexpr bool hasOverflow(Byte x) const { return value > (UINT8_MAX - x.value); }
        [[nodiscard]] constexpr bool hasOverflow(Byte a, Byte b) const { return value > (UINT8_MAX - a.value - b.value); }
        [[nodiscard]] constexpr bool hasHalfOverflow(Byte x) const { return (((value & 0xf) + (x.value & 0xf)) & 0x10) == 0x10; }
        [[nodiscard]] constexpr bool hasHalfOverflow(Byte a, Byte b) const { return (((value & 0xf) + (a.value & 0xf) + b.value) & 0x10) == 0x10; }

        [[nodiscard]] constexpr bool hasUnderflow(Byte x) const { return x.value > value; }
        [[nodiscard]] constexpr bool hasUnderflow(Byte a, Byte b) const { return std::int16_t(value) - a.value - b.value < 0; }
        [[nodiscard]] constexpr bool hasHalfUnderflow(Byte other) const { return (other.value & 0xf) > (value & 0xf); }
        [[nodiscard]] constexpr bool hasHalfUnderflow(Byte a, Byte b) const { return std::int16_t(value & 0xf) - (a.value & 0xf) - (b.value & 0xf) < 0; }

        [[nodiscard]] std::string toBinaryString() const
        {
            std::string result(8, '0');
            for (std::size_t i = 0; i < 8; ++i)
                result[7-i] = getBit(i) ? '1' : '0';
            return result;
        }

        [[nodiscard]] std::string toNibbleString() const
        {
            std::string result(9, '0');
            for (std::size_t i = 0; i < 4; ++i)
            {
                result[8-i] = getBit(i) ? '1' : '0';
                result[i] = getBit(7-i) ? '1' : '0';
            }
            result[4] = ' ';
            return result;
        }

        // Factory methods for creating from nibbles
        static constexpr Byte fromNibbles(Byte n0, Byte n1)
        {
            return Byte((n0.value & 0x0F) | ((n1.value & 0x0F) << 4));
        }
    };

    class Word : public BitType<std::uint16_t>
    {
    public:
        using BitType::BitType;

        // Byte extraction
        [[nodiscard]] constexpr Byte byte0() const { return Byte((value >> 0) & 0xFF); }
        [[nodiscard]] constexpr Byte byte1() const { return Byte((value >> 8) & 0xFF); }
        [[nodiscard]] constexpr Byte lo8() const { return Byte(value & 0xFF); }
        [[nodiscard]] constexpr Byte hi8() const { return Byte((value >> 8) & 0xFF); }

        // Overflow detection
        [[nodiscard]] constexpr bool hasOverflow(Word x) const { return value > (UINT16_MAX - x.value); }
        [[nodiscard]] constexpr bool hasHalfOverflow(Word x) const { return (((value & 0xfff) + (x.value & 0xfff)) & 0x1000) == 0x1000; }
        [[nodiscard]] constexpr bool hasUnderflow(Word x) const { return x.value > value; }
        [[nodiscard]] constexpr bool hasHalfUnderflow(Word x) const { return (x.value & 0xff) > (value & 0xff); }

        // Factory methods
        static constexpr Word fromBytes(Byte b0, Byte b1)
        {
            return Word(b0.value | (std::uint16_t(b1.value) << 8));
        }
    };

    class DWord : public BitType<std::uint32_t>
    {
    public:
        using BitType::BitType;

        // Byte extraction
        [[nodiscard]] constexpr Byte byte0() const { return Byte((value >> 0) & 0xFF); }
        [[nodiscard]] constexpr Byte byte1() const { return Byte((value >> 8) & 0xFF); }
        [[nodiscard]] constexpr Byte byte2() const { return Byte((value >> 16) & 0xFF); }
        [[nodiscard]] constexpr Byte byte3() const { return Byte((value >> 24) & 0xFF); }

        // Word extraction
        [[nodiscard]] constexpr Word word0() const { return Word((value >> 0) & 0xFFFF); }
        [[nodiscard]] constexpr Word word1() const { return Word((value >> 16) & 0xFFFF); }
        [[nodiscard]] constexpr Word lo16() const { return Word(value & 0xFFFF); }
        [[nodiscard]] constexpr Word hi16() const { return Word((value >> 16) & 0xFFFF); }

        // Factory methods
        static constexpr DWord fromBytes(Byte b0, Byte b1, Byte b2, Byte b3) {
            return DWord(b0.value | (std::uint32_t(b1.value) << 8) |
                         (std::uint32_t(b2.value) << 16) | (std::uint32_t(b3.value) << 24));
        }

        static constexpr DWord fromWords(Word w0, Word w1) {
            return DWord(w0.value | (std::uint32_t(w1.value) << 16));
        }
    };

    class QWord : public BitType<std::uint64_t> {
    public:
        using BitType::BitType;

        // Byte extraction
        [[nodiscard]] constexpr Byte byte0() const { return Byte((value >> 0 ) & 0xFF); }
        [[nodiscard]] constexpr Byte byte1() const { return Byte((value >> 8 ) & 0xFF); }
        [[nodiscard]] constexpr Byte byte2() const { return Byte((value >> 16) & 0xFF); }
        [[nodiscard]] constexpr Byte byte3() const { return Byte((value >> 24) & 0xFF); }
        [[nodiscard]] constexpr Byte byte4() const { return Byte((value >> 32) & 0xFF); }
        [[nodiscard]] constexpr Byte byte5() const { return Byte((value >> 40) & 0xFF); }
        [[nodiscard]] constexpr Byte byte6() const { return Byte((value >> 48) & 0xFF); }
        [[nodiscard]] constexpr Byte byte7() const { return Byte((value >> 56) & 0xFF); }

        // Word extraction
        [[nodiscard]] constexpr Word word0() const { return Word((value >> 0 ) & 0xFFFF); }
        [[nodiscard]] constexpr Word word1() const { return Word((value >> 16) & 0xFFFF); }
        [[nodiscard]] constexpr Word word2() const { return Word((value >> 32) & 0xFFFF); }
        [[nodiscard]] constexpr Word word3() const { return Word((value >> 48) & 0xFFFF); }

        // DWord extraction
        [[nodiscard]] constexpr DWord dword0() const { return DWord((value >> 0) & 0xFFFFFFFF); }
        [[nodiscard]] constexpr DWord dword1() const { return DWord((value >> 32) & 0xFFFFFFFF); }
        [[nodiscard]] constexpr DWord lo32() const { return DWord(value & 0xFFFFFFFF); }
        [[nodiscard]] constexpr DWord hi32() const { return DWord((value >> 32) & 0xFFFFFFFF); }

        // Factory methods
        static constexpr QWord fromBytes(Byte b0, Byte b1, Byte b2, Byte b3, Byte b4, Byte b5, Byte b6, Byte b7)
        {
            return QWord(b0.value |
                (std::uint64_t(b1.value) << 8 ) |
                (std::uint64_t(b2.value) << 16) |
                (std::uint64_t(b3.value) << 24) |
                (std::uint64_t(b4.value) << 32) |
                (std::uint64_t(b5.value) << 40) |
                (std::uint64_t(b6.value) << 48) |
                (std::uint64_t(b7.value) << 56));
        }

        static constexpr QWord fromWords(Word w0, Word w1, Word w2, Word w3)
        {
            return QWord(w0.value |
                (std::uint64_t(w1.value) << 16) |
                (std::uint64_t(w2.value) << 32) |
                (std::uint64_t(w3.value) << 48));
        }

        static constexpr QWord fromDWords(DWord d0, DWord d1)
        {
            return QWord(d0.value | (std::uint64_t(d1.value) << 32));
        }
    };

    // Endianness utilities
    enum class Endianness { Big, Little };

    inline Endianness getNativeEndianness()
    {
        union { std::uint32_t i; std::uint8_t c[4]; } u{0x1};
        return u.c[0] == 1 ? Endianness::Little : Endianness::Big;
    }

    // Type aliases for compatibility
    using byte_t = Byte;
    using word_t = Word;
    using dword_t = DWord;
    using qword_t = QWord;

    using b8 = Byte;
    using b16 = Word;
    using b32 = DWord;
    using b64 = QWord;


    inline std::ostream& operator<<(std::ostream& os, Byte const& c)
    {
        return os << c.toBinaryString();
    }

    inline std::string to_string(Byte const& c)
    {
        std::ostringstream ss;
        ss << c.toBinaryString();
        return ss.str();
    }

} // namespace ut
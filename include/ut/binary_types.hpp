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
        static_assert(std::is_integral_v<IntType>);

        IntType value;

        constexpr BitType() : value(0) {}
        constexpr BitType(IntType x) : value(x) {}

        //
        // Assignment
        //

        constexpr BitType& operator=(BitType const& x) { value = x.value; return *this; }
        constexpr BitType& operator=(IntType x) { value = x; return *this; }

        //
        // Arithmetic operations
        //

        constexpr BitType operator+ (BitType const& x) const { return BitType(value + x.value); }
        constexpr BitType operator- (BitType const& x) const { return BitType(value - x.value); }
        constexpr BitType operator* (BitType const& x) const { return BitType(value * x.value); }
        constexpr BitType operator/ (BitType const& x) const { return BitType(value / x.value); }

        constexpr BitType& operator+=(BitType const& x) { value += x.value; return *this; }
        constexpr BitType& operator-=(BitType const& x) { value -= x.value; return *this; }
        constexpr BitType& operator*=(BitType const& x) { value *= x.value; return *this; }
        constexpr BitType& operator/=(BitType const& x) { value /= x.value; return *this; }

        //
        // Bitwise operations
        //

        constexpr BitType operator& (BitType const& x) const { return BitType(value & x.value); }
        constexpr BitType operator| (BitType const& x) const { return BitType(value | x.value); }
        constexpr BitType operator^ (BitType const& x) const { return BitType(value ^ x.value); }

        constexpr BitType operator~ () const { return BitType(~value); }

        constexpr BitType operator<<(int shift) const { return BitType(value << shift); }
        constexpr BitType operator>>(int shift) const { return BitType(value >> shift); }

        constexpr BitType& operator&=(BitType const& x) { value &= x.value; return *this; }
        constexpr BitType& operator|=(BitType const& x) { value |= x.value; return *this; }
        constexpr BitType& operator^=(BitType const& x) { value ^= x.value; return *this; }

        constexpr BitType& operator<<=(int x) { value <<= x; return *this; }
        constexpr BitType& operator>>=(int x) { value >>= x; return *this; }

        //
        // Comparison operators
        //

        constexpr bool operator==    (BitType const& x) const { return value == x.value; }
        constexpr bool operator!=    (BitType const& x) const { return value != x.value; }
        constexpr bool operator<     (BitType const& x) const { return value <  x.value; }
        constexpr bool operator>     (BitType const& x) const { return value >  x.value; }
        constexpr bool operator<=    (BitType const& x) const { return value <= x.value; }
        constexpr bool operator>=    (BitType const& x) const { return value >= x.value; }

        //
        //
        //

        [[nodiscard]] std::string toBinString() const
        {
            constexpr int ndigits = sizeof(IntType)*8;

            std::string res; res.reserve(ndigits);
            for (int i = 0; i < ndigits; ++i)
            {
                int ri = ndigits - i - 1;
                res += ((value >> ri) & 1) ? '1' : '0';
            }

            return res;
        }

        [[nodiscard]] std::string toHexString() const
        {
            constexpr int ndigits = sizeof(IntType) * 2;
            constexpr char chars[] = "0123456789abcdef";

            std::string res; res.reserve(ndigits);
            for (int i = 0; i < ndigits; ++i)
            {
                int ri = ndigits - i - 1;
                res += chars[(value >> (ri * 4)) & 0xf];
            }

            return res;
        }

        [[nodiscard]] std::string toDecString() const
        {
            if (value == 0) return "0";

            constexpr int max_digits =
                sizeof(IntType) == 1 ? 3 :   // 255
                sizeof(IntType) == 2 ? 5 :   // 65535
                sizeof(IntType) == 4 ? 10 :  // 4294967295
                sizeof(IntType) == 8 ? 20 :  // 18446744073709551615
                25; // fallback

            char buffer[max_digits + 1]; // +1 for null terminator
            int pos = max_digits;
            buffer[pos] = '\0';

            IntType temp = value;

            do // Build digits from right to left
            {
                buffer[--pos] = '0' + (temp % 10);
                temp /= 10;
            } while (temp > 0);

            return std::string(buffer + pos);
        }
    };

    // Specialized bit manipulation classes
    class Byte : public BitType<std::uint8_t>
    {
    public:
        using BitType::BitType;

        //
        // Nibble extraction
        //

        [[nodiscard]] constexpr Byte nybl0() const { return Byte((value >> 0) & 0x0F); }
        [[nodiscard]] constexpr Byte nybl1() const { return Byte((value >> 4) & 0x0F); }

        //
        // Bit operations
        //

        [[nodiscard]] constexpr bool bit(size_t n) const { return (value >> n) & 0x01; }
        [[nodiscard]] constexpr bool bit0() const { return (value & 0b00000001) > 0; }
        [[nodiscard]] constexpr bool bit1() const { return (value & 0b00000010) > 0; }
        [[nodiscard]] constexpr bool bit2() const { return (value & 0b00000100) > 0; }
        [[nodiscard]] constexpr bool bit3() const { return (value & 0b00001000) > 0; }
        [[nodiscard]] constexpr bool bit4() const { return (value & 0b00010000) > 0; }
        [[nodiscard]] constexpr bool bit5() const { return (value & 0b00100000) > 0; }
        [[nodiscard]] constexpr bool bit6() const { return (value & 0b01000000) > 0; }
        [[nodiscard]] constexpr bool bit7() const { return (value & 0b10000000) > 0; }

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

        //
        // Rotation and shifting
        //

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

        //
        // Overflow detection
        //

        [[nodiscard]] constexpr bool hasOverflow(Byte x) const { return value > (UINT8_MAX - x.value); }
        [[nodiscard]] constexpr bool hasOverflow(Byte a, Byte b) const { return value > (UINT8_MAX - a.value - b.value); }
        [[nodiscard]] constexpr bool hasHalfOverflow(Byte x) const { return (((value & 0xf) + (x.value & 0xf)) & 0x10) == 0x10; }
        [[nodiscard]] constexpr bool hasHalfOverflow(Byte a, Byte b) const { return (((value & 0xf) + (a.value & 0xf) + b.value) & 0x10) == 0x10; }

        [[nodiscard]] constexpr bool hasUnderflow(Byte x) const { return x.value > value; }
        [[nodiscard]] constexpr bool hasUnderflow(Byte a, Byte b) const { return std::int16_t(value) - a.value - b.value < 0; }
        [[nodiscard]] constexpr bool hasHalfUnderflow(Byte other) const { return (other.value & 0xf) > (value & 0xf); }
        [[nodiscard]] constexpr bool hasHalfUnderflow(Byte a, Byte b) const { return std::int16_t(value & 0xf) - (a.value & 0xf) - (b.value & 0xf) < 0; }

        //
        // Factory methods for creating from nibbles
        //

        static constexpr Byte fromNibbles(Byte n0, Byte n1)
        {
            return Byte((n0.value & 0x0F) | ((n1.value & 0x0F) << 4));
        }
    };

    class Word : public BitType<std::uint16_t>
    {
    public:
        using BitType::BitType;

        //
        // Byte extraction
        //

        [[nodiscard]] constexpr Byte byte0() const { return Byte((value >> 0) & 0xFF); }
        [[nodiscard]] constexpr Byte byte1() const { return Byte((value >> 8) & 0xFF); }

        //
        // Overflow detection
        //

        [[nodiscard]] constexpr bool hasOverflow(Word x) const { return value > (UINT16_MAX - x.value); }
        [[nodiscard]] constexpr bool hasHalfOverflow(Word x) const { return (((value & 0xfff) + (x.value & 0xfff)) & 0x1000) == 0x1000; }
        [[nodiscard]] constexpr bool hasUnderflow(Word x) const { return x.value > value; }
        [[nodiscard]] constexpr bool hasHalfUnderflow(Word x) const { return (x.value & 0xff) > (value & 0xff); }

        //
        // Factory methods
        //

        static constexpr Word fromBytes(Byte b0, Byte b1)
        {
            return Word(b0.value
                | (std::uint16_t(b1.value) << 8));
        }
    };

    class DWord : public BitType<std::uint32_t>
    {
    public:
        using BitType::BitType;

        //
        // Byte extraction
        //

        [[nodiscard]] constexpr Byte byte0() const { return Byte((value >> 0) & 0xFF); }
        [[nodiscard]] constexpr Byte byte1() const { return Byte((value >> 8) & 0xFF); }
        [[nodiscard]] constexpr Byte byte2() const { return Byte((value >> 16) & 0xFF); }
        [[nodiscard]] constexpr Byte byte3() const { return Byte((value >> 24) & 0xFF); }

        //
        // Word extraction
        //

        [[nodiscard]] constexpr Word word0() const { return Word((value >> 0) & 0xFFFF); }
        [[nodiscard]] constexpr Word word1() const { return Word((value >> 16) & 0xFFFF); }

        //
        // Factory methods
        //

        static constexpr DWord fromBytes(Byte b0, Byte b1, Byte b2, Byte b3)
        {
            return DWord(b0.value
                | (std::uint32_t(b1.value) << 8)
                | (std::uint32_t(b2.value) << 16)
                | (std::uint32_t(b3.value) << 24));
        }

        static constexpr DWord fromWords(Word w0, Word w1)
        {
            return DWord(w0.value
                | (std::uint32_t(w1.value) << 16));
        }
    };

    class QWord : public BitType<std::uint64_t> {
    public:
        using BitType::BitType;

        //
        // Byte extraction
        //

        [[nodiscard]] constexpr Byte byte0() const { return Byte((value >> 0 ) & 0xFF); }
        [[nodiscard]] constexpr Byte byte1() const { return Byte((value >> 8 ) & 0xFF); }
        [[nodiscard]] constexpr Byte byte2() const { return Byte((value >> 16) & 0xFF); }
        [[nodiscard]] constexpr Byte byte3() const { return Byte((value >> 24) & 0xFF); }
        [[nodiscard]] constexpr Byte byte4() const { return Byte((value >> 32) & 0xFF); }
        [[nodiscard]] constexpr Byte byte5() const { return Byte((value >> 40) & 0xFF); }
        [[nodiscard]] constexpr Byte byte6() const { return Byte((value >> 48) & 0xFF); }
        [[nodiscard]] constexpr Byte byte7() const { return Byte((value >> 56) & 0xFF); }

        //
        // Word extraction
        //

        [[nodiscard]] constexpr Word word0() const { return Word((value >> 0 ) & 0xFFFF); }
        [[nodiscard]] constexpr Word word1() const { return Word((value >> 16) & 0xFFFF); }
        [[nodiscard]] constexpr Word word2() const { return Word((value >> 32) & 0xFFFF); }
        [[nodiscard]] constexpr Word word3() const { return Word((value >> 48) & 0xFFFF); }

        //
        // DWord extraction
        //

        [[nodiscard]] constexpr DWord dword0() const { return DWord((value >> 0) & 0xFFFFFFFF); }
        [[nodiscard]] constexpr DWord dword1() const { return DWord((value >> 32) & 0xFFFFFFFF); }

        // Factory methods
        static constexpr QWord fromBytes(Byte b0, Byte b1, Byte b2, Byte b3, Byte b4, Byte b5, Byte b6, Byte b7)
        {
            return QWord(b0.value
                | (std::uint64_t(b1.value) << 8 )
                | (std::uint64_t(b2.value) << 16)
                | (std::uint64_t(b3.value) << 24)
                | (std::uint64_t(b4.value) << 32)
                | (std::uint64_t(b5.value) << 40)
                | (std::uint64_t(b6.value) << 48)
                | (std::uint64_t(b7.value) << 56));
        }

        static constexpr QWord fromWords(Word w0, Word w1, Word w2, Word w3)
        {
            return QWord(w0.value
                | (std::uint64_t(w1.value) << 16)
                | (std::uint64_t(w2.value) << 32)
                | (std::uint64_t(w3.value) << 48));
        }

        static constexpr QWord fromDWords(DWord d0, DWord d1)
        {
            return QWord(d0.value
                | (std::uint64_t(d1.value) << 32));
        }
    };



    //
    // Endianness utilities
    //

    enum class Endianness { Big, Little };

    inline Endianness getNativeEndianness()
    {
        union { std::uint32_t i; std::uint8_t c[4]; } u{0x1};
        return u.c[0] == 1 ? Endianness::Little : Endianness::Big;
    }

    //
    // Type aliases for compatibility
    //

    using byte_t  = Byte;
    using word_t  = Word;
    using dword_t = DWord;
    using qword_t = QWord;

    using b8  = Byte;
    using b16 = Word;
    using b32 = DWord;
    using b64 = QWord;

    [[nodiscard]] constexpr b8  operator ""_b8 (unsigned long long int i) noexcept { return b8 {static_cast<std::uint8_t>(i)}; }
    [[nodiscard]] constexpr b16 operator ""_b16(unsigned long long int i) noexcept { return b16{static_cast<std::uint16_t>(i)}; }
    [[nodiscard]] constexpr b32 operator ""_b32(unsigned long long int i) noexcept { return b32{static_cast<std::uint32_t>(i)}; }
    [[nodiscard]] constexpr b64 operator ""_b64(unsigned long long int i) noexcept { return b64{static_cast<std::uint64_t>(i)}; }

    //
    // STL Compat
    //

    template <typename IntType>
    inline std::ostream& operator<<(std::ostream& os, BitType<IntType> const& c)
    {
        return os << c.toBinString();
    }

    template <typename IntType>
    inline std::string to_string(BitType<IntType> const& c)
    {
        return c.toBinString();
    }

} // namespace ut
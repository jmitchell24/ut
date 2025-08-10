#pragma once

#include "int.hpp"

#include <vector>
#include <string>
#include <cstdio>
#include <cerrno>
#include <cstring>

//util::b8 static swap_table[256]
//{
//    0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,
//    0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1,
//    0x02, 0x12, 0x22, 0x32 ,0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xa2, 0xb2, 0xc2, 0xd2, 0xe2, 0xf2,
//    0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xa3, 0xb3, 0xc3, 0xd3, 0xe3, 0xf3,
//    0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x84, 0x94, 0xa4, 0xb4, 0xc4, 0xd4, 0xe4, 0xf4,
//    0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85, 0x95, 0xa5, 0xb5, 0xc5, 0xd5, 0xe5, 0xf5,
//    0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86, 0x96, 0xa6, 0xb6, 0xc6, 0xd6, 0xe6, 0xf6,
//    0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xa7, 0xb7, 0xc7, 0xd7, 0xe7, 0xf7,
//    0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xa8, 0xb8, 0xc8, 0xd8, 0xe8, 0xf8,
//    0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79, 0x89, 0x99, 0xa9, 0xb9, 0xc9, 0xd9, 0xe9, 0xf9,
//    0x0a, 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a, 0x8a, 0x9a, 0xaa, 0xba, 0xca, 0xda, 0xea, 0xfa,
//    0x0b, 0x1b, 0x2b, 0x3b, 0x4b, 0x5b, 0x6b, 0x7b, 0x8b, 0x9b, 0xab, 0xbb, 0xcb, 0xdb, 0xeb, 0xfb,
//    0x0c, 0x1c, 0x2c, 0x3c, 0x4c, 0x5c, 0x6c, 0x7c, 0x8c, 0x9c, 0xac, 0xbc, 0xcc, 0xdc, 0xec, 0xfc,
//    0x0d, 0x1d, 0x2d, 0x3d, 0x4d, 0x5d, 0x6d, 0x7d, 0x8d, 0x9d, 0xad, 0xbd, 0xcd, 0xdd, 0xed, 0xfd,
//    0x0e, 0x1e, 0x2e, 0x3e, 0x4e, 0x5e, 0x6e, 0x7e, 0x8e, 0x9e, 0xae, 0xbe, 0xce, 0xde, 0xee, 0xfe,
//    0x0f, 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f, 0x9f, 0xaf, 0xbf, 0xcf, 0xdf, 0xef, 0xff,
//};

//util::b8 util::bit::swap(util::b8 x)
//{
//    return swap_table[x];
//}


namespace ut
{
    using b8  = std::uint8_t;
    using b16 = std::uint16_t;
    using b32 = std::uint32_t;
    using b64 = std::uint64_t;

    using byte_t = b8;
    using word_t = b16;
    using dword_t = b32;
    using qword_t = b64;

    inline constexpr b8 byteNybl0(b8 x) { return (x >> (0 * 4)) & 0x0F; }
    inline constexpr b8 byteNybl1(b8 x) { return (x >> (1 * 4)) & 0x0F; }

    inline constexpr b8 wordByte0(b16 x) { return (x >> (0 * 8)) & 0xFF; }
    inline constexpr b8 wordByte1(b16 x) { return (x >> (1 * 8)) & 0xFF; }

    inline constexpr b8 dwordByte0(b32 x) { return (x >> (0 * 8)) & 0xFF; }
    inline constexpr b8 dwordByte1(b32 x) { return (x >> (1 * 8)) & 0xFF; }
    inline constexpr b8 dwordByte2(b32 x) { return (x >> (2 * 8)) & 0xFF; }
    inline constexpr b8 dwordByte3(b32 x) { return (x >> (3 * 8)) & 0xFF; }

    inline constexpr b8 qwordByte0(b64 x) { return (x >> (0 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte1(b64 x) { return (x >> (1 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte2(b64 x) { return (x >> (2 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte3(b64 x) { return (x >> (3 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte4(b64 x) { return (x >> (4 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte5(b64 x) { return (x >> (5 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte6(b64 x) { return (x >> (6 * 8)) & 0xFF; }
    inline constexpr b8 qwordByte7(b64 x) { return (x >> (7 * 8)) & 0xFF; }

    inline constexpr b16 dwordWord0(b32 x) { return (x >> (0 * 16)) & 0xFFFF; }
    inline constexpr b16 dwordWord1(b32 x) { return (x >> (1 * 16)) & 0xFFFF; }

    inline constexpr b16 qwordWord0(b64 x) { return (x >> (0 * 16)) & 0xFFFF; }
    inline constexpr b16 qwordWord1(b64 x) { return (x >> (1 * 16)) & 0xFFFF; }
    inline constexpr b16 qwordWord2(b64 x) { return (x >> (2 * 16)) & 0xFFFF; }
    inline constexpr b16 qwordWord3(b64 x) { return (x >> (3 * 16)) & 0xFFFF; }

    inline constexpr b32 qwordDword0(b64 x) { return (x >> (0 * 32)) & 0xFFFFFFFF; }
    inline constexpr b32 qwordDword1(b64 x) { return (x >> (1 * 32)) & 0xFFFFFFFF; }

    inline constexpr b8 byte(b8 n0, b8 n1) { return (n0 & 0x0F) | ((n1 & 0x0F) << 4); }
    inline constexpr b16 word(b8 b0, b8 b1) { return b0 | (b16(b1) << 8); }
    inline constexpr b32 dword(b8 b0, b8 b1, b8 b2, b8 b3) { return b0 | (b32(b1) << 8) | (b32(b2) << 16) | (b32(b3) << 24); }
    inline constexpr b64 qword(b8 b0, b8 b1, b8 b2, b8 b3, b8 b4, b8 b5, b8 b6, b8 b7) { return b0 | (b64(b1) << 8) | (b64(b2) << 16) | (b64(b3) << 24) | (b64(b4) << 32) | (b64(b5) << 40) | (b64(b6) << 48) | (b64(b7) << 56); }
    inline constexpr b32 dword(b16 w0, b16 w1) { return w0 | (b32(w1) << 16); }
    inline constexpr b64 qword(b16 w0, b16 w1, b16 w2, b16 w3) { return w0 | (b64(w1) << 16) | (b64(w2) << 32) | (b64(w3) << 48); }
    inline constexpr b64 qword(b32 d0, b32 d1) { return d0 | (b64(d1) << 32); }

    enum Endianness  { ENDIAN_BIG, ENDIAN_LITTLE };

    inline Endianness get_native_endianness()
    {
        union { b32 i; b8 c[sizeof(int)]; } u{0x1};
        return u.c[0] == 1 ? ENDIAN_LITTLE : ENDIAN_BIG;
    }

    inline constexpr b8 lo4(b8 x) { return x & 0xf;        }
    inline constexpr b8 hi4(b8 x) { return (x >> 4) & 0xf; }

    inline constexpr b8 lo8(b16 x) { return x & 0xff;        }
    inline constexpr b8 hi8(b16 x) { return (x >> 8) & 0xff; }

    inline constexpr b16 lo16(b32 x) { return x & 0xffff;         }
    inline constexpr b16 hi16(b32 x) { return (x >> 16) & 0xffff; }

    inline constexpr b16 lo32(b64 x) { return x & 0xffffffff;         }
    inline constexpr b16 hi32(b64 x) { return (x >> 32) & 0xffffffff; }

    // inline constexpr b16 make16(b8  lo, b8  hi) { return b16((b8 (lo))|((b16(b8 (hi)))<<8 )); }
    // inline constexpr b32 make32(b16 lo, b16 hi) { return b32((b16(lo))|((b32(b16(hi)))<<16)); }
    // inline constexpr b64 make64(b32 lo, b32 hi) { return b64((b32(lo))|((b64(b32(hi)))<<32)); }
    //
    // inline constexpr b32 make32(b8 b0, b8 b1, b8 b2, b8 b3)
    // { return b32(b8(b0)) | (b32(b8(b1)) << 8) | (b32(b8(b2)) << 16) | (b32(b8(b3)) << 24); }
    //
    // inline constexpr b64 make64(b8 b0, b8 b1, b8 b2, b8 b3, b8 b4, b8 b5, b8 b6, b8 b7)
    // {
    //     return b64(b8(b0)) | (b64(b8(b1)) << 8)  | (b64(b8(b2)) << 16) | (b64(b8(b3)) << 24) |
    //            (b64(b8(b4)) << 32) | (b64(b8(b5)) << 40) | (b64(b8(b6)) << 48) | (b64(b8(b7)) << 56);
    // }

    // inline constexpr b8  fold8 (b16 x) { return lo8(x) ^ hi8(x); }
    // inline constexpr b16 fold16(b32 x) { return lo16(x) ^ hi16(x); }
    // inline constexpr b32 fold32(b64 x) { return lo32(x) ^ hi32(x); }

namespace bit
{

    inline bool getX(b8 x, size_t n) { return ((x >> n) & 0x01); }
    inline bool get0(b8 x) { return (x & 0b00000001) > 0; }
    inline bool get1(b8 x) { return (x & 0b00000010) > 0; }
    inline bool get2(b8 x) { return (x & 0b00000100) > 0; }
    inline bool get3(b8 x) { return (x & 0b00001000) > 0; }
    inline bool get4(b8 x) { return (x & 0b00010000) > 0; }
    inline bool get5(b8 x) { return (x & 0b00100000) > 0; }
    inline bool get6(b8 x) { return (x & 0b01000000) > 0; }
    inline bool get7(b8 x) { return (x & 0b10000000) > 0; }

    inline b8 flipX(b8 x, b8 bit) { return x ^ (0xff << bit); }
    inline b8 flip0(b8 x) { return x ^ 0b00000001; }
    inline b8 flip1(b8 x) { return x ^ 0b00000010; }
    inline b8 flip2(b8 x) { return x ^ 0b00000100; }
    inline b8 flip3(b8 x) { return x ^ 0b00001000; }
    inline b8 flip4(b8 x) { return x ^ 0b00010000; }
    inline b8 flip5(b8 x) { return x ^ 0b00100000; }
    inline b8 flip6(b8 x) { return x ^ 0b01000000; }
    inline b8 flip7(b8 x) { return x ^ 0b10000000; }

    inline void setX(b8& byte, b8 bit, size_t n) { byte ^= (-bit ^ byte) & (1UL << n); }
    inline void setX_2(b8& byte, bool bit, size_t n) { byte = (byte & ~(b8(1) << n)) | (b8(bit) << n); }
    inline b8 setX(b8 byte, size_t n) { setX(byte, 1, n); return byte; }

    inline b8 set0(b8 x) { return x | 0b00000001; }
    inline b8 set1(b8 x) { return x | 0b00000010; }
    inline b8 set2(b8 x) { return x | 0b00000100; }
    inline b8 set3(b8 x) { return x | 0b00001000; }
    inline b8 set4(b8 x) { return x | 0b00010000; }
    inline b8 set5(b8 x) { return x | 0b00100000; }
    inline b8 set6(b8 x) { return x | 0b01000000; }
    inline b8 set7(b8 x) { return x | 0b10000000; }

    inline b8 resX(b8 byte, size_t n) { setX(byte, 0, n); return byte; }
    inline b8 res0(b8 x) { return x & 0b11111110; }
    inline b8 res1(b8 x) { return x & 0b11111101; }
    inline b8 res2(b8 x) { return x & 0b11111011; }
    inline b8 res3(b8 x) { return x & 0b11110111; }
    inline b8 res4(b8 x) { return x & 0b11101111; }
    inline b8 res5(b8 x) { return x & 0b11011111; }
    inline b8 res6(b8 x) { return x & 0b10111111; }
    inline b8 res7(b8 x) { return x & 0b01111111; }

    inline b8 rotateL(b8 x) { return b8(x<<1) | b8(x>>7); }
    inline b8 rotateR(b8 x) { return b8(x>>1) | b8(x<<7); }

    inline b8 shiftRL(b8 x) { return b8(x>>1); }
    inline b8 shiftLL(b8 x) { return b8(x<<1); }

    inline b8 shiftLA(b8 x) { return b8(x<<1); }
    inline b8 shiftRA(b8 x)
    {
        static_assert (i8(-1) >> 1 == i8(-1), "This compiler/platform does not implement arithmetic right shift.");
        union { b8 b; i8 i; } u{x};
        u.i >>= 1;
        return u.b;
    }

    inline void copyX(b8& write_byte, b8 read_byte, size_t n) { setX(write_byte, getX(read_byte, n), n); }

    inline b8 swap4(b8 x) { return b8(x << 4) | b8(x >> 4); }

    inline bool overflow (b8 a, b8 b) { return a > (UINT8_MAX - b); }
    inline bool overflow (b8 a, b8 b, b8 c) { return a > (UINT8_MAX - b - c); }
    inline bool overflow (b16 a, b16 b) { return a > (UINT16_MAX - b); }

    inline bool halfOverflow(b8 a, b8 b) { return (((a & 0xf) + (b & 0xf)) & 0x10) == 0x10; }             // Add the lower 4 bits then check if 5th bit gets set (overflow)
    inline bool halfOverflow(b8 a, b8 b, b8 c) { return (((a & 0xf) + (b & 0xf) + c) & 0x10) == 0x10; }
    inline bool halfOverflow(b16 a, b16 b) { return (((a & 0xfff) + (b & 0xfff)) & 0x1000) == 0x1000; }     // Add the lower 12 bits then check if 13th bit gets set (overflow)

    inline bool underflow(b8 a, b8 b) { return b > a; }
    inline bool underflow(b8 a, b8 b, b8 c) { return i16(a) - b - c < 0; }
    inline bool underflow(b16 a, b16 b) { return b > a; }

    inline bool halfUnderflow(b8 a, b8 b) { return lo4(b) > lo4(a); }
    inline bool halfUnderflow(b8 a, b8 b, b8 c) { return i16(lo4(a)) - lo4(b) - lo4(c) < 0; }
    inline bool halfUnderflow(b16 a, b16 b) { return lo8(b) > lo8(a); }

    inline std::string str(b8 x)
    {
        static char buf[9];
        for (size_t i = 0; i < 8; ++i)
        {
            buf[7-i] = getX(x, i) ? '1' : '0';
        }

        buf[8] = '\0';

        return {buf};
    }

    inline std::string strNyb(b8 x)
    {
        static char buf[10];
        for (size_t i = 0; i < 4; ++i)
        {
            buf[8-i] = getX(x, i) ? '1' : '0';
            buf[i] = getX(x, 7-i) ? '1' : '0';
        }

        buf[4] = ' ';
        buf[9] = '\0';

        return {buf};
    }
}
}



//
// Created by james on 31/05/22.
//

//
// https://rosettacode.org/wiki/Base64_encode_data#C.2B.2B
//

#pragma once

//
// ut
//

#include "ut/check.hpp"
#include "ut/string/view.hpp"

//
// std
//

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <cassert>

namespace ut
{
    struct base64
    {
        static constexpr char const* CHAR_SET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        static constexpr unsigned M1 = 63 << 18;
        static constexpr unsigned M2 = 63 << 12;
        static constexpr unsigned M3 = 63 << 6;

        // Helper function to get character index in CHAR_SET, returns 255 for invalid chars
        static constexpr std::uint8_t char_to_index(char c)
        {
            if (c >= 'A' && c <= 'Z') return c - 'A';
            if (c >= 'a' && c <= 'z') return c - 'a' + 26;
            if (c >= '0' && c <= '9') return c - '0' + 52;
            if (c == '+') return 62;
            if (c == '/') return 63;
            return 255; // Invalid character
        }

        static std::vector<std::uint8_t> decode(std::string const& str)
        {
            return decode(str.c_str(), str.c_str() + str.size());
        }

        static std::vector<std::uint8_t> decode(strparam sv)
        {
            return decode(sv.begin(), sv.end());
        }

        static std::vector<std::uint8_t> decode(char const* begin, char const* end = nullptr)
        {
            check_null(begin);

            if (end == nullptr) {
                end = begin + std::strlen(begin);
            }

            std::vector<std::uint8_t> result;
            auto len = end - begin;

            // Skip whitespace and calculate actual length
            auto actual_len = 0;
            for (auto i = 0; i < len; ++i) {
                if (begin[i] != ' ' && begin[i] != '\t' && begin[i] != '\n' && begin[i] != '\r') {
                    ++actual_len;
                }
            }

            if (actual_len == 0) return result;

            // Reserve space for efficiency
            result.reserve((actual_len * 3) / 4 + 3);

            auto d = 0u;
            auto char_count = 0;
            auto padding_count = 0;

            for (auto i = 0; i < len; ++i) {
                auto c = begin[i];

                // Skip whitespace
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                    continue;
                }

                if (c == '=') {
                    ++padding_count;
                    d <<= 6; // Shift for padding
                } else {
                    auto idx = char_to_index(c);
                    if (idx == 255) {
                        // Invalid character - could throw or skip
                        continue; // Skip invalid characters
                    }
                    d = (d << 6) | idx;
                }

                ++char_count;

                // Process every 4 characters
                if (char_count == 4) {
                    // Extract 3 bytes from the 24-bit value
                    result.push_back((d >> 16) & 0xFF);
                    if (padding_count < 2) {
                        result.push_back((d >> 8) & 0xFF);
                    }
                    if (padding_count < 1) {
                        result.push_back(d & 0xFF);
                    }

                    d = 0;
                    char_count = 0;
                    padding_count = 0;
                }
            }

            return result;
        }

        static std::string encode(std::string const& str)
        {
            return encode(str.c_str(), str.size());
        }

        template <typename T, size_t N>
        static std::string encode(std::array<T, N> const& arr)
        {
            return encode(arr.data(), sizeof(T) * arr.size());
        }

        template <typename T>
        static std::string encode(std::vector<T> const& vec)
        {
            return encode(vec.data(), sizeof(T) * vec.size());
        }

        static std::string encode(void const* data, size_t size)
        {
            check_null(data);

            std::string res;
            auto d = 0u;
            auto a = 0u;
            auto l = static_cast<unsigned>(size);
            auto v = reinterpret_cast<std::uint8_t const*>(data);

            while (l > 2)
            {
                d = v[a++] << 16;
                d |= v[a++] << 8;
                d |= v[a++];

                res.append( 1, CHAR_SET[ ( d & M1 ) >> 18 ] );
                res.append( 1, CHAR_SET[ ( d & M2 ) >> 12 ] );
                res.append( 1, CHAR_SET[ ( d & M3 ) >>  6 ] );
                res.append( 1, CHAR_SET[ d & 63 ] );
                l -= 3;
            }

            if (l == 2)
            {
                d = v[a++] << 16;
                d |= v[a] << 8;

                res.append( 1, CHAR_SET[ ( d & M1 ) >> 18 ] );
                res.append( 1, CHAR_SET[ ( d & M2 ) >> 12 ] );
                res.append( 1, CHAR_SET[ ( d & M3 ) >>  6 ] );
                res.append( 1, '=' );
            }
            else if (l == 1)
            {
                d = v[a++] << 16;
                res.append( 1, CHAR_SET[ ( d & M1 ) >> 18 ] );
                res.append( 1, CHAR_SET[ ( d & M2 ) >> 12 ] );
                res.append( "==", 2 );
            }
            return res;
        }
    };
}

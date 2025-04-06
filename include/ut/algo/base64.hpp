//
// Created by james on 31/05/22.
//

//
// https://rosettacode.org/wiki/Base64_encode_data#C.2B.2B
//

#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <cassert>

#include "ut/check.hpp"

namespace ut
{
    struct base64
    {
        static constexpr char const* CHAR_SET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        static constexpr unsigned M1 = 63 << 18;
        static constexpr unsigned M2 = 63 << 12;
        static constexpr unsigned M3 = 63 << 6;

        using string_type       = std::string;
        using view_type         = std::string_view;
        using vector_type       = std::vector<std::uint8_t>;
        using size_type         = std::size_t;

//        inline static vector_type decode(view_type const& str)   { return decode(str.data(), str.size()); }
//        inline static vector_type decode(string_type const& str)        { return decode(str.data(), str.size()); }
//        inline static vector_type decode(char const* str)               { return decode(str, strlen(str)); }

//        static vector_type decode(char const* first, size_type size)
//        {
//            assert(first != nullptr);
//            assert(size % 4 == 0); // "Error in size to the decode method"
//
//            auto it = first;
//            auto end = first+size;
//
//            vector_type sink;
//            while (it != end) {
//                auto b1 = *it++;
//                auto b2 = *it++;
//                auto b3 = *it++;                // might be first padding byte
//                auto b4 = *it++;                // might be first or second padding byte
//
//                auto i1 = findIndex(b1);
//                auto i2 = findIndex(b2);
//                auto acc = i1 << 2;             // six bits came from the first byte
//                acc |= i2 >> 4;                 // two bits came from the first byte
//
//                sink.push_back(acc);            // output the first byte
//
//                if (b3 != '=')
//                {
//                    auto i3 = findIndex(b3);
//
//                    acc = (i2 & 0xF) << 4;      // four bits came from the second byte
//                    acc |= i3 >> 2;             // four bits came from the second byte
//
//                    sink.push_back(acc);        // output the second byte
//
//                    if (b4 != '=')
//                    {
//                        auto i4 = findIndex(b4);
//
//                        acc = (i3 & 0x3) << 6;  // two bits came from the third byte
//                        acc |= i4;              // six bits came from the third byte
//
//                        sink.push_back(acc);    // output the third byte
//                    }
//                }
//            }
//            return sink;
//        }

        static string_type encode(void const* data, size_type size)
        {
            check_null(data);

            string_type res;
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

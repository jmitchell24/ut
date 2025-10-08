//
// Created by james on 24/09/25.
//

#pragma once

#include <ut/string.hpp>

//
// std
//

#include <string>
#include <vector>
#include <array>
#include <cstdint>

namespace ut
{
    /// \brief  Transforms arbitrary data into embed-able source code.
    ///         Inspired by ImGui, borrowing the same stb lib code.
    struct SrcRes
    {
        struct Src { std::string decl, impl; };

        std::string name;
        size_t wrap;

        SrcRes(std::string name, size_t wrap = 120)
            : name{std::move(name)}, wrap{wrap}
        { check(!this->name.empty()); }

        template <typename T>
        Src encode(std::vector<T> const& vec) const
        { return encode(vec.data(), vec.size() * sizeof(T)); }

        template <typename T, size_t N>
        Src encode(std::array<T, N> const& arr) const
        { return encode(arr.data(), arr.size() * sizeof(T)); }

        Src encode(strparam str) const
        { return encode(str.data(), str.size() * sizeof(char)); }

        Src encode(void const* data_in, size_t size) const;

        static size_t decode(cstrparam str, void* data_out, size_t size);

        template <typename T>
        static void decode(std::vector<std::uint8_t>& vec)
        {
            vec.resize(static_cast<size_t>(T::DECODED_SIZE));
            decode(static_cast<cstrview>(T::ENCODED_DATA), vec.data(), vec.size());
        }

        template <typename T>
        static void decode(std::array<std::uint8_t, static_cast<size_t>(T::DECODED_SIZE)>& arr)
        {
            unpack(static_cast<cstrview>(T::ENCODED_DATA), arr.data(), arr.size());
        }
    };


}
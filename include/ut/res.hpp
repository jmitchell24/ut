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
#include <filesystem>

namespace ut
{
    /// \brief  Transforms arbitrary data into embed-able source code.
    ///         Inspired by ImGui, borrowing the same stb lib code.
    struct SrcRes
    {
        struct Src { std::string decl, impl; };

        SrcRes(std::string name, std::string ns="", size_t wrap = 120);

        /// Namespace for code output ( optional )
        std::string const& ns() const
        { return m_namespace; }

        /// Struct name for code output ( must be a valid C++ name )
        std::string const& name() const
        { return m_name; }

        /// Maximum line length for encoded string literal.
        size_t wrap() const
        { return m_wrap; }

        template <typename T>
        Src encode(std::vector<T> const& vec) const
        { return encode(vec.data(), vec.size() * sizeof(T)); }

        template <typename T, size_t N>
        Src encode(std::array<T, N> const& arr) const
        { return encode(arr.data(), arr.size() * sizeof(T)); }

        Src encode(std::string const& str) const
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
            decode(static_cast<cstrview>(T::ENCODED_DATA), arr.data(), arr.size());
        }

        static Src encodeFile(std::filesystem::path const& path);
        static Src encodeDirectory(std::filesystem::path const& path, bool recursive=true);

    private:
        std::string m_name;
        std::string m_namespace;
        size_t      m_wrap;
    };




}
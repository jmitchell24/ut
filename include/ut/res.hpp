//
// Created by james on 24/09/25.
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
        size_t wrap = 120;

        template <typename T>
        std::string pack(std::vector<T> const& vec) const
        {
            return pack(vec.data(), vec.size() * sizeof(T));
        }

        template <typename T, size_t N>
        std::string pack(std::array<T, N> const& arr) const
        {
            return pack(arr.data(), arr.size() * sizeof(T));
        }

        std::string pack(std::string const& str) const
        {
            return pack(str.data(), str.size() * sizeof(char));
        }

        std::string pack(void const* data_in, size_t size) const;

        static size_t unpackBin(std::string const& str, std::vector<std::uint8_t>& data);
        static size_t unpackStr(std::string const& str, std::string& data);
    };
}
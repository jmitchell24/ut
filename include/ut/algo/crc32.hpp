//
// Created by james on 16/09/25.
//

//
// std
//

#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>
#include <string>

namespace ut
{
    struct crc32
    {
        static std::uint32_t compute(void const* data, size_t sz)
        {
            std::uint32_t seed = 0xffffffff;
            std::uint32_t poly = 0x04c11db7;

            auto bytes = static_cast<std::uint8_t const*>(data);

            for (size_t i = 0; i < sz; ++i)
            {
                seed ^= bytes[i];
                for (int j = 0; j < 8; ++j)
                {
                    if (seed & 1)
                        seed = (seed >> 1) ^ poly;
                    else
                        seed >>= 1;
                }
            }

            return seed ^ 0xffffffff;
        }

        template <typename T, size_t N>
        static std::uint32_t compute(std::array<T, N> const& arr)
        {
            return compute(arr.data(), sizeof(T) * arr.size());
        }

        template <typename T>
        static std::uint32_t compute(std::vector<T> const& vec)
        {
            return compute(vec.data(), sizeof(T) * vec.size());
        }

        /// \brief Computes the CRC32 Hash for a string (excludes trailing '\0')
        static std::uint32_t compute(std::string const& str)
        {
            return compute(str.data(), sizeof(char) * str.size());
        }
    };
}
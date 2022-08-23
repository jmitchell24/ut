#pragma once

// https://stackoverflow.com/a/4976611
//    template <std::size_t BlockSize=4096, typename Char=char, typename Traits = std::char_traits<Char>>
//    std::basic_string<Char,Traits> gulp(std::basic_istream<Char,Traits>& in)
//    {
//        std::basic_string<Char,Traits> ret;
//        Char block[BlockSize];
//        while (in.read(block, sizeof(block)))
//            ret.append(block, sizeof(block));
//        ret.append(block, in.gcount());
//        return ret;
//    }

#include <cstdio>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <memory>
#include <cstdint>

namespace ut::gulp
{

    std::size_t static constexpr GULP_MAX_BLOCKS = 1024;
    std::size_t static constexpr GULP_BLOCK_SIZE = 4096;

    template <std::size_t MaxBlocks=GULP_MAX_BLOCKS, std::size_t BlockSize=GULP_BLOCK_SIZE, typename T=std::uint8_t>
    std::vector<T> file_to_vector(FILE* in)
    {
        assert(in != nullptr);

        std::vector<T> ret;
        T block[BlockSize];

        for (std::size_t block_count = 0; block_count < MaxBlocks; ++block_count)
        {
            size_t cnt = fread(block, sizeof(T), BlockSize, in);

            if (ferror(in))
            {
                throw std::runtime_error{"gulp::file_to_vector(): error while reading file"};
            }

            ret.append(block, cnt);

            if (feof(in))
            {
                return ret;
            }
        }

        throw std::runtime_error{"gulp::file_to_vector(): maxiumum file read exceeded"};
    }

    template <std::size_t MaxBlocks=GULP_MAX_BLOCKS, std::size_t BlockSize=GULP_BLOCK_SIZE, typename Char=char, typename Traits = std::char_traits<Char>>
    std::basic_string<Char,Traits> stream_to_string(std::basic_istream<Char,Traits>& in)
    {
        std::basic_string<Char,Traits> ret;
        Char block[BlockSize];

        for (std::size_t block_count = 0; block_count < MaxBlocks; ++block_count)
        {
            if (in.read(block, sizeof(block)))
            {
                ret.append(block, sizeof(block));
            }
            else
            {
                ret.append(block, in.gcount());
                return ret;
            }
        }

        throw std::runtime_error{"gulp::stream_to_string(): maxiumum stream read exceeded"};
    }

    template <std::size_t MaxBlocks=GULP_MAX_BLOCKS, std::size_t BlockSize=GULP_BLOCK_SIZE, typename Char=char, typename Traits = std::char_traits<Char>>
    std::basic_string<Char,Traits> file_to_string(FILE* in)
    {
        assert(in != nullptr);

        std::basic_string<Char,Traits> ret;
        Char block[BlockSize];

        for (std::size_t block_count = 0; block_count < MaxBlocks; ++block_count)
        {
            size_t cnt = fread(block, sizeof(Char), BlockSize, in);

            if (ferror(in))
            {
                throw std::runtime_error{"gulp::file_to_string(): error while reading file"};
            }

            ret.append(block, cnt);

            if (feof(in))
            {
                return ret;
            }
        }

        throw std::runtime_error{"gulp::file_to_string(): maxiumum file read exceeded"};
    }

    template <std::size_t MaxBlocks=GULP_MAX_BLOCKS, std::size_t BlockSize=GULP_BLOCK_SIZE, typename Char=char, typename Traits = std::char_traits<Char>>
    std::basic_string<Char,Traits> exec_to_string(char const* cmd)
    {
        std::unique_ptr<FILE, decltype(&pclose)> pipe{popen(cmd, "r"), pclose};

        if (!pipe)
        {
            throw std::runtime_error{"gulp::exec_to_string(): popen() failed"};
        }

        return file_to_string<MaxBlocks, BlockSize, Char, Traits>(pipe.get());
    }
}

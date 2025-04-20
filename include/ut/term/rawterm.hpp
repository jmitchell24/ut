//
// Created by james on 20/04/25.
//

#pragma once

#include "ut/string.hpp"

namespace ut
{


    class RawTerm
    {
    public:

        inline bool enabled() const
        { return m_enabled; }

        void enable();
        void disable();


        void puts(char const* s, size_t n);
        void puts(strparam s);
        void putc(char c);
        char getc();
        void sync();

        std::pair<int,int> getWindowSize();

        static RawTerm& instance();

    private:
        bool m_enabled=false;
    };

    [[maybe_unused]]
    inline RawTerm& RAWTERM = RawTerm::instance();
}
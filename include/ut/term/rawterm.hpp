//
// Created by james on 20/04/25.
//

#pragma once

#include "ut/string.hpp"
#include "ut/check.hpp"

//
// std
//
#include <variant>

namespace ut
{
    enum Key
    {
        KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_DELETE,
        KEY_BACKSPACE, KEY_HOME, KEY_END, KEY_NEWLINE, KEY_CARRIAGE_RETURN, KEY_TAB, KEY_PAGE_UP, KEY_PAGE_DOWN,
        KEY_EOF
    };

    struct Coord { size_t row=0,col=0; };

    struct RawTermChar
    {
        using var_type = std::variant<char, Key>;

        inline bool isChar() const { return data.index()==0; }
        inline bool isKey() const { return data.index()==1; }

        inline char asChar() const { check(isChar()); return std::get<0>(data); }
        inline Key asKey() const { check(isKey()); return std::get<1>(data); }

        inline bool keyIf(Key k) const { check(isKey()); return asKey() == k; }
        inline bool charIf(char c) const { check(isChar()); return asChar() == c; }

        var_type data;
    };

    class RawTerm
    {
    public:
        inline bool enabled() const
        { return m_enabled; }

        void enable();
        void disable();


        RawTerm& puts(char const* s, size_t n);
        RawTerm& puts(char const* s);
        RawTerm& putf(char const* fmt, ...);
        RawTerm& puts(strparam s);
        RawTerm& putc(char c);

        inline RawTerm& operator<< (char const* s) { return puts(s); }
        inline RawTerm& operator<< (strparam s) { return puts(s); }
        inline RawTerm& operator<< (char c) { return putc(c); }

        RawTermChar getc();
        void sync();

        std::pair<int,int> getWindowSize();

        static RawTerm& instance();

    private:
        bool m_enabled=false;

    };

    [[maybe_unused]]
    inline RawTerm& RAWTERM = RawTerm::instance();
}
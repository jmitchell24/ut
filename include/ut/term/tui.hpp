//
// Created by james on 20/04/25.
//

#pragma once

#include "escapes.hpp"
#include "ut/string.hpp"
namespace ut
{
namespace table
{
    bool begin();

    void end();
    void title(strparam title);
    void header(strparam text, int x);
    void style(char const* style);
    void cell(strparam text);

    void setCur(int x, int y);
    void nextColumn();
    void nextRow();

    inline void label(strparam text)
    { style(TERM_FG_BRIGHT_CYAN); cell(text); style(TERM_RESET); }

    inline void text(strparam text)
    { style(TERM_FG_BRIGHT_YELLOW); cell(text); style(TERM_RESET); }

    inline void integer(int i)
    { style(TERM_FG_BRIGHT_MAGENTA); cell(std::to_string(i)); style(TERM_RESET); }

    inline void decimal(double d)
    { style(TERM_FG_BRIGHT_MAGENTA); cell(std::to_string(d)); style(TERM_RESET); }

    inline void boolean(bool b, strparam t="true", strparam f="false")
    { style(b ? TERM_FG_BRIGHT_GREEN : TERM_FG_BRIGHT_RED); cell(b ? t : f); style(TERM_RESET); }

}

}

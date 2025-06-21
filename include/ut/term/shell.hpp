//
// Created by james on 20/04/25.
//

#pragma once

//
// ut
//
#include <ut/term/escapes.hpp>
#include <ut/string.hpp>

//
// std
//
#include <functional>

#define ut_shell ( ut::Shell::instance() )

#define DEFAULT_PROMPT \
    TERM_FG_BRIGHT_BLUE     "~" \
    TERM_FG_BRIGHT_RED      "~" \
    TERM_FG_BRIGHT_YELLOW   "~" \
    TERM_FG_BRIGHT_GREEN    "~" \
    TERM_FG_BRIGHT_CYAN     "> "

namespace ut
{
    class Shell
    {
    public:
        using hint_type = std::function<void(strparam)>;
        hint_type hint = {};

        std::string prompt = DEFAULT_PROMPT;

        Shell();

        inline std::string const& line() const
        { return m_line; }

        bool getLine();

        std::string runInteractiveHint(hint_type hint = {}, cstrparam prompt = DEFAULT_PROMPT);


        static Shell& instance();

    private:
        std::string m_line;

        void putHint(strparam s);
        void putLineRefresh(strparam buffer, size_t buffer_loc) const;
    };
}

#undef DEFAULT_PROMPT
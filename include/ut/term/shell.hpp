//
// Created by james on 20/04/25.
//

#pragma once

//
// ut
//
#include <ut/time.hpp>
#include <ut/term/escapes.hpp>
#include <ut/string.hpp>

//
// std
//
#include <functional>

#define ut_shell ( ut::Shell::instance() )

#define UT_TERM_DEFAULT_PROMPT \
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
        using prompt_type = std::function<std::string()>;

        hint_type hint = {};
        prompt_type prompt = []
        {
            return TERM_FG_BRIGHT_GREEN +
                local_datetime::now().str(
                    TERM_FG_BRIGHT_BLUE   "%I" TERM_RESET ":"
                    TERM_FG_BRIGHT_RED    "%M" TERM_RESET ":"
                    TERM_FG_BRIGHT_YELLOW "%S" TERM_RESET " "
                    TERM_FG_BRIGHT_GREEN  "%p" TERM_RESET
                    ) + " " UT_TERM_DEFAULT_PROMPT;
        };

        Shell();

        inline void setPromptString(strparam s)
        { this->prompt = [p=s.str()] { return p; }; }

        inline std::string const& line() const
        { return m_line; }

        bool getLine();

        std::string runInteractiveHint(hint_type hint = {}, cstrparam prompt = UT_TERM_DEFAULT_PROMPT);


        static Shell& instance();

    private:
        std::string m_line;

        void putHint(strparam s);
    };
}
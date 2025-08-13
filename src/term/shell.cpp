//
// Created by james on 20/04/25.
//

//
// ut
//
#include "ut/term/escapes.hpp"
#include "ut/term/rawterm.hpp"
#include "ut/term/shell.hpp"
using namespace ut;

//
// std
//
#include <string>
using namespace std;

//
// Shell -> Implementation
//

Shell::Shell()
{}

void Shell::putHint(strparam s) const
{
    if (hint != nullptr)
    {
        ut_term <<
            TERM_CURSOR_SAVE
            TERM_CURSOR_COLUMN(1)
            TERM_CURSOR_DOWN(1)
            TERM_CLEAR_LINE;

        hint(s);

        ut_term <<
            TERM_CLEAR_SCREEN_TO_END
            TERM_RESET
            TERM_CURSOR_RESTORE;
    }
}

#define REPOSITION_CURSOR ( ut_term << esc::termCursorColumn(prompt_loc.first + buffer_loc) )

bool Shell::getLine()
{
    check(!ut_term.enabled());

    ut_term.enable();

    // reset cursor, clear line
    ut_term << TERM_CURSOR_COLUMN(1) TERM_CLEAR_LINE;

    // reprint prompt (move cursor to beginning of buffer)
    ut_term << TERM_RESET << prompt() << TERM_RESET;

    size_t prompt_loc   = ut_term.getCursorPosition().first;
    size_t buffer_loc   = 0;
    string buffer       = "";

    for (;;)
    {
        auto c = ut_term.getc();

        if (c.isChar())
        {
            buffer.insert(buffer.begin()+buffer_loc, c.asChar());
            ++buffer_loc;

            ut_term
                << esc::termCursorColumn(prompt_loc)
                << buffer
                << esc::termCursorColumn(prompt_loc + buffer_loc);

            putHint(buffer);
        }

        if (c.isKey())
        {
            switch (c.asKey())
            {
            case KEY_BACKSPACE:
                if (!buffer.empty() && buffer_loc > 0)
                {
                    if (buffer_loc < buffer.size())
                        buffer.erase(buffer.begin() + buffer_loc - 1);
                    else
                        buffer.pop_back();
                    --buffer_loc;

                    ut_term
                        << esc::termCursorColumn(prompt_loc)
                        << TERM_CLEAR_LINE_TO_END
                        << buffer
                        << esc::termCursorColumn(prompt_loc + buffer_loc);

                    putHint(buffer);
                }
                break;

            case KEY_DELETE:
                if (!buffer.empty())
                {
                    if (buffer_loc < buffer.size())
                    {
                        buffer.erase(buffer.begin() + buffer_loc);

                        ut_term
                            << esc::termCursorColumn(prompt_loc)
                            << TERM_CLEAR_LINE_TO_END
                            << buffer
                            << esc::termCursorColumn(prompt_loc + buffer_loc);

                        putHint(buffer);
                    }
                }
                break;

            case KEY_HOME:
                buffer_loc = 0;
                ut_term << esc::termCursorColumn(prompt_loc + buffer_loc);
                break;

            case KEY_END:
                buffer_loc = buffer.size();
                ut_term << esc::termCursorColumn(prompt_loc + buffer_loc);
                break;

            case KEY_LEFT:
                if (buffer_loc > 0)
                {
                    --buffer_loc;
                    ut_term << esc::termCursorColumn(prompt_loc + buffer_loc);
                }
                break;

            case KEY_RIGHT:
                if (buffer_loc < buffer.size())
                {
                    ++buffer_loc;
                    ut_term << esc::termCursorColumn(prompt_loc + buffer_loc);
                }
                break;


            case KEY_EOF:
            case KEY_NEWLINE:
            case KEY_CARRIAGE_RETURN:
                m_line = buffer;
                ut_term.disable();
                printf("\n");
                fflush(stdout);
                return true;

            case KEY_CTRL_C:
                m_line = buffer;
                ut_term.disable();
                printf("\n");
                fflush(stdout);
                return false;
            default:break;
            }

        }
    }

    ut_term.disable();
    printf("\n");
    return false;
}

string Shell::runInteractiveHint(hint_type hint, cstrparam prompt)
{
    setPromptString(prompt);
    this->hint = hint;

    getLine();
    return m_line;
}

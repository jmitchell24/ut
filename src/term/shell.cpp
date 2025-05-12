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

#define CTRL_KEY(k) ((k) & 0x1f)

Shell::Shell()
{}

Shell& Shell::instance()
{
    static Shell x;
    return x;
}

void Shell::putHint(strparam s)
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

#define DO_CURSOR_RESET \
    TERM_CURSOR_COLUMN(1) TERM_CLEAR_LINE

#define DO_CURSOR_REFRESH \
    esc::termCursorColumn(1 + prompt.size() + buffer_loc)

#define DO_BUFFER_REFRESH \
    DO_CURSOR_RESET << prompt << buffer << DO_CURSOR_REFRESH

bool Shell::getLine(string& line)
{
    ut_term.enable();

    string prompt       = "$ ";
    string buffer       = "";
    size_t buffer_loc   = 0;

    ut_term << DO_BUFFER_REFRESH;

    for (;;)
    {
        auto c = ut_term.getc();

        if (c.isChar())
        {
            buffer.insert(buffer.begin()+buffer_loc, c.asChar());
            ++buffer_loc;


            ut_term << DO_BUFFER_REFRESH;
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

                    ut_term << DO_BUFFER_REFRESH;
                    putHint(buffer);
                }
                break;

            case KEY_DELETE:
                if (!buffer.empty())
                {
                    if (buffer_loc < buffer.size())
                    {
                        buffer.erase(buffer.begin() + buffer_loc);
                        ut_term << DO_BUFFER_REFRESH;
                        putHint(buffer);
                    }
                }
                break;

            case KEY_HOME:
                buffer_loc = 0;
                ut_term << DO_CURSOR_REFRESH;
                break;

            case KEY_END:
                buffer_loc = buffer.size();
                ut_term << DO_CURSOR_REFRESH;
                break;

            case KEY_LEFT:
                if (buffer_loc > 0)
                {
                    --buffer_loc;
                    ut_term << DO_CURSOR_REFRESH;
                }
                break;

            case KEY_RIGHT:
                if (buffer_loc < buffer.size())
                {
                    ++buffer_loc;
                    ut_term << DO_CURSOR_REFRESH;
                }
                break;


            case KEY_EOF:
            case KEY_NEWLINE:
            case KEY_CARRIAGE_RETURN:
                line = buffer;
                ut_term << TERM_CURSOR_NEXT_LINE(1);
                ut_term.disable();
                return true;

            case KEY_CTRL_C:
                line = buffer;
                ut_term << TERM_CURSOR_NEXT_LINE(1);
                ut_term.disable();
                return false;
            default:break;
            }

        }
    }

    ut_term.disable();
    return false;
}
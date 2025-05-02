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
using namespace std;

//
// Shell -> Implementation
//

#define CTRL_KEY(k) ((k) & 0x1f)

static void defaultPrompt()
{
    RAWTERM << "$ ";
}

Shell::Shell()
    : prompt{nullptr}
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
        RAWTERM <<
            TERM_CURSOR_SAVE
            TERM_CURSOR_COLUMN(1)
            TERM_CURSOR_DOWN(1)
            TERM_CLEAR_LINE
            TERM_DIM;

        RAWTERM << hint(s);

        RAWTERM <<
            TERM_RESET
            TERM_CURSOR_RESTORE;
    }
}


bool Shell::getLine(string& line)
{
    RAWTERM.enable();

    RAWTERM <<
        TERM_CURSOR_COLUMN(1)
        TERM_CLEAR_LINE;

    auto _prompt = prompt == nullptr ? defaultPrompt : prompt;
    _prompt();

    string buffer;
    size_t buffer_loc=0;

    for (;;)
    {
        auto c = RAWTERM.getc();

        if (c.isChar())
        {
            buffer.insert(buffer.begin()+buffer_loc, c.asChar());
            ++buffer_loc;

            RAWTERM <<
                TERM_CURSOR_SAVE
                TERM_CURSOR_COLUMN(1)
                TERM_CLEAR_LINE;

            _prompt();
            RAWTERM << buffer;

            RAWTERM <<
                TERM_CURSOR_RESTORE;

            putHint(buffer);
        }

        if (c.isKey())
        {
            switch (c.asKey())
            {
            case KEY_BACKSPACE:
                if (!buffer.empty())
                {
                    buffer.pop_back();

                    RAWTERM <<
                        TERM_CURSOR_LEFT(1)
                        " "
                        TERM_CURSOR_LEFT(1);

                    putHint(buffer);
                }
                break;

            case KEY_LEFT:
                if (buffer_loc > 0)
                {
                    RAWTERM <<
                        TERM_CURSOR_LEFT(1);
                    --buffer_loc;
                }
                break;

            case KEY_RIGHT:
                if (buffer_loc < buffer.size())
                {
                    RAWTERM <<
                        TERM_CURSOR_RIGHT(1);
                    ++buffer_loc;
                }
                break;

            case KEY_EOF:
            case KEY_NEWLINE:
            case KEY_CARRIAGE_RETURN:
                line = buffer;
                RAWTERM << TERM_CURSOR_NEXT_LINE(1);
                RAWTERM.disable();
                return true;
            default:break;
            }

        }
    }

    RAWTERM.disable();
    return false;
}
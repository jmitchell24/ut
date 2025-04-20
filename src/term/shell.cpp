//
// Created by james on 20/04/25.
//

#include "ut/term/shell.hpp"


//
// Shell -> Implementation
//

#if 0

#define CTRL_KEY(k) ((k) & 0x1f)

static void defaultPrompt()
{
    rawputs("$ "_sv);
}

Shell::Shell()
    : prompt{nullptr}
{}

Shell& Shell::instance()
{
    static Shell x;
    return x;
}

bool Shell::getLine(string& line)
{
    enableRawMode();

    (prompt == nullptr ? defaultPrompt : prompt)();

    string buffer;

    for (;;)
    {
        char c = rawgetc();

        switch (c)
        {
        case 127: // backspace
            if (!buffer.empty())
            {
                buffer.pop_back();
                rawputs(TERM_CURSOR_LEFT(1));
                rawputc(' ');
                rawputs(TERM_CURSOR_LEFT(1));
            }
            break;


        case CTRL_KEY('c'): // ctrl+c
            break;

        case '\0':
        case '\r':
        case '\n': // newline / eof
            line = buffer;
            rawputs(TERM_CURSOR_NEXT_LINE(1));

            disableRawMode();

            return true;

        default: // everything else...
            buffer += c;
            rawputc(c);
            break;

        }
    }

    disableRawMode();
    return false;
}

#endif
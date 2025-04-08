//
// Created by james on 04/04/25.
//

#include "ut/term.hpp"
#include "ut/check.hpp"
using namespace ut;
using namespace ut::term;

//
// std
//
using namespace std;



#ifdef __linux__

//
// unix
//
#include <unistd.h>
#include <termio.h>

//
// term.hpp -> implementation
//

static bool g_raw_mode_enabled = false;

void die(char const* s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

void ut::term::disableRawMode()
{
    termios term{};

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        die("tcgetattr");

    term.c_lflag |= (ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        die("tcsetattr");

    g_raw_mode_enabled=false;
}

void ut::term::enableRawMode()
{
    termios term{};

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        die("tcgetattr");

    term.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        die("tcsetattr");

    atexit(disableRawMode);
    g_raw_mode_enabled=true;
}

void ut::term::rawputs(char const* buf, size_t sz)
{
    check_msg(g_raw_mode_enabled, "raw mode should have been enabled");
    if (write(STDOUT_FILENO, buf, sz) < 0)
        die("write");
}

void ut::term::rawputs(strparam s)
{
    rawputs(s.data(), s.size());
}

void ut::term::rawputc(char c)
{
    rawputs(&c, 1);
}

char ut::term::rawgetc()
{
    check_msg(g_raw_mode_enabled, "raw mode should have been enabled");

    char c;
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        die("read");
    return c;
}

void ut::term::rawsync()
{
    check_msg(g_raw_mode_enabled, "raw mode should have been enabled");

    fsync(STDOUT_FILENO);
}

#else
static void die()
{
    fprintf(stderr, "ut::term unsupported on this platform, exiting...\n");
    exit(EXIT_FAILURE);
}

void ut::term::disableRawMode() {die();}
void ut::term::enableRawMode() {die();}
void ut::term::rawputs(char const* buf, size_t sz) {die();}
void ut::term::rawputs(strparam s) {die();}
void ut::term::rawputc(char c) {die();}
char ut::term::rawgetc() {die(); return 0; }
void ut::term::rawsync() {die();}
#endif

#define CTRL_KEY(k) ((k) & 0x1f)

//
// Shell -> Implementation
//

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


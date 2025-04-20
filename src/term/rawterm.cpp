//
// Created by james on 20/04/25.
//

#include "ut/check.hpp"
#include "ut/term/rawterm.hpp"
using namespace ut;

//
// std
//
#include <string>
using namespace std;

#ifdef __linux__

//
// unix
//
#include <unistd.h>
#include <termio.h>

//
// helper functions
//

static void rawErrorExit(char const* s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

static void rawDisable()
{
    termios term{};

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        rawErrorExit("tcgetattr");

    term.c_lflag |= (ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        rawErrorExit("tcsetattr");
}

static void rawEnable()
{
    termios term{};

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        rawErrorExit("tcgetattr");

    term.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        rawErrorExit("tcsetattr");

    atexit(rawDisable);
}

//
// RawTerm -> implementation
//

RawTerm& RawTerm::instance()
{
    static RawTerm x;
    return x;
}


void RawTerm::disable()
{
    if (m_enabled)
    {
        rawDisable();
        m_enabled = false;
    }
}

void RawTerm::enable()
{
    if (!m_enabled)
    {
        rawEnable();
        m_enabled = true;
    }
}


void RawTerm::puts(char const* buf, size_t sz)
{
    check_msg(m_enabled, "raw mode should have been enabled");
    if (write(STDOUT_FILENO, buf, sz) < 0)
        rawErrorExit("write");
}

void RawTerm::puts(strparam s)
{
    puts(s.data(), s.size());
}

void RawTerm::putc(char c)
{
    puts(&c, 1);
}

char RawTerm::getc()
{
    check_msg(m_enabled, "raw mode should have been enabled");

    char c;
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        rawErrorExit("read");
    return c;
}

void RawTerm::sync()
{
    check_msg(m_enabled, "raw mode should have been enabled");

    fsync(STDOUT_FILENO);
}

pair<int, int> RawTerm::getWindowSize()
{
    //check_msg(g_raw_mode_enabled, "raw mode should be enabled") ;

    winsize ws{};

    fsync(STDOUT_FILENO);
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0)
        rawErrorExit("ioctl");

    // if (ws.ws_col == 0 || ws.ws_row)
    // {
    //     rawputs("zero terminal size reported"_sv);
    //     exit(EXIT_FAILURE);
    // }

    return { ws.ws_col, ws.ws_row };
}

#endif
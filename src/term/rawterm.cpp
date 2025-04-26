//
// Created by james on 20/04/25.
//

#include "ut/check.hpp"
#include "ut/term/rawterm.hpp"

#include "ut/time.hpp"
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

static termios g_previous_term;

static void rawDisable()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_previous_term);
}

static void rawEnable()
{
    termios term{};

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        rawErrorExit("tcgetattr");

    g_previous_term = term;

    term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term.c_oflag &= ~(OPOST);
    term.c_cflag |= (CS8);
    term.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
        rawErrorExit("tcsetattr");
    atexit(rawDisable);
}

static char rawGetChar()
{
    char c;
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        rawErrorExit("read");
    return c;
}

static pair<int, int> rawGetWinSize()
{
    //check_msg(g_raw_mode_enabled, "raw mode should be enabled") ;
    winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) < 0)
        rawErrorExit("ioctl");
    return {ws.ws_col, ws.ws_row};
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


RawTerm& RawTerm::puts(char const* buf, size_t sz)
{
    check_msg(m_enabled, "raw mode should have been enabled");
    if (write(STDOUT_FILENO, buf, sz) < 0)
        rawErrorExit("write");
    return *this;
}

RawTerm& RawTerm::puts(char const* buf)
{
    check_msg(m_enabled, "raw mode should have been enabled");
    if (write(STDOUT_FILENO, buf, strlen(buf)) < 0)
        rawErrorExit("write");
    return *this;
}

RawTerm& RawTerm::puts(strparam s)
{
    check_msg(m_enabled, "raw mode should have been enabled");

    return puts(s.data(), s.size());
}

RawTerm& RawTerm::putf(char const* fmt, ...)
{
    string buf;
    buf.resize(1000);

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf.data(), buf.size(), fmt, args);
    va_end(args);

    return puts(buf.data(), buf.size());
}

RawTerm& RawTerm::putc(char c)
{
    check_msg(m_enabled, "raw mode should have been enabled");

    puts(&c, 1);
    return *this;
}

struct EscScanner
{
    string buf;

    bool nextIf(char c)
    {
        if (buf.back() == c)
        {
            buf += rawGetChar();

            return true;
        }
        return false;
    }

    char next()
    {
        char c = rawGetChar();
        buf += c;
        return c;
    }

    char peek() const
    {
        check(!buf.empty());
        return buf.back();
    }

    bool peekIf(char c1) const
    {
        size_t sz = buf.size();
        check(sz >= 1);
        return buf[sz-1] == c1;
    }

    bool peekIf(char c1, char c2) const
    {
        size_t sz = buf.size();
        check(sz >= 2);
        return
            buf[sz-1] == c2 &&
            buf[sz-2] == c1;
    }

    bool peekIf(char c1, char c2, char c3) const
    {
        size_t sz = buf.size();
        check(sz >= 3);
        return
            buf[sz-1] == c3 &&
            buf[sz-2] == c2 &&
            buf[sz-3] == c1;
    }
};

RawTermChar RawTerm::getc()
{
    check_msg(m_enabled, "raw mode should have been enabled");

    EscScanner scan;

    // read a char from terminal
    scan.next();

    // if char is beginning of escape sequence
    if (scan.nextIf('\x1b'))
    {
        // if char is beginning of escape sequence
        if (scan.nextIf('['))
        {
            if (scan.peekIf('A')) return { KEY_UP };
            if (scan.peekIf('B')) return { KEY_DOWN };
            if (scan.peekIf('C')) return { KEY_RIGHT };
            if (scan.peekIf('D')) return { KEY_LEFT };
            if (scan.peekIf('H')) return { KEY_HOME };
            if (scan.peekIf('F')) return { KEY_END };

            scan.next();

            if (scan.peekIf('3', '~')) return { KEY_DELETE };
            if (scan.peekIf('5', '~')) return { KEY_PAGE_UP };
            if (scan.peekIf('6', '~')) return { KEY_PAGE_DOWN };
        }

        // unknown / invalid escape sequence
        return {'\x1b'};
    }

    if (scan.peekIf(127)) return { KEY_BACKSPACE };
    if (scan.peekIf('\t')) return { KEY_TAB };
    if (scan.peekIf('\n')) return { KEY_NEWLINE };
    if (scan.peekIf('\r')) return { KEY_CARRIAGE_RETURN };


    // not an escape sequence or special key
    return {scan.peek()};
}

void RawTerm::sync()
{
    check_msg(m_enabled, "raw mode should have been enabled");

    fsync(STDOUT_FILENO);
}

pair<int, int> RawTerm::getWindowSize()
{
    check_msg(m_enabled, "raw mode should be enabled") ;

    for (size_t i = 0; i < 5; ++i)
    {
        auto sz = rawGetWinSize();
        if (sz.first > 0 && sz.second > 0)
            return sz;
        timer::sleep(200_milliseconds);
    }

    rawErrorExit("zero window size");
    return{0,0};
}

#endif
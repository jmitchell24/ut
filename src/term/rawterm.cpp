//
// Created by james on 20/04/25.
//

#include "ut/check.hpp"
#include "ut/term/rawterm.hpp"

#include "ut/time.hpp"
#include "ut/term/escapes.hpp"
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

#define check_raw_mode { check_msg(m_enabled, "raw mode should be enabled"); }

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

static void rawPuts(char const* s, size_t n)
{
    if (write(STDOUT_FILENO, s, n) < 0)
        rawErrorExit("write");
}

// static void rawPutc(char c)
// {
//     rawPuts(&c, 1);
// }

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
    check_raw_mode
    rawPuts(buf, sz);
    return *this;
}

RawTerm& RawTerm::puts(char const* buf)
{
    check_raw_mode
    rawPuts(buf, strlen(buf));
    return *this;
}

RawTerm& RawTerm::puts(strparam s)
{
    check_raw_mode
    return puts(s.data(), s.size());
}

RawTerm& RawTerm::puts(std::string const& s)
{
    check_raw_mode
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
    check_raw_mode
    puts(&c, 1);
    return *this;
}

struct EscScanner
{
    string buf;

    size_t size() const
    { return buf.size(); }

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
    check_raw_mode

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

    if (scan.peekIf('a' & 0x1f)) return { KEY_CTRL_A };
    if (scan.peekIf('b' & 0x1f)) return { KEY_CTRL_B };
    if (scan.peekIf('c' & 0x1f)) return { KEY_CTRL_C };
    if (scan.peekIf('d' & 0x1f)) return { KEY_CTRL_D };
    if (scan.peekIf('e' & 0x1f)) return { KEY_CTRL_E };
    if (scan.peekIf('f' & 0x1f)) return { KEY_CTRL_F };
    if (scan.peekIf('g' & 0x1f)) return { KEY_CTRL_G };
    if (scan.peekIf('h' & 0x1f)) return { KEY_CTRL_H };
    if (scan.peekIf('i' & 0x1f)) return { KEY_CTRL_I };
    if (scan.peekIf('j' & 0x1f)) return { KEY_CTRL_J };
    if (scan.peekIf('k' & 0x1f)) return { KEY_CTRL_K };
    if (scan.peekIf('l' & 0x1f)) return { KEY_CTRL_L };
    if (scan.peekIf('m' & 0x1f)) return { KEY_CTRL_M };
    if (scan.peekIf('n' & 0x1f)) return { KEY_CTRL_N };
    if (scan.peekIf('o' & 0x1f)) return { KEY_CTRL_O };
    if (scan.peekIf('p' & 0x1f)) return { KEY_CTRL_P };
    if (scan.peekIf('q' & 0x1f)) return { KEY_CTRL_Q };
    if (scan.peekIf('r' & 0x1f)) return { KEY_CTRL_R };
    if (scan.peekIf('s' & 0x1f)) return { KEY_CTRL_S };
    if (scan.peekIf('t' & 0x1f)) return { KEY_CTRL_T };
    if (scan.peekIf('u' & 0x1f)) return { KEY_CTRL_U };
    if (scan.peekIf('v' & 0x1f)) return { KEY_CTRL_V };
    if (scan.peekIf('w' & 0x1f)) return { KEY_CTRL_W };
    if (scan.peekIf('x' & 0x1f)) return { KEY_CTRL_X };
    if (scan.peekIf('y' & 0x1f)) return { KEY_CTRL_Y };
    if (scan.peekIf('z' & 0x1f)) return { KEY_CTRL_Z };

    // not an escape sequence or special key
    return {scan.peek()};
}

void RawTerm::sync()
{
    check_raw_mode

    fsync(STDOUT_FILENO);
}

pair<int, int> RawTerm::getWindowSize()
{
    check_raw_mode

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

std::pair<int, int> RawTerm::getCursorPosition() const
{
    check_raw_mode

    auto req = CSI "6n"_sv;
    rawPuts(req.data(), req.size());

    EscScanner scan;

    scan.next();
    while (scan.size() < 100)
    {
        if (scan.peekIf('R'))
        {
            int row, col;
            sscanf(scan.buf.c_str()+2, "%d;%d", &row,&col);
            return {col, row};
        }
        scan.next();
    }

    return {};
}


#endif
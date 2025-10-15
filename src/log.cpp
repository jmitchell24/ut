//
// Created by james on 26/09/25.
//

#include "ut/log.hpp"
#include "ut/term/escapes.hpp"
#include "ut/random.hpp"
using namespace ut;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

//
// std
//

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;



//
// helpers
//

static std::string getEscFgBg(color const& fg, color const& bg)
{
    return fg.toFgEscCode() + bg.toBgEscCode();
}

static std::string getEscBg(color const& bg)
{
    return getEscFgBg(
        bg.toOKLCH().l < .5 ? colors::white : colors::black,
        bg);
}

log::Printer::Printer(ostream& os)
    : m_os{os}
{
#if defined(__EMSCRIPTEN__)
    setPrintText();
#elif defined(_WIN32)
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &mode))
    {
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        setPrintTerm();
    }
    else
    {
        setPrintText();
    }
#else
    if (isatty(STDOUT_FILENO))
    {
        setPrintTerm();
    }
    else
    {
        setPrintText();
    }
#endif
}

log::Printer& log::Printer::instance()
{
    static Printer x{cout};
    return x;
}


void log::Printer::setPrintTerm()
{
    print_mode      = TERM;
    esc_trace       = getEscBg( color(0x71f100FF) );
    esc_debug       = getEscBg( color(0x71f100FF) );
    esc_info        = getEscBg( color(0x00a1f1FF) );
    esc_warning     = getEscBg( color(0xf18100FF) );
    esc_error       = getEscBg( color(0xf15000FF) );
    esc_fatal       = getEscBg( color(0xf10000FF) );
    esc_value       = colors::hotpink.toFgEscCode();
    esc_affix       = colors::gold.toFgEscCode();
    esc_tim         = getEscBg( color(0xf1e900FF) );
    esc_src         = "";
    esc_reset       = TERM_RESET;
}

void log::Printer::setPrintText()
{
    print_mode      = TEXT;
    esc_trace       = "";
    esc_debug       = "";
    esc_info        = "";
    esc_warning     = "";
    esc_error       = "";
    esc_fatal       = "";
    esc_value       = "";
    esc_affix       = "";
    esc_tim         = "";
    esc_src         = "";
    esc_reset       = "";
}


string log::Printer::strLvl(Level lvl) const
{
    switch (lvl)
    {
        case TRACE:   return print_mode == TERM ? "  TRACE  " : "[  TRACE  ]";
        case DEBUG:   return print_mode == TERM ? "  DEBUG  " : "[  DEBUG  ]";
        case INFO:    return print_mode == TERM ? "  INFO   " : "[  INFO   ]";
        case WARNING: return print_mode == TERM ? " WARNING " : "[ WARNING ]";
        case ERROR:   return print_mode == TERM ? "  ERROR  " : "[  ERROR  ]";
        case FATAL:   return print_mode == TERM ? "  FATAL  " : "[  FATAL  ]";
        default:nopath_case(log::Level);
    }
    return "[  ?????  ]";
}

string const& log::Printer::escLvl(Level lvl) const
{
    switch (lvl)
    {
        case TRACE:   return esc_trace;
        case DEBUG:   return esc_debug;
        case INFO:    return esc_info;
        case WARNING: return esc_warning;
        case ERROR:   return esc_error;
        case FATAL:   return esc_fatal;
        default:nopath_case(log::Level);
    }

    thread_local string s = "[  ?????  ]";
    return s;
}

std::string log::Printer::strSrc(source_location const& src)
{
    auto str = format("{}:{}",
        strview(src.file_name()).split("/").back().str(),
        src.line());
    m_src_pad = max(m_src_pad, str.size());
    return format("{:{}}", str, m_src_pad);
}

std::string log::Printer::strTim(local_datetime const& tim) const
{
    return tim.str(print_mode == TERM ? " %p %I:%M:%S ": "[ %p %I:%M:%S ]");
}

void log::Printer::printLog(Log const& log)
{
    m_indent = 1;

    string str = strLvl(log.lvl);
    m_indent += str.size() + 1;
    m_os << " " << escLvl(log.lvl) << str << esc_reset;

    str = strTim(log.tim);
    m_indent += str.size() + 1;
    m_os << " " << esc_tim << str << esc_reset;

    str = strSrc(log.src);
    m_indent += str.size() + 1;
    m_os << " " << esc_src << str << esc_reset;

    m_os << " " << log.msg << "\n";
}

string log::Printer::getPrefix(VarChars const& v) const
{
    ostringstream oss;
    oss << esc_affix << v.open
        << esc_reset << v.prefix
        << esc_value;
    return oss.str();
}

string log::Printer::getSuffix(VarChars const& v) const
{
    ostringstream oss;
    oss
        << esc_reset << v.suffix
        << esc_affix << v.close
        << esc_reset;
    return oss.str();
}




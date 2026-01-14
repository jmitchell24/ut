//
// Created by james on 26/09/25.
//

#include "ut/log.hpp"
#include "ut/term/escapes.hpp"
#include "ut/random.hpp"
using namespace ut;
using namespace ut::log;


//
// std
//

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#ifdef _WIN32
// Fuck you Windows!
#else
#include <unistd.h>
#endif

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

/// return true if term, otherwise false (text)
bool getSystemPrintMode()
{
#if defined(__EMSCRIPTEN__)
    return false;
#elif defined(_WIN32)
    return false; // Fuck you Windows!
#else
    return isatty(STDOUT_FILENO) ? true : false;
#endif
}

struct PrinterTerm
{
    string esc_trace;
    string esc_debug;
    string esc_info;
    string esc_warning;
    string esc_error;
    string esc_fatal;
    string esc_tim;
    string esc_src;
    string esc_value;
    string esc_affix;
    string esc_reset;

    PrinterTerm()
    {
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

    string strLvl(Level lvl) const
    {
        switch (lvl)
        {
            case TRACE:   return "  TRACE  ";
            case DEBUG:   return "  DEBUG  ";
            case INFO:    return "  INFO   ";
            case WARNING: return " WARNING ";
            case ERROR:   return "  ERROR  ";
            case FATAL:   return "  FATAL  ";
            default:nopath_case(log::Level);
        }
        return "[  ?????  ]";
    }

    string escLvl(Level lvl) const
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


        return "[  ?????  ]";
    }

    std::string strSrc(source_location const& src) const
    {
        thread_local size_t src_pad=0;

        auto str = format("{}:{}",
            strview(src.file_name()).split("/").back().str(),
            src.line());
        src_pad = max(src_pad, str.size());
        return format("{:{}}", str, src_pad);
    }

    std::string strTim(local_datetime const& tim) const
    {
        return tim.str(" %p %I:%M:%S ");
    }

    std::string getString(Log const& log) const
    {
        ostringstream oss;

        string str = strLvl(log.lvl);
        oss << " " << escLvl(log.lvl) << str << esc_reset;

        str = strTim(log.tim);
        oss << " " << esc_tim << str << esc_reset;

        str = strSrc(log.src);
        oss << " " << esc_src << str << esc_reset;

        oss << " " << log.msg << "\n";

        return oss.str();
    }

    string getPrefix(VarChars const& v) const
    {
        ostringstream oss;
        oss << esc_affix << v.open
            << esc_reset << v.prefix
            << esc_value;
        return oss.str();
    }

    string getSuffix(VarChars const& v) const
    {
        ostringstream oss;
        oss
            << esc_reset << v.suffix
            << esc_affix << v.close
            << esc_reset;
        return oss.str();
    }

    static PrinterTerm& instance()
    {
        static PrinterTerm x;
        return x;
    }

};

struct PrinterText
{
    string strLvl(Level lvl) const
    {
        switch (lvl)
        {
            case TRACE:   return "[  TRACE  ]";
            case DEBUG:   return "[  DEBUG  ]";
            case INFO:    return "[  INFO   ]";
            case WARNING: return "[ WARNING ]";
            case ERROR:   return "[  ERROR  ]";
            case FATAL:   return "[  FATAL  ]";
            default:nopath_case(log::Level);
        }
        return "[  ?????  ]";
    }

    string escLvl(Level lvl) const
    {
        switch (lvl)
        {
            case TRACE:   return "";
            case DEBUG:   return "";
            case INFO:    return "";
            case WARNING: return "";
            case ERROR:   return "";
            case FATAL:   return "";
            default:nopath_case(log::Level);
        }

        thread_local string s = "[  ?????  ]";
        return s;
    }

    std::string strSrc(source_location const& src) const
    {
        thread_local size_t src_pad=0;

        auto str = format("{}:{}",
            strview(src.file_name()).split("/").back().str(),
            src.line());
        src_pad = max(src_pad, str.size());
        return format("{:{}}", str, src_pad);
    }

    std::string strTim(local_datetime const& tim) const
    {
        return tim.str("[ %p %I:%M:%S ]");
    }

    std::string getString(Log const& log) const
    {
        ostringstream oss;

        string str = strLvl(log.lvl);
        oss << " " << escLvl(log.lvl) << str;

        str = strTim(log.tim);
        oss << " " << str;

        str = strSrc(log.src);
        oss << " " << str;

        oss << " " << log.msg << "\n";

        return oss.str();
    }

    string getPrefix(VarChars const& v) const
    {
        ostringstream oss;
        oss << v.open
             << v.prefix
            ;
        return oss.str();
    }

    string getSuffix(VarChars const& v) const
    {
        ostringstream oss;
        oss
             << v.suffix
             << v.close
            ;
        return oss.str();
    }

    static PrinterText& instance()
    {
        static PrinterText x;
        return x;
    }
};

Logger::Logger()
{
    if (getSystemPrintMode())
        setTerm();
    else
        setText();

    setDefaultLog();
}

void Logger::setText()
{
    strfn([](Log const& l) { return PrinterText::instance().getString(l); });
    strPreFn([](VarChars const& v) { return PrinterText::instance().getPrefix(v); });
    strSufFn([](VarChars const& v) { return PrinterText::instance().getSuffix(v); });
}

void Logger::setTerm()
{
    strfn([](Log const& l) { return PrinterTerm::instance().getString(l); });
    strPreFn([](VarChars const& v) { return PrinterTerm::instance().getPrefix(v); });
    strSufFn([](VarChars const& v) { return PrinterTerm::instance().getSuffix(v); });
}

void Logger::setDefaultLog()
{
    logfn([](std::string const& s) { std::cout << s; });
}

Logger& Logger::instance()
{
    static Logger x;
    return x;
}
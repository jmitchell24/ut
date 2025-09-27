//
// Created by james on 26/09/25.
//

#include "ut/log.hpp"
#include "ut/term/escapes.hpp"
#include "ut/random.hpp"
using namespace ut;

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

void printItem(ostream& os, log::Style::Item const& it)
{
    os << it.fg.toFgEscCode() << it.bg.toBgEscCode();
}


log::Style::Style()
{
    // Log levels - pastels with good contrast
    level_trace   = Item { color(0x71f100FF) };
    level_debug   = Item { color(0x71f100FF) };
    level_info    = Item { color(0x00a1f1FF) };
    level_warning = Item { color(0xf18100FF) };
    level_error   = Item { color(0xf15000FF) };
    level_fatal   = Item { color(0xf10000FF) };

    var_value = colors::hotpink;
    var_affix = colors::gold;

    // Supporting elements - subtle and readable
    time = Item { color(0xf1e900FF)  };
    src  = Item { color(0x808080FF) };


}

void log::Style::printLevel(ostream& os, Level level) const
{
    if (print_mode == TERM)
    {
        switch (level)
        {
            case TRACE:    os << " "; printItem(os, level_trace  ); os << "▓▒░  TRACE  ░▒▓" << esc::reset; break;
            case DEBUG:    os << " "; printItem(os, level_debug  ); os << "▓▒░  DEBUG  ░▒▓" << esc::reset; break;
            case INFO:     os << " "; printItem(os, level_info   ); os << "▓▒░  INFO   ░▒▓" << esc::reset; break;
            case WARNING:  os << " "; printItem(os, level_warning); os << "▓▒░ WARNING ░▒▓" << esc::reset; break;
            case ERROR:    os << " "; printItem(os, level_error  ); os << "▓▒░  ERROR  ░▒▓" << esc::reset; break;
            case FATAL:    os << " "; printItem(os, level_fatal  ); os << "▓▒░  FATAL  ░▒▓" << esc::reset; break;
            default:nopath_case(log::Level);
        }
    }
    else
    {
        switch (level)
        {
            case TRACE:    os << "[  TRACE  ]"; break;
            case DEBUG:    os << "[  DEBUG  ]"; break;
            case INFO:     os << "[  INFO   ]"; break;
            case WARNING:  os << "[ WARNING ]"; break;
            case ERROR:    os << "[  ERROR  ]"; break;
            case FATAL:    os << "[  FATAL  ]"; break;
            default:nopath_case(log::Level);
        }
    }
}

static int g_pad = 0;

void log::Style::printSrc(ostream& os, source_location const& src) const
{
    auto str = format("{}:{}",
        strview(src.file_name()).split("/").back().str(),
        src.line());

    g_pad = max(g_pad, (int)str.size());

    os << "[ " << setw(g_pad) << str << " ]";
}

void log::Style::printTimestamp(ostream& os, local_datetime const& timestamp) const
{
    if (print_mode == TERM)
    {
        printItem(os, time); os << timestamp.str(" %p %I:%M:%S ") << esc::reset;
    }
    else
    {
        os << timestamp.str("[ %p %I:%M:%S ]");
    }
}

void log::Style::printMessage(ostream& os, string const& message) const
{
    os << message;
}




void log::Style::printLog(std::ostream& os, Log const& log) const
{
    printLevel(os, log.level);
    os << " ";
    printTimestamp(os, log.timestamp);
    os << " ";
    printSrc(os, log.src);
    os << " ";
    printMessage(os, log.message);

    if (print_mode == TERM)
        os << esc::rendl;
    else
        os << endl;
}

string log::Style::getPrefix(VarChars const& v) const
{
    if (print_mode == TERM)
    {
        ostringstream oss;
        oss << var_affix.toFgEscCode() << v.open
            << esc::reset << v.prefix
            << var_value.toFgEscCode();
        return oss.str();
    }
    return v.open;
}

string log::Style::getSuffix(VarChars const& v) const
{
    if (print_mode == TERM)
    {
        ostringstream oss;
        oss
            << esc::reset << v.suffix
            << var_affix.toFgEscCode() << v.close
            << esc::reset;
        return oss.str();
    }
    return v.close;
}



log::Sink& log::Sink::instance()
{
    static Sink x;
    return x;
}


void log::Sink::push(Log const& log)
{
    style.printLog(cout, log);
}


//
// Created by james on 25/09/25.
//

#pragma once

//
// ut
//

#include "ut/term/escapes.hpp"
#include "ut/string.hpp"
#include "ut/color.hpp"
#include "ut/time.hpp"

//
// std
//
#include <source_location>
#include <utility>

#define EXPAND_STRUCTS(X)           \
    X(   PrintWrap  ) \
    X(   Timestamp  ) \
    X(   Prefix     ) \
    X(   Parameter  ) \
    X(   SrcLoc     )



namespace ut::log
{
    struct Theme
    {
        color timestamp         = colors::gold;
        color param_box         = colors::gold;
        color param_val         = colors::hotpink;
        color source            = colors::cornflowerblue;
        color prefix_error      = colors::orangered;
        color prefix_warning    = colors::orange;
        color prefix_info       = colors::lightgreen;
        color prefix_fatal      = colors::darkred;

        static Theme instance()
        {
            static Theme x;
            return x;
        }
    };

    enum PrefixKind
    { INFO, WARNING, ERROR, FATAL };

    enum TimestampKind
    { DATE, TIME };

    struct SrcLocOpts
    { bool line=false, col=false, file_short=false, file_long=false; };

namespace detail
{
    static void printLabel(std::ostream& os, std::string const& str, color const& bg)
    {
        auto fg = bg.toOKLCH().l > .5 ? colors::black : colors::white;

        os
            << bg.toBgEscCode() << fg.toFgEscCode()
            << str
            << esc::reset;
    }

    struct PrintWrap
    {
        strparam    str;
        size_t      limit;
        char const* prefix;
        char const* suffix;

        void print(std::ostream& os) const
        {
            auto s = str;
            if (limit > 0 && !s.empty())
            {
                while (s.size() > limit)
                {
                    os << prefix << s.take(limit) << suffix;
                    s = s.skipBegin(limit);

                }
            }
            os << prefix << s << suffix;
        }
    };

    struct SrcLoc
    {
        std::source_location sl;
        SrcLocOpts opts;
        void print(std::ostream& os) const
        {
            std::string location;

            if (opts.file_short)
            {
                // TODO: Implement rsplit and use it here
                location += strview(sl.file_name()).split("/").back().str();
            }
            else if (opts.file_long)
            {
                location += sl.file_name();
            }

            if (opts.line)
            {
                if (!location.empty()) location += ":";
                location += std::to_string(sl.line());
            }

            if (opts.col)
            {
                if (!location.empty()) location += ":";
                location += std::to_string(sl.column());
            }

            if (!location.empty())
            {
                auto&& t = Theme::instance();
                // location = " " + location + " ";
                printLabel(os, location, t.source);
                os << " ";
            }
        }
    };

    struct Timestamp
    {
        local_datetime dt;
        TimestampKind k;
        Timestamp(local_datetime const& dt, TimestampKind k) : dt{dt}, k{k} { }
        void print(std::ostream& os) const
        {
            auto&& t = Theme::instance();
            switch (k)
            {
                case DATE: printLabel(os, dt.str(" %Y-%m-%d "), t.timestamp); break;
                case TIME: printLabel(os, dt.str(" %p %I:%M:%S "), t.timestamp); break;
            }
            os << " ";
        }
    };

    struct Prefix
    {
        PrefixKind kind;
        Prefix(PrefixKind kind)
            : kind{kind}
        { }

        void print(std::ostream& os) const
        {
            auto&& t = Theme::instance();
            switch (kind)
            {
                case INFO   : printLabel(os, "  INFO   ", t.prefix_info); break;
                case WARNING: printLabel(os, " WARNING ", t.prefix_warning); break;
                case ERROR  : printLabel(os, "  ERROR  ", t.prefix_error); break;
                case FATAL  : printLabel(os, "  FATAL  ", t.prefix_fatal); break;
            }
            os << " ";
        }
    };

    struct Parameter
    {
        std::string lbl;
        std::string val;
        std::string suf;
        std::string pre;

        void print(std::ostream& os) const
        {
            auto&& t = Theme::instance();
            os
            << " "
            << t.param_box.toFgEscCode() << "[" << esc::reset
            << lbl << " "
            << t.param_val.toFgEscCode() << pre << val << suf
            << t.param_box.toFgEscCode() << "]" << esc::reset
            << " ";
        }
    };
}



    //
    // stream manipulators
    //

    static std::ostream& info(std::ostream& os)
    { detail::Prefix(INFO).print(os); return os; }

    static std::ostream& warning(std::ostream& os)
    { detail::Prefix(WARNING).print(os); return os; }

    static std::ostream& error(std::ostream& os)
    { detail::Prefix(ERROR).print(os); return os; }

    static std::ostream& fatal(std::ostream& os)
    { detail::Prefix(FATAL).print(os); return os; }

    static std::ostream& date(std::ostream& os)
    { detail::Timestamp(local_datetime::now(), DATE).print(os); return os; }

    static std::ostream& time(std::ostream& os)
    { detail::Timestamp(local_datetime::now(), TIME).print(os); return os; }

    static std::ostream& end(std::ostream& os)
    { return os << esc::rendl; }

    //
    // parametarized stream manipulators
    //

    static detail::SrcLoc src(SrcLocOpts opts={true,true,true}, std::source_location const& sl = std::source_location::current())
    { return { .sl=sl, .opts = opts }; }

    static detail::PrintWrap wrap(strparam str, size_t limit, char const* prefix="", char const* suffix="\n")
    {
        check_null(prefix); check_null(suffix);
        return { .str =str, .limit =limit, .prefix =prefix, .suffix =suffix };
    }

    template <typename T>
    static detail::Parameter param(strparam lbl, T const&& t, char const* suffix="", char const* prefix="")
    { return { lbl.str(), std::to_string(t), suffix, prefix }; }

}

namespace ut
{
    //
    // struct operator overloads
    //

#define X(_type) \
    static std::ostream& operator<< (std::ostream& os, log::detail::_type const& x) { x.print(os);  return os; }
EXPAND_STRUCTS(X)
#undef X
}

#undef EXPAND_STRUCTS
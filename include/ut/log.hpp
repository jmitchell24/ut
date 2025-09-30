//
// Created by james on 26/09/25.
//

#pragma once

//
// ut
//

#include <ut/string.hpp>
#include <ut/term/escapes.hpp>
#include <ut/color.hpp>
#include <ut/time.hpp>

//
// std
//
#include <vector>
#include <mutex>
#include <source_location>


#ifndef NDEBUG
#define ut_trace    ( ut::log::trace() )
#define ut_debug    ( ut::log::debug() )
#else
#define ut_trace(...)
#define ut_debug(...)
#endif

#define ut_info     ( ut::log::info() )
#define ut_warning  ( ut::log::warning() )
#define ut_error    ( ut::log::error() )
#define ut_fatal    ( ut::log::fatal() )

#define ut_var(_var)   ( ut::log::var( (_var), #_var "=") )
#define ut_list(_list) ( ut::log::list( (_list), #_list "=" ) )

#ifdef _WIN32
#define DEFAULT_PRINT_MODE ( ut::log::TEXT )
#else
#define DEFAULT_PRINT_MODE ( ut::log::TERM )
#endif

namespace ut::log
{

    enum Level { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL };

    enum PrintMode
    {
        TERM, ///< Print to modern terminal (escapes, unicode chars, etc...)
        TEXT  ///< Print to plaintext
    };

    struct Log
    {
        Level                   level;
        std::source_location    src;
        local_datetime          timestamp;
        std::string             message;
    };

    struct VarChars
    {
        char const *prefix="", *suffix="", *open="[", *close="]";
        bool use_newline=false;
    };

    template<typename T> struct Var
    {
        T const& value;
        VarChars chars;
    };

    template<typename T> struct VarList
    {
        T const& value;
        VarChars chars;
    };


    struct Style
    {
        struct Item
        {
            color bg, fg;
            Item(color const& bg, color const& fg)
                : bg{bg}, fg{fg}
            { }

            explicit Item(color const& bg = colors::white)
                : bg{bg}, fg{bg.toOKLCH().l < .5 ? colors::white : colors::black}
            { }
        };

        Item level_trace    { };
        Item level_debug    { };
        Item level_info     { };
        Item level_warning  { };
        Item level_error    { };
        Item level_fatal    { };

        Item time   { };
        Item src    { };

        color var_value { };
        color var_affix { };

        PrintMode print_mode = DEFAULT_PRINT_MODE;

        Style();

        /// Prints log to stream.
        ///     \returns The length of the log BEFORE message (for indenting).
        void printLog(std::ostream& os, Log const& log) const;
        [[nodiscard]] std::string getPrefix(VarChars const& v) const;
        [[nodiscard]] std::string getSuffix(VarChars const& v) const;

    private:
        void printLevel(std::ostream& os, Level level) const;
        void printSrc(std::ostream& os, std::source_location const& src) const;
        void printTimestamp(std::ostream& os, local_datetime const& timestamp) const;
        void printMessage(std::ostream& os, std::string const& message) const;
    };



    class Sink
    {
    public:
        Style style;

        static Sink& instance();

        void push(Log const& log);

    private:
    };

    class Builder
    {
    public:
        Log log;

        Builder(Level level, std::source_location src)
            : log{ .level=level, .src=src, .timestamp = local_datetime::now() }
        { }

        ~Builder()
        { Sink::instance().push(log); }

        Builder()=delete;
        Builder(Builder const&)=delete;
        Builder(Builder&&)=delete;
        Builder& operator= (Builder const&)=delete;
        Builder& operator= (Builder&&)=delete;

        template <typename... Args>
        Builder& operator() (std::string const& fmt, Args&&... args)
        {
            log.message += std::vformat(fmt, std::make_format_args(args...));
            return *this;
        }
    };


    template<typename T> [[maybe_unused]] auto var(T const& value,
    char const* pre="", char const* suf="", char const* open="[", char const* close="]")
    { return Var<T>{value, {pre, suf, open, close}}; }

    template<typename T> [[maybe_unused]] auto list(T const& value,
    bool use_newlines=false, char const* pre="", char const* suf="", char const* open="[", char const* close="]")
    { return VarList<T>{value, {pre, suf, open, close, use_newlines} }; }

    [[maybe_unused]] static Builder trace   (std::source_location src = std::source_location::current()) { return {TRACE  , src}; }
    [[maybe_unused]] static Builder debug   (std::source_location src = std::source_location::current()) { return {DEBUG  , src}; }
    [[maybe_unused]] static Builder info    (std::source_location src = std::source_location::current()) { return {INFO   , src}; }
    [[maybe_unused]] static Builder warning (std::source_location src = std::source_location::current()) { return {WARNING, src}; }
    [[maybe_unused]] static Builder error   (std::source_location src = std::source_location::current()) { return {ERROR  , src}; }
    [[maybe_unused]] static Builder fatal   (std::source_location src = std::source_location::current()) { return {FATAL  , src}; }



}

namespace std
{
    template<typename T>
    struct formatter<ut::log::Var<T>>
    {
        formatter<T> underlying_formatter;

        constexpr auto parse(std::format_parse_context& ctx)
        {
            return underlying_formatter.parse(ctx);
        }

        auto format(ut::log::Var<T> const& p, std::format_context& ctx) const
        {
            auto&& s = ut::log::Sink::instance();

            std::format_to(ctx.out(), "{}", s.style.getPrefix(p.chars));
            auto it = underlying_formatter.format(p.value, ctx);
            return std::format_to(it, "{}", s.style.getSuffix(p.chars)); // reset color
        }
    };

    template<typename T>
    struct formatter<ut::log::VarList<T>>
    {
        using element_type = std::remove_reference_t<decltype(*std::declval<T>().begin())>;
        formatter<element_type> underlying_formatter;

        constexpr auto parse(std::format_parse_context& ctx)
        {
            return underlying_formatter.parse(ctx);
        }

        auto format(ut::log::VarList<T> const& p, std::format_context& ctx) const
        {
            auto&& s = ut::log::Sink::instance();

            std::format_to(ctx.out(), "{}", s.style.getPrefix(p.chars));

            auto it = p.value.begin();
            auto end = p.value.end();

            if (it != end)
            {
                if (p.chars.use_newline)
                {
                    std::format_to(ctx.out(), "\n  ");
                }

                ctx.advance_to(underlying_formatter.format(*it, ctx));
                ++it;

                for (; it != end; ++it)
                {
                    if (p.chars.use_newline)
                    {
                        std::format_to(ctx.out(), ",\n  ");
                    }
                    else
                    {
                        std::format_to(ctx.out(), ", ");
                    }
                    ctx.advance_to(underlying_formatter.format(*it, ctx));
                }

                if (p.chars.use_newline)
                {
                    std::format_to(ctx.out(), "\n");
                }
            }

            return std::format_to(ctx.out(), "{}", s.style.getSuffix(p.chars));
        }
    };
}
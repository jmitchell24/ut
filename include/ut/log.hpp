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
#include <functional>
#include <iostream>


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

#define ut_var(_var)                ( ut::log::var( (_var), #_var "=") )
#define ut_named_var(_name, _var)   ( ut::log::var( (_var), _name "=") )
#define ut_list(_list)              ( ut::log::list( (_list), false, #_list "=" ) )
#define ut_vlist(_list)             ( ut::log::list( (_list), true, #_list "=" ) )

#define ut_logger ( ut::log::Logger::instance() )



namespace ut::log
{

    enum Level { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL };

    struct Log
    {
        Level                   lvl;
        std::source_location    src;
        local_datetime          tim;
        std::string             msg;
    };

    struct VarChars
    {
        char const *prefix="", *suffix="", *open="[", *close="]", *delim=",";
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

    struct Printer
    {
        /// function to stringify log (default to text or term string, if supported)
        using strfn_type = std::function<std::string(Log const&)>;

        /// function to stringify var prefix or suffix (default to text or term string, if supported)
        using varfn_type = std::function<std::string(VarChars const&)>;

        strfn_type m_strfn;
        varfn_type m_pre_varfn;
        varfn_type m_suf_varfn;

        static Printer getText();
        static Printer getTerm();
        static Printer getSystem();
    };

    class Logger
    {
    public:
        /// function to output log as string (default to std::cout)
        using logfn_type = std::function<void(std::string const&)>;



        void logfn(logfn_type const& fn)
        { auto g = std::lock_guard{m_mutex}; m_logfn = fn; }

        /// submit log
        void log(Printer const& prt, Log const& log) const
        { auto g = std::lock_guard{m_mutex}; m_logfn(prt.m_strfn(log)); }

        /// Set Term Mode
        void setTerm();

        /// Set Text Mode
        void setText();

        /// Set Default Log
        void setDefaultLog();

        static Logger& instance();

    private:
        Logger();

        logfn_type m_logfn;


        mutable std::mutex m_mutex;
    };

    class LogGuard
    {
    public:
        Log log;

        LogGuard(Level lvl, std::source_location src)
            : log{ .lvl=lvl, .src=src, .tim = local_datetime::now(), .msg={} }
        { }

        ~LogGuard()
        { Logger::instance().log(log); }

        LogGuard()=delete;
        LogGuard(LogGuard const&)=delete;
        LogGuard(LogGuard&&)=delete;
        LogGuard& operator= (LogGuard const&)=delete;
        LogGuard& operator= (LogGuard&&)=delete;

        template <typename... Args>
        LogGuard& operator() (std::string const& fmt, Args&&... args)
        {
            log.msg += std::vformat(fmt, std::make_format_args(args...));
            return *this;
        }
    };


    template<typename T> [[maybe_unused]] auto var(T const& value,
    char const* pre="", char const* suf="", char const* open="[", char const* close="]")
    { return Var<T>{value, {pre, suf, open, close}}; }

    template<typename T> [[maybe_unused]] auto list(T const& value,
    bool use_newlines=false, char const* pre="", char const* suf="", char const* open="[", char const* close="]", char const* delim=",")
    { return VarList<T>{value, {pre, suf, open, close, delim, use_newlines} }; }

    [[maybe_unused]] static LogGuard trace   (std::source_location src = std::source_location::current()) { return {TRACE  , src}; }
    [[maybe_unused]] static LogGuard debug   (std::source_location src = std::source_location::current()) { return {DEBUG  , src}; }
    [[maybe_unused]] static LogGuard info    (std::source_location src = std::source_location::current()) { return {INFO   , src}; }
    [[maybe_unused]] static LogGuard warning (std::source_location src = std::source_location::current()) { return {WARNING, src}; }
    [[maybe_unused]] static LogGuard error   (std::source_location src = std::source_location::current()) { return {ERROR  , src}; }
    [[maybe_unused]] static LogGuard fatal   (std::source_location src = std::source_location::current()) { return {FATAL  , src}; }



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
            auto&& fnPre = ut::log::Logger::instance().strPreFn();
            auto&& fnSuf = ut::log::Logger::instance().strSufFn();

            std::format_to(ctx.out(), "{}", fnPre(p.chars));
            auto it = underlying_formatter.format(p.value, ctx);
            return std::format_to(it, "{}", fnSuf(p.chars)); // reset color
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
            auto&& fnPre = ut::log::Logger::instance().strPreFn();
            auto&& fnSuf = ut::log::Logger::instance().strSufFn();

            auto indent  = string(1, ' ');
            auto indent2 = string(5, ' ');

            if (p.chars.use_newline)
                std::format_to(ctx.out(), "{}", fnPre(p.chars));
            else
                std::format_to(ctx.out(), "{}", fnPre(p.chars));

            auto it = p.value.begin();
            auto end = p.value.end();

            if (it != end)
            {
                if (p.chars.use_newline)
                {
                    std::format_to(ctx.out(), "\n{}", indent2);
                }

                ctx.advance_to(underlying_formatter.format(*it, ctx));
                ++it;

                for (; it != end; ++it)
                {
                    if (p.chars.use_newline)
                    {
                        std::format_to(ctx.out(), "{}\n{}", p.chars.delim, indent2);
                    }
                    else
                    {
                        std::format_to(ctx.out(), "{} ", p.chars.delim);
                    }
                    ctx.advance_to(underlying_formatter.format(*it, ctx));
                }

                if (p.chars.use_newline)
                {
                    std::format_to(ctx.out(), "\n");
                }
            }

            if (p.chars.use_newline)
                return std::format_to(ctx.out(), "{}{}", indent, fnSuf(p.chars));
            return std::format_to(ctx.out(), "{}", fnSuf(p.chars));
        }
    };
}
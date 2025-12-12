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

#define ut_var(_var)                ( ut::log::var( (_var), #_var "=") )
#define ut_named_var(_name, _var)   ( ut::log::var( (_var), _name "=") )
#define ut_list(_list)              ( ut::log::list( (_list), false, #_list "=" ) )
#define ut_vlist(_list)             ( ut::log::list( (_list), true, #_list "=" ) )



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


    class Printer
    {
    public:
        Printer(std::ostream& os);

        [[nodiscard]] size_t indent() const
        { return m_indent; }

        void setPrintTerm();
        void setPrintText();

        void printLog(Log const& log);
        [[nodiscard]] std::string getPrefix(VarChars const& v) const;
        [[nodiscard]] std::string getSuffix(VarChars const& v) const;

        static Printer& instance();

    private:
        size_t m_indent=0;
        size_t m_src_pad=0;

        std::ostream& m_os;

        PrintMode print_mode=TEXT;

        std::string esc_trace;
        std::string esc_debug;
        std::string esc_info;
        std::string esc_warning;
        std::string esc_error;
        std::string esc_fatal;
        std::string esc_tim;
        std::string esc_src;
        std::string esc_value;
        std::string esc_affix;
        std::string esc_reset;

        std::string strLvl(Level lvl) const;
        std::string strSrc(std::source_location const& src);
        std::string strTim(local_datetime const& tim) const;

        std::string const& escLvl(Level lvl) const;
    };

    class Builder
    {
    public:
        Log log;
        Printer& prt;

        Builder(Level lvl, std::source_location src, Printer& prt = Printer::instance())
            : log{ .lvl=lvl, .src=src, .tim = local_datetime::now(), .msg={} }, prt{prt}
        { }

        ~Builder()
        { prt.printLog(log); }

        Builder()=delete;
        Builder(Builder const&)=delete;
        Builder(Builder&&)=delete;
        Builder& operator= (Builder const&)=delete;
        Builder& operator= (Builder&&)=delete;

        template <typename... Args>
        Builder& operator() (std::string const& fmt, Args&&... args)
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

    [[maybe_unused]] static Builder trace   (std::source_location src = std::source_location::current()) { return {TRACE  , src}; }
    [[maybe_unused]] static Builder debug   (std::source_location src = std::source_location::current()) { return {DEBUG  , src}; }
    [[maybe_unused]] static Builder info    (std::source_location src = std::source_location::current()) { return {INFO   , src}; }
    [[maybe_unused]] static Builder warning (std::source_location src = std::source_location::current()) { return {WARNING, src}; }
    [[maybe_unused]] static Builder error   (std::source_location src = std::source_location::current()) { return {ERROR  , src}; }
    [[maybe_unused]] static Builder fatal   (std::source_location src = std::source_location::current()) { return {FATAL  , src}; }

    PrintMode getSystemPrintMode();

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
            auto&& prt = ut::log::Printer::instance();

            std::format_to(ctx.out(), "{}", prt.getPrefix(p.chars));
            auto it = underlying_formatter.format(p.value, ctx);
            return std::format_to(it, "{}", prt.getSuffix(p.chars)); // reset color
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
            auto&& prt = ut::log::Printer::instance();

            auto indent  = string(1, ' ');
            auto indent2 = string(5, ' ');

            if (p.chars.use_newline)std::format_to(ctx.out(), "{}", prt.getPrefix(p.chars));
            else
                std::format_to(ctx.out(), "{}", prt.getPrefix(p.chars));

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
                return std::format_to(ctx.out(), "{}{}", indent, prt.getSuffix(p.chars));
            return std::format_to(ctx.out(), "{}", prt.getSuffix(p.chars));
        }
    };
}
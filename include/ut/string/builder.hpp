#pragma once

#if !defined(UT_BUILDER_BUFFER_SIZE)
#define UT_BUILDER_BUFFER_SIZE 256
#endif

#include "fmt.hpp"

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

namespace ut
{
    class strbuilder
    {
    public:
        M_DECL void clear()
        {
            m_str.clear();
        }

        M_DECL void append(strparam view)
        {
            m_str.append(view.begin(), view.size());
        }

        M_DECL void appendln(strparam view)
        {
            append(view);
            append("\n"_str);
        }

        M_DECL void appendf(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            appendfv(fmt, args);
            va_end(args);
        }

        M_DECL void appendfln(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            appendfvln(fmt, args);
            va_end(args);
        }

        M_DECL void appendfv(char const* fmt, va_list args)
        {
            auto view = m_fmtbuf.viewv(fmt, args);
            append(view);
        }

        M_DECL void appendfvln(char const* fmt, va_list args)
        {
            appendfv(fmt, args);
            append("\n"_str);
        }

        M_DECL_PURE std::string const& string() const
        { return m_str; }

        M_DECL_PURE strview view() const
        { return m_str; }

    private:
        std::string     m_str;
        fbuf_t          m_fmtbuf;
    };
}

#undef M_DECL_PURE
#undef M_DECL

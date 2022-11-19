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
        struct line { size_t beg=0, end=0; };

        using linelist_type = std::vector<line>;
        using fmtbuf_type   = basic_format_buffer<UT_BUILDER_BUFFER_SIZE, 1>;

        M_DECL strbuilder()
        { m_lines.push_back({0,0}); }

        M_DECL_PURE strview lineView(line const& x)
        {
            assert(x.beg <= x.end);
            assert(x.end <= m_str.size());

            char const* p = m_str.data();
            return strview{p + x.beg, p + x.end};
        }

        M_DECL_PURE linelist_type const& lines() const
        {
            return m_lines;
        }

        M_DECL void clear()
        {
            m_str.clear();
            m_lines.clear();
            m_lines.push_back({0,0});
        }

        M_DECL void append(cstrparam view)
        {
            m_str.append(view.c_str());

            char const* beg = m_str.data() + m_str.size() - view.size();
            char const* end = m_str.data() + m_str.size();

            for (char const* it = beg; it != end; ++it)
            {
                if (*it == '\n')
                {
                    size_t i = it - m_str.data();
                    m_lines.back().end = i;
                    m_lines.push_back({i+1, m_str.size()});
                }
            }

            m_lines.back().end = m_str.size();
        }

        M_DECL void appendf(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            appendfv(fmt, args);
            va_end(args);
        }

        M_DECL void appendfv(char const* fmt, va_list args)
        {
            auto view = m_fmtbuf.viewv(fmt, args);
            append(view);
        }

        M_DECL_PURE std::string const& string() const
        { return m_str; }

        M_DECL_PURE strview view() const
        { return m_str; }

    private:
        std::string         m_str;
        fmtbuf_type         m_fmtbuf;
        linelist_type       m_lines;
    };
}

#undef M_DECL_PURE
#undef M_DECL

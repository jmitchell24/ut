#pragma once

#include "view.hpp"

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

namespace ut
{
    class strbuilder
    {
    public:
        strbuilder()=default;
        strbuilder(strbuilder const&)=default;
        strbuilder(strbuilder&&)=default;
        strbuilder& operator=(strbuilder const&)=default;
        strbuilder& operator=(strbuilder&&)=default;

        M_DECL_PURE std::string const& str() const
        { return m_str; }

        M_DECL_PURE strview view() const
        { return strview(m_str.data(), m_str.data()+m_str.size()); }

        M_DECL_PURE size_t size() const
        { return m_str.size()-1; }

        M_DECL void clear()
        { m_str.clear(); }

        M_DECL void put(strparam view)
        {
            size_t n = view.size();
            char* s = grow(n);
            view.strncpy(s,n);
        }

        M_DECL void putln(strparam view)
        {
            put(view);
            put("\n"_sv);
        }

        M_DECL void printf(char const* fmt, ...)
        {
            size_t n;
            {
                va_list args;
                va_start(args, fmt);
                int res = vsnprintf(nullptr, 0, fmt, args);
                va_end(args);

                assert(res >= 0);
                if (res <= 0)
                    return;
                n = res;
            }

            {
                va_list args;
                va_start(args, fmt);
                char* s = grow(n);

                [[maybe_unused]]
                size_t res = (size_t)vsnprintf(s, n+1, fmt, args);

                assert(res == n);
                va_end(args);
            }
        }

    private:
        std::string m_str;

        M_DECL char* grow(size_t n)
        {
            m_str.resize(m_str.size() + n);
            return m_str.data() + m_str.size() - n;
        }
    };
}

#undef M_DECL_PURE
#undef M_DECL

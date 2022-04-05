#ifndef STRING_HPP
#define STRING_HPP

#include "StringView.hpp"

#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>

#define FMT_VARARGS_OBJ(__obj__, __start_arg__) \
{ \
    va_list __args__; \
    va_start(__args__, __start_arg__); \
    __obj__.vsprintf(__start_arg__, __args__); \
    va_end(__args__); \
}

#define FMT_VARARGS(__start_arg__) FMT_VARARGS_OBJ(::ut::FMT, __start_arg__)

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

namespace ut
{
    class Fmt
    {
    public:
#if defined(UT_FMT_BUFFER_SIZE)
        int static constexpr BUFFER_SIZE = UT_FMT_BUFFER_SIZE;
#else
        int static constexpr BUFFER_SIZE = 1024;
#endif

        using buffer_type = std::array<char, BUFFER_SIZE>;

        Fmt();
        static Fmt& instance();

        M_DECL_PURE char const* buffer() const { return m_buffer.data(); }
        M_DECL_PURE int         result() const { return m_result; }

        M_DECL_PURE cstrview view() const
        { return cstrview::explicit_construct_cstr(m_buffer.data(), (size_t)m_result); }

        M_DECL_PURE std::string string() const
        {
            if (m_result > 0)
                return std::string{m_buffer.data(), m_buffer.data() + m_result};
            return std::string{};
        }

        M_DECL std::string string(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return string();
        }

        M_DECL cstrview view(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return view();
        }

        M_DECL int vsprintf(char const* fmt, va_list args)
        {
            m_result = vsnprintf(m_buffer.data(), m_buffer.size(), fmt, args);

            if (m_result < 0)
                m_buffer[0] = '\0';

            return m_result;
        }

        M_DECL int sprintf(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return m_result;
        }

        M_DECL int fscanf(FILE* file)
        {
            m_result = ::fscanf(file, "%1023s ", m_buffer.data());
            return m_result;
        }

        M_DECL cstrview operator() (char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return view();
        }

    private:
        buffer_type m_buffer;
        int         m_result;
    };

    [[maybe_unused]] static Fmt& FMT = Fmt::instance();
}

#undef M_DECL_PURE
#undef M_DECL

#endif // STRING_HPP

#ifndef STRING_HPP
#define STRING_HPP

//
// Preprocessor Options
//

//---- Use ut::cstrview if enabled, otherwise use std::string_view
//#define UT_FMT_USE_CSTRVIEW

//---- Specify string buffer size, default is 512
//#define UT_FMT_BUFFER_SIZE

//---- Specify number of string buffers, default is 8
//#define UT_FMT_BUFFER_COUNT

#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>

#if defined(UT_FMT_USE_CSTRVIEW)
#include "StringView.hpp"
#else
#include <string_view>
#endif

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
#define M_CURRENT_BUFFER        ( m_buffer[m_counter%BUFFER_COUNT] )

namespace ut
{
    class Fmt
    {
    public:
#if defined(UT_FMT_BUFFER_SIZE)
        int static constexpr BUFFER_SIZE = UT_FMT_BUFFER_SIZE;
#else
        int static constexpr BUFFER_SIZE = 512;
#endif

#if defined(UT_FMT_BUFFER_COUNT)
        int static constexpr BUFFER_COUNT = UT_FMT_BUFFER_COUNT;
#else
        int static constexpr BUFFER_COUNT = 8;
#endif

#if defined(UT_FMT_USE_CSTRVIEW)
        using stringview_type = cstrview;
#else
        using stringview_type = std::string_view;
#endif

        using buffer_type           = std::array<char, BUFFER_SIZE>;
        using buffer_container_type = std::array<buffer_type, BUFFER_COUNT>;

        Fmt() noexcept;
        static Fmt& instance() noexcept;

        M_DECL_PURE char const* buffer() const { return M_CURRENT_BUFFER.data(); }
        M_DECL_PURE int         result() const { return m_result; }

#if defined(UT_FMT_USE_CSTRVIEW)
        M_DECL_PURE stringview_type view() const
        { return cstrview::explicit_construct_cstr(M_CURRENT_BUFFER.data(), (size_t)m_result); }
#else
        M_DECL_PURE stringview_type view() const
        { return {M_CURRENT_BUFFER.data(), (size_t)m_result}; }
#endif

        M_DECL_PURE std::string string() const
        {
            if (m_result > 0)
                return std::string{M_CURRENT_BUFFER.data(), M_CURRENT_BUFFER.data() + m_result};
            return std::string{};
        }

        M_DECL std::string string(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = printNextBuffer(fmt, args);
            va_end(args);

            return string();
        }

        M_DECL stringview_type view(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = printNextBuffer(fmt, args);
            va_end(args);

            return view();
        }

        M_DECL stringview_type operator() (char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = printNextBuffer(fmt, args);
            va_end(args);

            return view();
        }

    private:
        buffer_container_type   m_buffer;
        size_t                  m_counter;
        int                     m_result;

        M_DECL int printNextBuffer(char const* fmt, va_list args)
        {
            ++m_counter;
            m_result = vsnprintf(M_CURRENT_BUFFER.data(), M_CURRENT_BUFFER.size(), fmt, args);

            if (m_result < 0)
                M_CURRENT_BUFFER[0] = '\0';

            return m_result;
        }
    };

    [[maybe_unused]] static Fmt& FMT = Fmt::instance();
}

#undef M_DECL_PURE
#undef M_DECL
#undef M_CURRENT_BUFFER

#endif // STRING_HPP

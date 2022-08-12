#ifndef STRING_HPP
#define STRING_HPP

//
// Preprocessor Options
//

//---- Use ut::cstrview if enabled, otherwise use std::string_view
//#define UT_FMT_USE_CSTRVIEW

//---- Specify string buffer size
//#define UT_FMT_BUFFER_SIZE 512

#if !defined(UT_FMT_BUFFER_SIZE)
#define UT_FMT_BUFFER_SIZE 512
#endif

//---- Specify number of string buffers
//#define UT_FMT_BUFFER_COUNT 8

#if !defined(UT_FMT_BUFFER_COUNT)
#define UT_FMT_BUFFER_COUNT 8
#endif

#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <charconv>

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
    template <size_t BufferSize, size_t BufferCount>
    class basic_fmt
    {
    public:
        int static constexpr BUFFER_SIZE  = BufferSize;
        int static constexpr BUFFER_COUNT = BufferCount;

#if defined(UT_FMT_USE_CSTRVIEW)
        using stringview_type = cstrview;
#else
        using stringview_type = std::string_view;
#endif

        using buffer_type           = std::array<char, BUFFER_SIZE>;
        using buffer_container_type = std::array<buffer_type, BUFFER_COUNT>;

        basic_fmt() noexcept
            : m_buffer{}, m_counter{0}, m_result{0}
        {}

        static basic_fmt& instance() noexcept;

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

        M_DECL stringview_type toChars(short        x, int base = 10) { return toCharsInt(x, base); }
        M_DECL stringview_type toChars(int          x, int base = 10) { return toCharsInt(x, base); }
        M_DECL stringview_type toChars(long         x, int base = 10) { return toCharsInt(x, base); }
        M_DECL stringview_type toChars(long long    x, int base = 10) { return toCharsInt(x, base); }

        M_DECL stringview_type toChars(unsigned short       x, int base = 10) { return toCharsInt(x, base); }
        M_DECL stringview_type toChars(unsigned int         x, int base = 10) { return toCharsInt(x, base); }
        M_DECL stringview_type toChars(unsigned long        x, int base = 10) { return toCharsInt(x, base); }
        M_DECL stringview_type toChars(unsigned long long   x, int base = 10) { return toCharsInt(x, base); }

        M_DECL stringview_type toChars(float        x, int precision) { return toCharsFloat(x, precision); }
        M_DECL stringview_type toChars(double       x, int precision) { return toCharsFloat(x, precision); }
        M_DECL stringview_type toChars(long double  x, int precision) { return toCharsFloat(x, precision); }

        M_DECL stringview_type toChars(float        x) { return toCharsFloat(x); }
        M_DECL stringview_type toChars(double       x) { return toCharsFloat(x); }
        M_DECL stringview_type toChars(long double  x) { return toCharsFloat(x); }

    private:
        buffer_container_type   m_buffer;
        size_t                  m_counter =0;
        int                     m_result  =0;

        M_DECL int printNextBuffer(char const* fmt, va_list args)
        {
            ++m_counter;
            m_result = vsnprintf(M_CURRENT_BUFFER.data(), M_CURRENT_BUFFER.size(), fmt, args);

            if (m_result < 0)
                M_CURRENT_BUFFER[0] = '\0';

            return m_result;
        }

        template <typename T>
        M_DECL stringview_type toCharsInt(T x, int base)
        {
            ++m_counter;

            char* first = M_CURRENT_BUFFER.data();
            char* last  = M_CURRENT_BUFFER.data() + M_CURRENT_BUFFER.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x, base); ec == std::errc())
                return cstrview::explicit_construct_cstr(first, std::distance(first, ptr));
            return {};
        }

        template <typename T>
        M_DECL stringview_type toCharsFloat(T x, int precision)
        {
            ++m_counter;

            char* first = M_CURRENT_BUFFER.data();
            char* last  = M_CURRENT_BUFFER.data() + M_CURRENT_BUFFER.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x, std::chars_format::fixed, precision); ec == std::errc())
                return cstrview::explicit_construct_cstr(first, std::distance(first, ptr));
            return {};
        }

        template <typename T>
        M_DECL stringview_type toCharsFloat(T x)
        {
            ++m_counter;

            char* first = M_CURRENT_BUFFER.data();
            char* last  = M_CURRENT_BUFFER.data() + M_CURRENT_BUFFER.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x); ec == std::errc())
                return cstrview::explicit_construct_cstr(first, std::distance(first, ptr));
            return {};
        }


    };

    template <size_t BufferSize, size_t BufferCount>
    basic_fmt<BufferSize, BufferCount>& basic_fmt<BufferSize, BufferCount>::instance() noexcept
    {
        static thread_local basic_fmt<BufferSize, BufferCount> fmt;
        return fmt;
    }

    using fmt = basic_fmt<UT_FMT_BUFFER_SIZE, UT_FMT_BUFFER_COUNT>;

    [[maybe_unused]] static fmt& FMT = fmt::instance();
}

#undef M_DECL_PURE
#undef M_DECL
#undef M_CURRENT_BUFFER

#endif // STRING_HPP

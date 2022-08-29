#ifndef STRING_HPP
#define STRING_HPP

//
// Preprocessor Options
//

//---- Use std::string_view if enabled, otherwise use ut::cstrview
//#define UT_FMT_USE_STL_STRINGVIEW

//---- Specify string buffer size
//#define UT_FMT_BUFFER_SIZE 256

#if !defined(UT_FMT_BUFFER_SIZE)
#define UT_FMT_BUFFER_SIZE 256
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

#if defined(UT_FMT_USE_STL_STRINGVIEW)
#include <string_view>
#else
#include "ut/string/view.hpp"
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
#define M_ARR                   ( m_buffer[m_index%BUFFER_COUNT].arr )
#define M_SZ                    ( m_buffer[m_index%BUFFER_COUNT].sz )

namespace ut
{
    template <size_t BufferSize, size_t BufferCount>
    class basic_format_buffer
    {
    public:
        int static constexpr BUFFER_SIZE  = BufferSize;
        int static constexpr BUFFER_COUNT = BufferCount;

        using string_type = std::string;

#if defined(UT_FMT_USE_STL_STRINGVIEW)
        using view_type = std::string_view;
#else
        using view_type = cstrview;
#endif

        struct buf { std::array<char, BUFFER_SIZE> arr; size_t sz=0; };
        using buffer_container_type = std::array<buf, BUFFER_COUNT>;

        static basic_format_buffer& instance() noexcept;

        M_DECL_PURE char const* data() const { return M_ARR.data(); }
        M_DECL_PURE int         sz  () const { return M_SZ; }

#if defined(UT_FMT_USE_STL_STRINGVIEW)
        M_DECL_PURE view_type view() const
        { return {M_ARR.data(), M_SZ}; }
#else

        M_DECL_PURE view_type view() const
        { return cstrview::make_cstrview(M_ARR.data(), M_SZ); }
#endif

        M_DECL_PURE string_type string() const
        { return string_type{M_ARR.data(), M_ARR.data() + M_SZ}; }

        M_DECL string_type string(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return string();
        }

        M_DECL view_type view(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return view();
        }

        M_DECL view_type operator() (char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            next(fmt, args);
            va_end(args);

            return view();
        }

        //
        // integer format
        //

        M_DECL view_type intView(short     x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(int       x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(long      x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(long long x, int base = 10) { nextInt(x, base); return view(); }

        M_DECL view_type intView(unsigned short     x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(unsigned int       x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(unsigned long      x, int base = 10) { nextInt(x, base); return view(); }
        M_DECL view_type intView(unsigned long long x, int base = 10) { nextInt(x, base); return view(); }

        M_DECL string_type intString(short     x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(int       x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(long      x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(long long x, int base = 10) { nextInt(x, base); return string(); }

        M_DECL string_type intString(unsigned short     x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(unsigned int       x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(unsigned long      x, int base = 10) { nextInt(x, base); return string(); }
        M_DECL string_type intString(unsigned long long x, int base = 10) { nextInt(x, base); return string(); }

        //
        // float format
        //

        M_DECL view_type floatView(float        x, int precision) { nextFloat(x, precision); return view(); }
        M_DECL view_type floatView(double       x, int precision) { nextFloat(x, precision); return view(); }
        M_DECL view_type floatView(long double  x, int precision) { nextFloat(x, precision); return view(); }

        M_DECL view_type floatView(float        x) { nextFloat(x); return view(); }
        M_DECL view_type floatView(double       x) { nextFloat(x); return view(); }
        M_DECL view_type floatView(long double  x) { nextFloat(x); return view(); }

        M_DECL string_type floatString(float        x, int precision) { nextFloat(x, precision); return string(); }
        M_DECL string_type floatString(double       x, int precision) { nextFloat(x, precision); return string(); }
        M_DECL string_type floatString(long double  x, int precision) { nextFloat(x, precision); return string(); }

        M_DECL string_type floatString(float        x) { nextFloat(x); return string(); }
        M_DECL string_type floatString(double       x) { nextFloat(x); return string(); }
        M_DECL string_type floatString(long double  x) { nextFloat(x); return string(); }

    private:
        buffer_container_type   m_buffer    { };
        size_t                  m_index     {0};

        M_DECL void reset()
        {
            M_SZ = 0;
            M_ARR[0] = '\0';
        }

        M_DECL void incrementIndex()
        {
            ++m_index;
            m_index = (m_index % BUFFER_COUNT + BUFFER_COUNT) % BUFFER_COUNT;
        }

        M_DECL void next(char const* fmt, va_list args)
        {
            incrementIndex();

            if (int res = vsnprintf(M_ARR.data(), M_ARR.size(), fmt, args); res >= 0)
                M_SZ = res;
            else
                reset();
        }

        template <typename T>
        M_DECL void nextInt(T x, int base)
        {
            incrementIndex();

            char* first = M_ARR.data();
            char* last  = M_ARR.data() + M_ARR.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x, base); ec == std::errc())
                M_SZ = std::distance(first, ptr);
            else
                reset();
        }

        template <typename T>
        M_DECL void nextFloat(T x, int precision)
        {
            incrementIndex();

            char* first = M_ARR.data();
            char* last  = M_ARR.data() + M_ARR.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x, std::chars_format::fixed, precision); ec == std::errc())
                M_SZ = std::distance(first, ptr);
            else
                reset();
        }

        template <typename T>
        M_DECL void nextFloat(T x)
        {
            incrementIndex();

            char* first = M_ARR.data();
            char* last  = M_ARR.data() + M_ARR.size();

            if (auto [ptr, ec] = std::to_chars(first, last, x); ec == std::errc())
                M_SZ = std::distance(first, ptr);
            else
                reset();
        }


    };

    template <size_t BufferSize, size_t BufferCount>
    basic_format_buffer<BufferSize, BufferCount>& basic_format_buffer<BufferSize, BufferCount>::instance() noexcept
    {
        static thread_local basic_format_buffer<BufferSize, BufferCount> x;
        return x;
    }

    using fmtbuf = basic_format_buffer<UT_FMT_BUFFER_SIZE, UT_FMT_BUFFER_COUNT>;

    [[maybe_unused]] static fmtbuf& FMTBUF = fmtbuf::instance();
}

#undef M_DECL_PURE
#undef M_DECL
#undef M_ARR
#undef M_SZ

#endif // STRING_HPP

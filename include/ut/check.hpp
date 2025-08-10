//
// Created by james on 9/28/23.
//


#pragma once

#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <cstdlib>
#include <type_traits>


#ifdef NDEBUG
#define check_msg(...)      (void)0;
#define nopath(...)         (void)0;
#define nopath_case(x_)     (void)0;
#else
#define check_msg(x_, ...)  do { ::ut::internal::check_expression(x_, __LINE__, #x_, __func__, __FILE__, __VA_ARGS__); }while(0)
#define nopath(...)         do { ::ut::internal::check_impossible(__LINE__, __func__, __FILE__, __VA_ARGS__); }while(0)
#define nopath_case(x_)     do { static_assert(std::is_enum_v<x_>); ::ut::internal::check_impossible(__LINE__, __func__, __FILE__, "BAD ENUM CASE: %s", #x_); }while(0)
#endif

#define check(x_)           check_msg(x_, "SILENT CHECK")
#define check_null(x_)      check_msg((x_) != nullptr, "NULL POINTER")

#define nopath_impl         nopath("UNIMPLEMENTED")
#define nopath_break        nopath("BREAKPOINT")

namespace ut::internal
{
    [[noreturn]] static inline void check_impl(int line, char const* msg, char const* expr, char const* func, char const* file)
    {
        bool m=msg != nullptr, e=expr != nullptr;

        if (m && e)
        {
            fprintf(stderr, "\nASSERTION : %s"
                            "\nEXPR      : %s"
                            "\nFILE      : %s:%d"
                            "\nFUNC      : %s",
                    msg, expr, file, line, func);

        }
        else if (!m && !e)
        {
            fprintf(stderr, "\nASSERTION"
                            "\nFILE      : %s:%d"
                            "\nFUNC      : %s",
                    file, line, func);
        }
        else
        {
            fprintf(stderr, "\nASSERTION : %s"
                            "\nFILE      : %s:%d"
                            "\nFUNC      : %s",
                    m ? msg : expr, file, line, func);
        }

        fflush(stderr);
        abort();
    }

    static inline void check_expression(bool b, int line, char const* expr, char const* func, char const* file, char const* fmt, ...)
    {
        if (!b)
        {
            size_t constexpr MAX_LEN = 512;
            static char buffer[MAX_LEN];

            va_list args;
            va_start(args, fmt);
            bool has_msg = vsnprintf(buffer, MAX_LEN, fmt, args) > 0;
            va_end(args);

            check_impl(line, has_msg ? buffer : nullptr, expr, func, file);
        }
    }

    static inline void check_impossible(int line, char const* func, char const* file, char const* fmt, ...)
    {
        size_t constexpr MAX_LEN = 512;
        static char buffer[MAX_LEN];

        va_list args;
        va_start(args, fmt);
        bool has_msg = vsnprintf(buffer, MAX_LEN, fmt, args) > 0;
        va_end(args);

        check_impl(line, has_msg ? buffer : nullptr, nullptr, func, file);
    }

}

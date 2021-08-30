#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "Fmt.hpp"

#include <exception>

namespace ut
{
    class Exception : public std::exception
    {
    public:
        Exception(std::string const& message)
            : m_message(message)
        {}

        static Exception fmt(char const* fmt, ...)
        {
            Fmt f;
            FMT_VARARGS_OBJ(f, fmt);
            return Exception{ f.string() };
        }

        std::string const& message() const
        {
            return m_message;
        }

        char const* what() const noexcept override
        {
            return m_message.c_str();
        }

    private:
        std::string const& m_message;
    };
}

#endif // EXCEPTION_HPP

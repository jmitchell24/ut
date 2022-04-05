#ifndef STRING_HPP
#define STRING_HPP

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


namespace ut
{
    class Fmt
    {
    public:
        int static constexpr BUFFER_SIZE = 1024;

        using buffer_type = std::array<char, BUFFER_SIZE>;

        Fmt()
            : m_buffer{}, m_result{0}
        {}

        static Fmt& instance();

        [[nodiscard]] inline char const* buffer() const { return m_buffer.data(); }
        [[nodiscard]] inline int result() const { return m_result; }

        [[nodiscard]] inline std::string string() const
        {
            if (m_result > 0)
                return std::string{m_buffer.data(), m_buffer.data() + m_result};
            return std::string{};
        }

        inline std::string string(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return string();
        }

        inline int vsprintf(char const* fmt, va_list args)
        {
            m_result = vsnprintf(m_buffer.data(), m_buffer.size(), fmt, args);

            if (m_result < 0)
                m_buffer[0] = '\0';

            return m_result;
        }

        inline int sprintf(char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return m_result;
        }

        inline int fscanf(FILE* file)
        {
            m_result = ::fscanf(file, "%1023s ", m_buffer.data());
            return m_result;
        }

        inline char const* operator() (char const* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            m_result = vsprintf(fmt, args);
            va_end(args);

            return m_buffer.data();
        }

    private:
        buffer_type m_buffer;
        int         m_result;
    };

    [[maybe_unused]] static Fmt& FMT = Fmt::instance();
}


#endif // STRING_HPP

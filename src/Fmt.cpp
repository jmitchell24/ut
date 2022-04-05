#include <ut/Fmt.hpp>
using namespace ut;

Fmt::Fmt() noexcept
        : m_buffer{}, m_counter{0}, m_result{0}
{}

Fmt& Fmt::instance() noexcept
{
    static thread_local Fmt fmt;
    return fmt;
}


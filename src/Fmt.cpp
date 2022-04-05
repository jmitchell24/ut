#include <ut/Fmt.hpp>
using namespace ut;

Fmt::Fmt()
        : m_buffer{}, m_result{0}
{}

Fmt& Fmt::instance()
{
    static thread_local Fmt fmt;
    return fmt;
}


#include <ut/Fmt.hpp>
using namespace ut;

Fmt& Fmt::instance()
{
    static thread_local Fmt fmt;
    return fmt;
}


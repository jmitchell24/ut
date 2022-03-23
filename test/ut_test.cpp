#include <ut/ut.hpp>
using namespace ut;

#include <fmt/core.h>
using namespace fmt;

int main()
{
    auto output = gulp::exec_to_string("zfs list -Hp");

    print("output: {}\n", output);

    return 0;
}

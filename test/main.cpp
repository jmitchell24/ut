#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

void foo(char const* s)
{
    printf("%s, World\n", s);
}

consteval cstrview parsePrettyFunctionGCC(cstrparam s)
{
    return s;
}

struct Foo
{
    enum Baz { BZ1, BZ2, BZ3 };

    void bar()
    {
        printf("%s", parsePrettyFunctionGCC(__PRETTY_FUNCTION__));
    }
};

int main()
{
    Foo f;
    f.bar();

    return EXIT_SUCCESS;
}

//int main(int argc, char** argv)
//{
//    doctest::Context context;
//    context.applyCommandLine(argc, argv);
//    context.setOption("no-breaks", true);
//    context.setOption("--version", true);
//    context.setOption("--count", true);
//    context.setOption("--list-test-cases", true);
//    context.setOption("--list-test-suites", true);
//    context.setOption("--success", false);
//    context.setOption("--exit", true);

//    const int result = context.run();
//    if (context.shouldExit())
//        return result;
//    return result;
//}

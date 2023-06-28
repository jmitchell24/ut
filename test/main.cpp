#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



//#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <iomanip>
#include <iostream>
using namespace std;

int main()
{
    auto s1 = "foo"_sv;
    auto s2 = "bar"_sv;

    auto d1 = digest32::range(s1);
    auto d2 = digest32::range(s2);

    cout << d1 << endl;
    cout << d2 << endl;
    cout << (d2+d1) << endl;
    cout << (d1+d2) << endl;
    cout << (digest32::range("bar"_sv) + d1) << endl;

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

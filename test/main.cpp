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
    auto c = colors::hsluv::aliceblue();

    cout << "aliceblue: " << c << endl;

    cout << "scheme: " << endl;
    for (auto&& it: c.scheme<10>())
        cout << it << endl;

    cout << "swatch: " << endl;
    for (auto&& it: c.swatch<10>())
        cout << it << endl;

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

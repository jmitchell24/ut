#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



//#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
#include <ut/container/vector_array.hpp>
#include <ut/check.hpp>
using namespace ut;

#include <iomanip>
#include <iostream>
#include <ranges>
using namespace std;

#define DEBUG_PRINT(x_) cout << #x_ << endl; (x_);

enum Foo {};

int main()
{

    auto r = psize(100,100).rect();

    cout << r.fitAspectSize(1.32);

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

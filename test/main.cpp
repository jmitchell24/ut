#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

int main()
{

    hasher h;

    h.putArgs(1,2);

    auto x = h.digest();

    hasher h2(x);

    h.putArgs(3);
    cout << h.digest() << endl;

    h2.putArgs(3);
    cout << h2.digest() << endl;


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

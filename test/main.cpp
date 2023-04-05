#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"


#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <type_traits>
#include <iostream>
#include <iomanip>
#include <set>
#include <new>
using namespace std;


int main()
{
    cout << PRINTER.fmt("{:x<30}\n", "sadf");



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

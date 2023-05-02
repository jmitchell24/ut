#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
using namespace std;

int main()
{
    auto now = ut::local_datetime::now();

    timer::sleep(1_seconds);

    auto now1s = ut::local_datetime::now();


    if (now < now1s)
        printf("now -> %02d:%02d:%02d\n", now.hours(), now.minutes(), now.seconds());
    else
        printf("now1s -> %02d:%02d:%02d\n", now1s.hours(), now1s.minutes(), now1s.seconds());

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

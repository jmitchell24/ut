#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"



//#define UT_PRINTER_USE_FMT
#include <ut/ut.hpp>
using namespace ut;

#include <iomanip>
#include <iostream>
using namespace std;

#define DEBUG_PRINT(x_) cout << #x_ << endl; (x_);

int main()
{
    if (false)
    {
        auto r = rect(psize(100, 100));
        cout << "row: " << endl;
        for (size_t i = 0; i < 10; ++i)
        {
            cout << i << ": " << r.row(10, i, {.inner_pad=5, .outer_pad=10}) << endl;
        }
    }

    if (false)
    {
        auto r = rect(psize(100, 100));
        cout << "col: " << endl;
        for (size_t i = 0; i < 10; ++i)
        {
            cout << i << ": " << r.col(10, i, {.inner_pad=5, .outer_pad=10}) << endl;
        }
    }

    if (false)
    {
        auto r = rect(psize(100, 100));
        cout << "cell: " << endl;
        for (size_t i = 0; i < 10; ++i)
        {
            for (size_t j = 0; j < 10; ++j)
            {
                cout << j << ": " << r.cell(10, 10, i, j, {.inner_pad=5, .outer_pad=10}) << endl;
            }
        }
    }

    //if (false)
    {
        auto r = rect(psize(100, 100));
        cout << "tie: " << endl;

        rect a,b,c;
        r.rowTie({.inner_pad=5}, a,b,c);

        cout << a.psize() << endl;
        cout << b.psize() << endl;
        cout << c.psize() << endl;
    }

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

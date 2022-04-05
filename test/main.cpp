#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define UT_STL_INTEROP
#include <ut/ut.hpp>
using namespace ut;

#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    regionf r(rectf{0,0,800,600});
    r.rect<char>();
    cout << r << endl;


#define PRINT(__x__) cout << #__x__ << " --> " << r.__x__(100,100) << setw(25) << r.__x__(100,100).rect() << endl;

    PRINT(alignTLtoTL)
    PRINT(alignTLtoTR)
    PRINT(alignTLtoBL)
    PRINT(alignTLtoBR)
    cout << endl;
    PRINT(alignTRtoTL)
    PRINT(alignTRtoTR)
    PRINT(alignTRtoBL)
    PRINT(alignTRtoBR)
    cout << endl;
    PRINT(alignBLtoTL)
    PRINT(alignBLtoTR)
    PRINT(alignBLtoBL)
    PRINT(alignBLtoBR)
    cout << endl;
    PRINT(alignBRtoTL)
    PRINT(alignBRtoTR)
    PRINT(alignBRtoBL)
    PRINT(alignBRtoBR)
    cout << endl;
    PRINT(fit)
    cout << endl;
//    PRINT(alignTCtoTC)
//    PRINT(alignTCtoBC)
//    PRINT(alignBCtoTC)
//    PRINT(alignBCtoBC)
//    PRINT(alignLCtoLC)
//    PRINT(alignLCtoRC)
//    PRINT(alignRCtoLC)
//    PRINT(alignRCtoRC)

#undef PRINT
    return EXIT_SUCCESS;
}


//
//
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
//
//    const int result = context.run();
//    if (context.shouldExit())
//        return result;
//    return result;
//}

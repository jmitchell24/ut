#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <ut/ut.hpp>
using namespace ut;

#include <type_traits>
#include <iostream>
#include <iomanip>
#include <set>
#include <new>
using namespace std;

struct A
{
    int val=0;
    explicit A(int i=0) : val{i}  { printf("A::ctor (%d)\n", val); }
    ~A() { printf("A::dtor (%d)\n", val); }
};

namespace ut
{
    template struct stack_array<A,100>;
}



int main()
{
    stack_array<A,100> arr;

    cout << "--> emplace" << endl;
    for (int i = 0; i < 5; ++i)
    {
        arr.emplace(i);
    }

    cout << "--> for loop : " << arr.size() << endl;
    for (auto&& it : arr)
    {
        cout << it.val << endl;
    }

    cout << "--> pop" << endl;
//    arr.pop();
//    arr.pop();
    arr.pop(2);

    cout << "--> clear" << endl;
    arr.clear();

    cout << "--> exit" << endl;
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

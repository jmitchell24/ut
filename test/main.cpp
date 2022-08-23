#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#define UT_FMT_USE_CSTRVIEW
#include <ut/ut.hpp>
using namespace ut;

#include <fmt/core.h>
#include <fmt/color.h>
using namespace fmt;

#include <type_traits>
#include <iostream>
#include <iomanip>
using namespace std;

#include <thread>
#include <functional>

template <typename T> class waitable
{
public:
    using func_type = std::function<T()>;

    waitable(func_type func)
        : m_func{std::move(func)}
    { }

    bool tryGet(T& t)
    {
        m_thread.joinable()
        t = std::move(m_obj);
    }
private:
    std::thread m_thread;
    func_type   m_func;
    T           m_obj;
};



int main()
{
    array<cstrview, 30> arr;
    for (int i = 0; i < arr.size(); ++i)
        arr[i] = FMTBUF.intView(i);

    for (auto&& it : arr)
        cout << it << endl;



    return EXIT_SUCCESS;
}

//int main()
//{
//    using vec10f = vec<float,10>;
//
//    constexpr vec10f v1{{1,2,3,4,5,6,7,8,9,10}};
//    constexpr vec10f v2{};
//    constexpr vec10f v3{1};
//
//
//
//    cout << v1.length() << endl;
//    cout << v2 << endl;
//    cout << v3 << endl;
//    cout << (v2 < v3) << endl;
//
//    for (auto& x : v3)
//        cout << x << endl;
//
//}


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

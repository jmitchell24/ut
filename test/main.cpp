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

int wrap(int i, int sz)
{
    return (i % sz + sz) % sz;
}

class TreePrinter
{
public:
    explicit TreePrinter(std::ostream* os)
        : m_os{os}, m_next{0}, m_prev{0}
    { assert(os != nullptr); }

    ~TreePrinter()
    { assert(m_next == 0); }

    void node(cstrparam s)
    {
        printLine(s);
        m_prev = m_next;
    }

    void push()
    {
        //m_prev = m_next;
        ++m_next;
    }

    void pop()
    {
        assert(m_next > 0);
        //m_prev = m_next;
        --m_next;
    }


private:
    std::ostream* m_os;
    std::ostringstream m_prefix;
    int m_next, m_prev;

    void printLine(cstrparam prefix, cstrparam line, int depth)
    {
        //            char const* head = "├";
        //            char const* node = "│";
        //            char const* tail = "└";
        char const* spaces = "  ";



        for (int i = 0; i < m_next; ++i)
        {
            (*m_os) << spaces;
        }

        (*m_os) << line;
        (*m_os) << "\n";

    }


};

/*

┌─┬┐  ╔═╦╗  ╓─╥╖  ╒═╤╕
│ ││  ║ ║║  ║ ║║  │ ││
├─┼┤  ╠═╬╣  ╟─╫╢  ╞═╪╡
└─┴┘  ╚═╩╝  ╙─╨╜  ╘═╧╛

┌───────────────────┐
│  ╔═══╗ Some Text  │▒
│  ╚═╦═╝ in the box │▒
╞═╤══╩══╤═══════════╡▒
│ ├──┬──┤           │▒
│ └──┴──┘           │▒
└───────────────────┘▒
 ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒

*/

int main()
{

    TreePrinter tp{addressof(cout)};

    tp.node("a");
    tp.push();
    tp.node("+");
    tp.node("-");
    tp.node(".");
    tp.push();
    tp.node("0");
    tp.node("1");
    tp.node("2");
    tp.pop();
    tp.pop();
    tp.node("b");

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


//
// ut
//
#include <ut/test.hpp>
#include <ut/term.hpp>
using namespace ut;

//
// std
//
#include <iostream>
using namespace std;

//
// Tester -> Implementation
//

void Tester::add(TestCase const& tc)
{
    m_testlist.push_back(tc);
}

void Tester::require(TestCase const& tc, char const* expr)
{
    cout << TERM_RESET << TERM_FG_BRIGHT_RED << expr << TERM_RESET << endl;
}

void Tester::runTests()
{

    for (size_t i = 0; i < m_testlist.size(); ++i)
    {
        auto&& it = m_testlist[i];

        TestState ts;
        it.fn(it, ts);

        if (ts.passed())
        {
            tui::tableCell(2, i, "PASSED");
        }
        else
        {
            auto s = ut_printer(
                "FAILED @ line %d: '%s'", ts.line(), ts.expr());
            tui::tableCell(2, i, s.c_str());
        }
    }
}


Tester& Tester::instance()
{
    static Tester x;
    return x;
}


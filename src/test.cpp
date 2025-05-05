
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

    if (tui::beginTable("testlist", 3, 0))
    {
        for (size_t i = 0; i < m_testlist.size(); ++i)
        {
            auto&& it = m_testlist[i];
            tui::tableCell(0, i, "%d", i);
            tui::tableCell(1, i, "%s", it.name.c_str());

            TestState ts;
            it.fn(ts);

            if (ts.passed())
            {
                tui::tableCell(2, i, "PASSED");
            }
            else
            {
                auto s = PRINTER(
                    "FAILED @ line %d: '%s'", ts.line(), ts.expr());
                tui::tableCell(2, i, s.c_str());
            }
        }
        tui::endTable();
    }
}


Tester& Tester::instance()
{
    static Tester x;
    return x;
}


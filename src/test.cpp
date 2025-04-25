
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

void Tester::runTests()
{

    if (tui::beginTable("testlist", 3, 0))
    {
        for (size_t i = 0; i < m_testlist.size(); ++i)
        {
            auto&& it = m_testlist[i];
            tui::tableCell(0, i, "%d", i);
            tui::tableCell(2, i, "%s", it.name.c_str());
            tui::tableCell(1, i, "%d", it.line);
        }
        tui::endTable();
    }
}


Tester& Tester::instance()
{
    static Tester x;
    return x;
}


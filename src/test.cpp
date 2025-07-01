
//
// ut
//
#include <ut/test.hpp>
#include <ut/term.hpp>
using namespace ut;
using namespace ut::test;

//
// std
//
#include <iostream>
using namespace std;

//
// Suite -> Implementation
//

void Suite::printPretty(ostream& os) const
{
    char const* indent = "  ";


    os << TERM_FG_BRIGHT_BLUE "UT" TERM_RESET " unit test results: \n";

    if (tests.empty())
    {

        os << indent << TERM_DIM "no test results available" TERM_RESET;
        return;
    }


    for (auto&& test: tests)
    {
        os
            <<
            TERM_FG_BRIGHT_BLUE "TEST"
            TERM_RESET " '"
            TERM_FG_BRIGHT_YELLOW
            << test.name
            << TERM_RESET "': \n";

        for (auto&& req: test.reqs)
        {

        }

        for (auto&& sec: test.secs)
        {
            os
                << indent <<
                TERM_FG_BRIGHT_BLUE "SECTION "
                TERM_RESET " '"
                TERM_FG_BRIGHT_YELLOW
                << sec.name
                << TERM_RESET "': \n";

            for (auto&& req: sec.reqs)
            {
                if (req.passed())
                {
                    os
                        << indent << indent <<
                        TERM_FG_BRIGHT_MAGENTA << setw(4) << setfill(' ') << req.line <<
                        TERM_FG_BRIGHT_GREEN " PASSED"
                        << TERM_RESET "\n";
                }

                if (req.failed())
                {
                    os
                        << indent << indent <<
                        TERM_FG_BRIGHT_MAGENTA << setw(4) << setfill(' ') << req.line <<
                        TERM_FG_BRIGHT_RED " FAILED"
                        TERM_RESET " '"
                        TERM_FG_BRIGHT_YELLOW
                        << req.expr
                        << TERM_RESET << "'\n";
                }

                if (req.skipped())
                {
                    os
                        << indent << indent <<
                        TERM_FG_BRIGHT_YELLOW "SKIPPED"
                        TERM_RESET " '"
                        TERM_FG_BRIGHT_YELLOW
                        << req.expr
                        << TERM_RESET << "': \n";
                }
            }
        }
    }

    return;
}

void Suite::printJunitXml(std::ostream& os) const
{

}







Suite::Stats Suite::getTestStats(const Test& test) const
{
    Stats stats;

    // Count test-level requirements
    for (auto&& req : test.reqs)
    {
        stats.total++;
        switch (req.state)
        {
            case Require::PASSED: stats.passed++; break;
            case Require::FAILED: stats.failed++; break;
            case Require::SKIPPED: stats.skipped++; break;
        }
    }

    // Count section requirements
    for (auto&& sec : test.secs)
    {
        for (auto&& req : sec.reqs)
        {
            stats.total++;
            switch (req.state)
            {
                case Require::PASSED: stats.passed++; break;
                case Require::FAILED: stats.failed++; break;
                case Require::SKIPPED: stats.skipped++; break;
            }
        }
    }

    return stats;
}

Suite::Stats Suite::getSuiteStats() const
{
    Stats stats;

    for (auto&& test : tests)
    {
        Stats testStats = getTestStats(test);
        stats.total += testStats.total;
        stats.passed += testStats.passed;
        stats.failed += testStats.failed;
        stats.skipped += testStats.skipped;
    }

    return stats;
}


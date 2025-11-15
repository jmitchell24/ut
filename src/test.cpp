
//
// ut
//
#include "ut/test.hpp"

#include <ut/test.hpp>
#include <ut/term.hpp>
#include <ut/term/spinner.hpp>

using namespace ut;
using namespace ut::test;

//
// std
//
#include <iostream>
#include <iomanip>
using namespace std;

//
// runAllTests -> Implementation
//

Suite ut::test::runAllTests()
{
    auto&& reg = impl::Registry::instance();
    auto runlist = reg.runlist();

    cout << "\nRunning All tests...\n";


    Suite s;

    spinParallel(runlist.size(), [&](SpinnerRunner& sr, size_t i)
    {
        auto&& it = runlist[i];
        sr.prefix("running...");
        sr.suffix(it.test.name);

        auto rc = impl::RunContext(it.test);
        it.fn(rc);
        s.tests.push_back(rc.test());

        sr.prefix("done      ");
    });

    return s;
}

//
// Suite -> Implementation
//

void Suite::print(ostream& os, int flags) const
{
    m_print_flags = flags;
    printSuite(os, *this, "", false);
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

#define INDENT "    "

void Suite::printSuite(ostream& os, Suite const& sut, string prefix, bool is_last) const
{
    os
    << prefix
    << TERM_FG_BLUE "SUITE " TERM_RESET
    << "\n";

    if (!sut.tests.empty())
    {
        size_t sz = sut.tests.size()-1;
        for (size_t i = 0; i < sz; ++i)
            printTest(os, sut.tests[i], prefix, false);
        printTest(os, sut.tests[sz], prefix, true);
    }

}

void Suite::printTest(ostream& os, Test const& tst, string prefix, bool is_last) const
{
    os
    << prefix
    << TERM_FG_BLUE "TEST" TERM_RESET
    << " : "
    << TERM_FG_BRIGHT_YELLOW << tst.name << TERM_RESET
    << " : "
    << TERM_FG_BRIGHT_CYAN << tst.file << TERM_RESET
    << "\n";

    if (!tst.reqs.empty())
    {
        size_t sz = tst.reqs.size()-1;
        for (size_t i = 0; i < sz; ++i)
            printRequire(os, tst.reqs[i], prefix + "  ", false);
        printRequire(os, tst.reqs[sz], prefix + "  ", true);
    }

    if (!tst.secs.empty())
    {
        size_t sz = tst.secs.size()-1;
        for (size_t i = 0; i < sz; ++i)
            printSection(os, tst.secs[i], prefix + "  ", false);
        printSection(os, tst.secs[sz], prefix + "  ", true);
    }
}

void Suite::printSection(ostream& os, Section const& seq, string prefix, bool is_last) const
{
    os
    << TERM_FG_BRIGHT_MAGENTA << setw(4) << seq.line << TERM_RESET " "
    << prefix;

    os
    << TERM_FG_BLUE "SECTION" TERM_RESET
    << " : "
    << TERM_FG_BRIGHT_YELLOW << seq.name << TERM_RESET
    << "\n";


    if (!seq.reqs.empty())
    {
        size_t sz = seq.reqs.size()-1;
        for (size_t i = 0; i < sz; ++i)
            printRequire(os, seq.reqs[i], prefix + "  ", false);
        printRequire(os, seq.reqs[sz], prefix + "  ", true);
    }

    if (!seq.secs.empty())
    {
        size_t sz = seq.secs.size()-1;
        for (size_t i = 0; i < sz; ++i)
            printSection(os, seq.secs[i], prefix + "  ", false);
        printSection(os, seq.secs[sz], prefix + "  ", true);
    }
}

void Suite::printRequire(ostream& os, Require const& req, string prefix, bool is_last) const
{
    // skip if PRINT_FAILS flag is not set
    if ( !(m_print_flags&PF_PRINT_FAILS) && req.failed() )
        return;

    // skip if PRINT_PASSES flag is not set
    if ( !(m_print_flags&PF_PRINT_PASSES) && req.passed() )
        return;

    os
    << TERM_FG_BRIGHT_MAGENTA << setw(4) << req.line << TERM_RESET " "
    << prefix;

    printRequireState(os, req.state);

    os
    << " : "
    << req.expr
    << "\n";
}

void Suite::printRequireState(ostream& os, Require::State state) const
{
    switch (state)
    {
        case Require::FAILED: os << TERM_FG_BRIGHT_RED "FAILED" TERM_RESET; break;
        case Require::PASSED: os << TERM_FG_BRIGHT_GREEN "PASSED" TERM_RESET; break;
        case Require::SKIPPED: os << TERM_FG_BRIGHT_YELLOW "SKIPPED" TERM_RESET; break;
        default:nopath_case(Require::State);
    }
}




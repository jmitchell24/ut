#pragma once

//
// std
//
#include <vector>
#include <string>

#define ut_test_context \
    ut::test::impl::RunContext

#define ut_test_register(_line, _file, _label_text, _fn) \
    ut::test::impl::Registry::instance().addRun(_line, _file, _label_text, &run)

//
// test macros
//


#define ut_test_expansion_2(_line, _file, _label_text) \
namespace ut::test::impl \
{ \
    struct Test_##_line { \
        static void run(ut_test_context& _context); \
        Test_##_line() { ut_test_register(_line, _file, _label_text, &run); } \
    } static t_##_line; \
} \
void ut::test::impl::Test_##_line::run(ut_test_context& _context)


#define ut_test_expansion_1(_line, _file, _label_text) \
    ut_test_expansion_2(_line, _file, _label_text)


#define ut_test(_label_text) \
    ut_test_expansion_1(__LINE__, __FILE__, _label_text)


//
// section macros
//


#define ut_section_expansion_2(_line, _label_text) \
    if (auto _guard_##_line = _context.section(_line, _label_text))


#define ut_section_expansion_1(_line, _label_text) \
    ut_section_expansion_2(_line, _label_text)


#define ut_section(_label_text) \
    ut_section_expansion_1(__LINE__, _label_text)


//
// require macros
//

#define ut_require(_expression) \
    { _context.require(__LINE__, #_expression, _expression); }


namespace ut::test
{
    struct Require;
    struct Section;
    struct Test;
    struct Suite;

    using reqslist_type = std::vector<Require>;
    using secslist_type = std::vector<Section>;
    using testlist_type = std::vector<Test>;

    struct Require
    {
        int line=0;
        char const* expr=nullptr;
        enum State { PASSED, FAILED, SKIPPED } state=PASSED;

        inline bool passed() const
        { return state == PASSED; }

        inline bool failed() const
        { return state == FAILED; }

        inline bool skipped() const
        { return state == SKIPPED; }
    };

    struct Section
    {
        int line=0;
        char const* name=nullptr;
        reqslist_type reqs;
    };

    struct Test
    {
        int line=0;
        char const* file=nullptr;
        char const* name=nullptr;
        secslist_type secs;
        reqslist_type reqs;
    };

    struct Suite
    {
        struct Stats { int total=0, passed=0, failed=0, skipped=0; };

        testlist_type tests;

        Stats getTestStats(Test const& test) const;
        Stats getSuiteStats() const;

        void printPretty(std::ostream& os) const;
        void printJunitXml(std::ostream& os) const;
    };



namespace impl
{
    struct Run;
    struct RunContext;
    using runlist_type = std::vector<Run>;
    using runfunc_type = void(*)(RunContext&);
    struct Run { Test test; runfunc_type fn; };

    class Registry
    {
    public:
        static Registry& instance()
        { static Registry x; return x; }

        runlist_type const& runlist() const
        { return m_runlist; }

        void addRun(int line, char const* file, char const* name, runfunc_type fn)
        {
            m_runlist.push_back
            ({
                Test
                {
                    .line=line,
                    .file=file,
                    .name=name,
                    .secs={},
                    .reqs={}
                }, fn
            });
        }

    private:
        Registry()=default;
        runlist_type m_runlist;
    };

    class RunContext
    {
    public:
        struct SectionScopeGuard
        {
            RunContext& rc;
            SectionScopeGuard(Section const& s, RunContext& rc) : rc(rc) { rc.pushSection(s); }
            ~SectionScopeGuard() { rc.popSection(); }
            operator bool() const { return true; }
        };

        RunContext(Test const& test)
            : m_test(test)
        { }

        Test const& test() const
        { return m_test; }

        void require(int line, char const* expr, bool pass)
        {
            auto r = Require
            {
                .line=line,
                .expr=expr,
                .state=pass ? Require::PASSED : Require::FAILED
            };

            auto& reqs = m_idx_seq < 0 ? m_test.reqs : m_test.secs[m_idx_seq].reqs;
            reqs.push_back(r);
        }

        SectionScopeGuard section(int line, char const* name)
        {
            auto s = Section
            {
                .line=line,
                .name=name,
                .reqs={}
            };

            return SectionScopeGuard(s, *this);
        }

        void pushSection(Section const& s)
        {
            m_test.secs.push_back(s);
            m_idx_seq = m_test.secs.size()-1;
        }

        void popSection()
        { --m_idx_seq; }

    private:
        int m_idx_seq=-1;
        Test m_test;
    };
}

    static Suite runAllTests()
    {
        auto runlist = impl::Registry::instance().runlist();

        Suite s;

        for (auto&& it: runlist)
        {
            auto rc = impl::RunContext(it.test);
            it.fn(rc);
            s.tests.push_back(rc.test());
        }

        return s;
    }
}
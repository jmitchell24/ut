#pragma once

//
// ut
//
#include <ut/random.hpp>
#include <ut/time.hpp>

//
// std
//
#include <vector>
#include <string>
#include <unordered_map>
#include <typeindex>

#define ut_test_context \
    ut::test::impl::RunContext

#define ut_test_register(_line, _file, _label_text, _fn) \
    ut::test::impl::Registry::instance().addRun<Test_##_line>(_line, _file, _label_text, &run)

//
// test macros
//

#define ut_test_expansion_2(_line, _file, _label_text) \
namespace ut::test::impl \
{ \
    struct Test_##_line { \
        using _test_type = Test_##_line; \
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
    if (auto _guard_##_line = _context.section(_line, _label_text, Registry::SecTicker<_test_type,_line>::value))


#define ut_section_expansion_1(_line, _label_text) \
    ut_section_expansion_2(_line, _label_text)


#define ut_section(_label_text) \
    ut_section_expansion_1(__LINE__, _label_text)


//
// require macros
//

#define ut_require(_expr) \
    { _context.require(__LINE__, #_expr, _expr, Registry::ReqTicker<_test_type, __LINE__>::value); }


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

        secslist_type secs;
        reqslist_type reqs;
    };

    struct Test : Section
    {
        char const* file=nullptr;
    };

    struct Suite
    {
        enum PrintFlags
        {
            PF_NONE         = 0,
            PF_PRINT_FAILS  = 1,
            PF_PRINT_PASSES = 2,
            PF_ALL = PF_PRINT_FAILS|PF_PRINT_PASSES
        };



        struct Stats { int total=0, passed=0, failed=0, skipped=0; };

        testlist_type tests;



        Stats getTestStats(Test const& test) const;
        Stats getSuiteStats() const;

        void print(std::ostream& os, int flags=PF_ALL) const;
        void printJunitXml(std::ostream& os) const;

    private:
        mutable int m_print_flags=PF_ALL;

        void printSuite(std::ostream& os, Suite const& sut, std::string prefix, bool is_last) const;
        void printTest(std::ostream& os, Test const& tst, std::string prefix, bool is_last) const;
        void printSection(std::ostream& os, Section const& seq, std::string prefix, bool is_last) const;
        void printRequire(std::ostream& os, Require const& req, std::string prefix, bool is_last) const;
        void printRequireState(std::ostream& os, Require::State state) const;

    };

namespace impl
{
    struct Run;
    class RunContext;
    using runlist_type = std::vector<Run>;
    using runfunc_type = void(*)(RunContext&);

    struct Run
    {
        Test test;
        runfunc_type fn;

        int sec_count=0;
        int req_count=0;
    };

    class Registry
    {
    public:
        template<typename,int> struct SecTicker { static int value; };
        template<typename,int> struct ReqTicker { static int value; };

        static Registry& instance()
        { static Registry x; return x; }

        runlist_type const& runlist() const
        { return m_runlist; }

        template <typename T>
        void addRun(int line, char const* file, char const* name, runfunc_type fn)
        {
            Test t;
            t.file = file;
            t.line = line;
            t.name = name;
            t.secs = { };
            t.reqs = { };

            Run r;
            r.test = t;
            r.sec_count = m_secs[typeid(T)];
            r.req_count = m_reqs[typeid(T)];

            m_runlist.push_back({ t, fn });
        }



    private:
        std::unordered_map<std::type_index, int> m_secs;
        std::unordered_map<std::type_index, int> m_reqs;

        runlist_type m_runlist;

        Registry()=default;

        template <typename T> int tickSecs()
        { return ++m_secs[typeid(T)]; }

        template <typename T> int tickReqs()
        { return ++m_reqs[typeid(T)]; }
    };

    template <typename T, int I> int Registry::SecTicker<T,I>::value = instance().tickSecs<T>();
    template <typename T, int I> int Registry::ReqTicker<T,I>::value = instance().tickReqs<T>();



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

        void require(int line, char const* expr, bool pass, int /* req_tick */)
        {
            Require r;
            r.line = line;
            r.expr = expr;
            r.state = pass ? Require::PASSED : Require::FAILED;

            auto& parent = getTopSec();
            parent.reqs.push_back(r);

            timer::sleep(duration::milliseconds(ut_rng.nexti(100)));
        }

        SectionScopeGuard section(int line, char const* name, int /* sec_tick */)
        {
            Section s;
            s.line = line;
            s.name = name;
            s.reqs = { };

            timer::sleep(50_milliseconds);
            return SectionScopeGuard(s, *this);
        }

        void pushSection(Section const& s)
        {
            auto& parent = getTopSec();

            m_indices.push_back(parent.secs.size());
            parent.secs.push_back(s);
        }

        void popSection()
        {
            if (!m_indices.empty())
                m_indices.pop_back();
        }

    private:
        std::vector<int> m_indices;
        Test m_test;

        Section& getTopSec()
        {
            Section* ptr = &m_test;
            for (auto&& it: m_indices)
                ptr = &ptr->secs[it];
            return *ptr;
        }
    };
}

    Suite runAllTests();
}
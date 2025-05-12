#pragma once

//
// std
//
#include <vector>
#include <string>



#define ut_test_expansion_2(_line, _label_text) \
namespace ut::tests \
{ \
    struct I_##_line { \
        static void run(TestState& ts); \
        I_##_line() { ut::Tester::instance().add({_label_text, _line, &run}); } \
    } static i_##_line; \
} \
void ut::tests::I_##_line::run(TestState& ts)

#define ut_test_expansion_1(_line, _label_text) ut_test_expansion_2(_line, _label_text)
#define ut_test(_label_text) ut_test_expansion_1(__LINE__, _label_text)

#define ut_require(_expression) { if (!(_expression)) return ts.fail(__LINE__, #_expression);  }

#define ut_tester ( ut::Tester::instance() )

namespace ut
{
    class TestState
    {
    public:

        inline int line() const
        { return m_line; }

        inline bool passed() const
        { return m_passed; }

        inline char const* expr() const
        { return m_expr; }

        inline void fail(int line, char const* expr)
        { m_line = line; m_passed = false; m_expr = expr; }

    private:
        bool m_passed = true;
        int m_line = 0;
        char const* m_expr;
    };

    struct TestCase
    {
        using fn_type = void(*)(TestState& tc);

        std::string name;
        size_t      line;
        fn_type     fn;
    };



    class Tester
    {
    public:
        using testlist_type = std::vector<TestCase>;

        inline testlist_type const& tests() const
        { return m_testlist; }


        void add(TestCase const&);
        void require(TestCase const& tc, char const* expr);
        void runTests();

        static Tester& instance();

    private:
        testlist_type m_testlist;
    };
}
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
        static void run(); \
        I_##_line() { TESTER.add({_label_text, _line, &run}); } \
    } static i_##_line; \
} \
void ut::tests::I_##_line::run()

#define ut_test_expansion_1(_line, _label_text) ut_test_expansion_2(_line, _label_text)
#define ut_test(_label_text) ut_test_expansion_1(__LINE__, _label_text)

namespace ut
{
    struct TestCase
    {
        using fn_type = void(*)();

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
        void runTests();

        static Tester& instance();

    private:
        testlist_type m_testlist;
    };

    inline Tester& TESTER = Tester::instance();
}
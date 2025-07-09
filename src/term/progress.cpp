//
// Created by james on 08/07/25.
//
#include "ut/term/progress.hpp"

#include <atomic>

#include "ut/term/escapes.hpp"
#include "ut/term/rawterm.hpp"
#include "ut/time.hpp"
using namespace ut;

//
// std
//
#include <iostream>
using namespace std;

void ProgressRunner::progress(task_param task)
{
    atomic_bool done = false;
    size_t delay = 50;

    thread task_thread([&]
    {
        task(*this);
        done = true;
    });

    cout << flush;
    ut_term.enable();

    ut_term << TERM_CURSOR_HIDE TERM_CURSOR_NEXT_LINE(1) TERM_CURSOR_SAVE;

    {
        while (!done)
        {
            ut_term << TERM_CURSOR_RESTORE;
            print();
            timer::sleep(duration::milliseconds(delay));
            m_abort_flag = ut_term.getAbortRequested();
        }

        ut_term << TERM_CURSOR_RESTORE;
        print();
    }

    ut_term << TERM_CURSOR_SHOW TERM_RESET;
    ut_term.disable();

    if (task_thread.joinable())
        task_thread.join();
}

void ProgressRunner::progressParallel(runnerlist_type& runners, task_param_multi task)
{
    atomic_uint done = 0;

    size_t delay = 50;


    vector<thread> task_threads;
    for (size_t i = 0; i < runners.size(); ++i)
    {
        auto&& ptr = runners[i].get();
        task_threads.emplace_back([&done, task, i, ptr]
        {
            task(*ptr, i);
            ++done;
        });
    }

    cout << flush;
    ut_term.enable();

    ut_term << TERM_CURSOR_HIDE TERM_CURSOR_NEXT_LINE(1) TERM_CURSOR_SAVE;

    {
        bool abort_requested = false;

        while (done < runners.size())
        {

            ut_term << TERM_CURSOR_RESTORE;

            for (size_t i = 0; i < runners.size(); ++i)
            {
                auto&& it = *runners[i];
                it.print();
                it.m_abort_flag = abort_requested;
            }

            timer::sleep(duration::milliseconds(delay));
            abort_requested |= ut_term.getAbortRequested();
        }

        ut_term << TERM_CURSOR_RESTORE;
        for (size_t i = 0; i < runners.size(); ++i)
            runners[i]->print();
    }

    ut_term << TERM_CURSOR_SHOW TERM_RESET;
    ut_term.disable();

    for (auto&& it: task_threads)
        if (it.joinable())
            it.join();
}

double rangeHelper(double old_min, double old_max, double new_min, double new_max, double value)
{
    double old_range = old_max - old_min;
    double new_range = new_max - new_min;
    return ((value - old_min) / old_range) * new_range + new_min;
}

void ProgressRunner::print()
{

    size_t bar_size = 50;
    size_t bar_size_on;
    size_t bar_size_off;


    int pv = progressValue();
    bar_size_on = rangeHelper(1, 100, 0, bar_size, pv);
    bar_size_off = bar_size - bar_size_on;


    ostringstream oss;

    oss << m_bar.start << TERM_RESET;

    if (bar_size_on == 0)
    {
        for (size_t i = 0; i < bar_size; ++i) oss << m_bar.off;
    }
    else if (bar_size_on == bar_size)
    {
        for (size_t i = 0; i < bar_size; ++i) oss << m_bar.on;
    }
    else
    {
        for (size_t i = 0; i < bar_size_on-1; ++i) oss << m_bar.on;
        oss << TERM_RESET << m_bar.tip << TERM_RESET;
        for (size_t i = 0; i < bar_size_off; ++i) oss << m_bar.off;
    }

    oss << TERM_RESET << m_bar.end;

    string text = oss.str();
    string pre = prefix();
    string suf = suffix();

    ut_term
        << TERM_CLEAR_LINE
        << pre
        << TERM_RESET " "
        << text
        << TERM_RESET " "
        << suf
        << TERM_RESET TERM_CURSOR_NEXT_LINE(1);

}
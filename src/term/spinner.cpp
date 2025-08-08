//
// Created by james on 16/05/25.
//

//
// ut
//
#include "ut/term/spinner.hpp"

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


//
// SpinnerRunner Implementation
//

void SpinnerRunner::spin(task_param task)
{
    atomic_bool done = false;

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
            advanceFrame();
            timer::sleep(duration::milliseconds(m_spinner.interval));
            m_abort_flag = ut_term.getAbortRequested();
        }

        ut_term << TERM_CURSOR_RESTORE;
        m_frame=0;
        advanceFrame();
    }

    ut_term << TERM_CURSOR_SHOW TERM_RESET;
    ut_term.disable();

    if (task_thread.joinable())
        task_thread.join();
}

void SpinnerRunner::advanceFrame()
{
    int frame = m_frame;

    if (frame < 0)
    {
        ut_term
            << TERM_CLEAR_LINE
            << prefix()
            << m_spinner.frames[0]
            << TERM_RESET " "
            << suffix()
            << TERM_RESET TERM_CURSOR_NEXT_LINE(1);
    }
    else
    {
        ut_term
        << TERM_CLEAR_LINE
        << prefix()
        << m_spinner.frames[frame]
        << TERM_RESET " "
        << suffix()
        << TERM_RESET TERM_CURSOR_NEXT_LINE(1);

        m_frame = (frame + 1) % m_spinner.frames.size();
    }
}

void SpinnerRunner::spinParallel(runnerlist_type& runners, task_param_multi task)
{
    atomic_uint done = 0;
    size_t delay = 99999;

    vector<thread> task_threads;
    for (size_t i = 0; i < runners.size(); ++i)
    {
        auto&& ptr = runners[i].get();

        ptr->m_frame = static_cast<int>(i % ptr->m_spinner.frames.size());
        if (auto it_delay = ptr->m_spinner.interval; it_delay < delay)
            delay = it_delay;

        task_threads.emplace_back([&done, task, i, ptr]
        {
            task(*ptr, i);
            ptr->m_frame = -1;
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
                it.advanceFrame();
                it.m_abort_flag = abort_requested;
            }

            timer::sleep(duration::milliseconds(delay));
            abort_requested |= ut_term.getAbortRequested();
        }

        ut_term << TERM_CURSOR_RESTORE;
        for (size_t i = 0; i < runners.size(); ++i)
        {
            auto&& it = *runners[i];
            it.m_frame=0;
            it.advanceFrame();
        }
    }

    ut_term << TERM_CURSOR_SHOW TERM_RESET;
    ut_term.disable();



    for (auto&& it: task_threads)
        if (it.joinable())
            it.join();
}

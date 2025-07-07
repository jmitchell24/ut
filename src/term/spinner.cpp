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
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
using namespace std;

//
// Threadpool
//



//
// SpinnerRunner Implementation
//

void SpinnerRunner::spin(task_param task)
{

    atomic_bool done = false;
    m_frame = 0;

    thread spin_thread([&]
    {
        cout << endl << TERM_CURSOR_HIDE << flush;

        while (!done)
        {
            cout
                << TERM_CURSOR_COLUMN(0) TERM_CLEAR_LINE
                << prefix()
                << m_spinner.frames[m_frame]
                << TERM_RESET " "
                << suffix()
                << TERM_RESET
                << flush;

            m_frame = (m_frame + 1) % m_spinner.frames.size();

            timer::sleep(duration::milliseconds(m_spinner.interval));
        }
        // Clear the spinner character when done
        cout << TERM_CURSOR_SHOW TERM_CURSOR_COLUMN(0) TERM_CLEAR_LINE << flush;
    });

    task(*this);

    done = true;

    if (spin_thread.joinable())
        spin_thread.join();
}

void SpinnerRunner::advanceFrame()
{
    ut_term
        << TERM_CLEAR_LINE
        << prefix()
        << m_spinner.frames[m_frame]
        << TERM_RESET " "
        << suffix()
        << TERM_RESET TERM_CURSOR_NEXT_LINE(1);

    m_frame = (m_frame + 1) % m_spinner.frames.size();
}

void SpinnerRunner::spinMulti(runnerlist_type& runners, task_param_multi task)
{
    atomic_uint done = 0;

    size_t cnt = runners.size();
    size_t delay = 99999;

    vector<thread> task_threads;
    for (size_t i = 0; i < cnt; ++i)
    {
        auto&& ptr = runners[i].get();

        ptr->m_frame = static_cast<int>(i % ptr->m_spinner.frames.size());
        if (auto it_delay = ptr->m_spinner.interval; it_delay < delay)
            delay = it_delay;

        task_threads.emplace_back([&done, task, i, cnt, ptr]
        {
            task(*ptr, i, cnt);
            ++done;
        });
    }

    cout << flush;
    ut_term.enable();

    ut_term << TERM_CURSOR_HIDE TERM_CURSOR_NEXT_LINE(1) TERM_CURSOR_SAVE;

    {
        while (done < runners.size())
        {
            ut_term << TERM_CURSOR_RESTORE;
            for (size_t i = 0; i < runners.size(); ++i)
            {
                auto&& it = *runners[i];
                it.advanceFrame();
            }

            if (ut_term.getAbortRequested())
                break;

            timer::sleep(duration::milliseconds(delay));
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
        it.join();
}

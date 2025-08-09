//
// Created by james on 04/04/25.
//

#include "ut/term/rawterm.hpp"
#include "ut/check.hpp"
using namespace ut;

//
// std
//
#include <iostream>
using namespace std;

#include "ut/term/tui.hpp"
#include "tui_table.hpp"
using namespace ut;

Table g_table;

struct TableState
{
    std::ostream* os = nullptr;
    bool enabled=false;
    int x=-1;
    int y=-1;
    std::string style;
} g_state;

#define _enable_guard if (!g_state.enabled) { return; }

string sanitize(strparam s)
{
    string res;
    for (auto&& it: s)
        if (isprint(it))
            res += it;
    return res;
}

bool table::begin()
{
    if (g_state.enabled)
        return false;

    g_table = Table();
    g_state = TableState();
    g_state.enabled = true;
    return true;
}



void table::end()
{
    _enable_guard

    g_table.print(cout);
    g_state.enabled=false;
}

void table::title(strparam title)
{
    _enable_guard

    g_table.title = title.str();
}

void table::header(strparam text, int x)
{
    _enable_guard

    g_table.setHeader(x, text.str());
}

void table::cell(strparam text)
{
    _enable_guard

    if (g_state.x < 0 || g_state.y < 0) return;

    g_table.setCell(g_state.x, g_state.y, sanitize(text), g_state.style);
}



void table::style(char const* style)
{
    _enable_guard

    g_state.style = style;
}

void table::setCur(int x, int y)
{
    _enable_guard

    if (x >= 0) g_state.x = x;
    if (y >= 0) g_state.y = y;
}

void table::nextColumn()
{
    _enable_guard

    ++g_state.x;
}

void table::nextRow()
{
    _enable_guard

    ++g_state.y;
    g_state.x = -1;
}
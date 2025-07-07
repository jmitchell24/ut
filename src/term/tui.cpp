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
using namespace ut::tui;

Table g_table;
bool g_table_flag=false;

bool ut::tui::beginTable(char const* name, int column_count, int table_flags)
{
    if (g_table_flag)
        return false;

    g_table = Table(name, column_count);
    g_table_flag = true;
    return true;
}

void ut::tui::endTable()
{
    if (g_table_flag)
    {
        g_table.print(cout);
        g_table_flag = false;
    }
}

void ut::tui::tableHeader(char const* name, int index)
{
    //nopath_impl;
}

string sanitize(char const* s, size_t n)
{
    string res;
    for (size_t i = 0; i < n; ++i)
        if (isprint(s[i]))
            res += s[i];
    return res;
}

void ut::tui::tableCell(int x, int y, char const* fmt, ...)
{
    array<char, 1000> buffer{};

    va_list args;
    va_start(args, fmt);

    int res = vsnprintf(buffer.data(), buffer.size(), fmt, args);
    check_msg(res >= 0 && res < (int)buffer.size(), "format failed, this is YOUR fault!");

    va_end(args);

    g_table.setCell(x, y, sanitize(buffer.data(), res));

}
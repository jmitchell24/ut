//
// Created by james on 04/04/25.
//

#include "ut/term/rawterm.hpp"
#include "ut/check.hpp"
using namespace ut;

//
// std
//
using namespace std;











#include "ut/term/tui.hpp"
#include "tui_table.hpp"
using namespace ut::tui;

Table g_table;

bool ut::tui::beginTable(char const* name, int column_count, int table_flags)
{
    g_table = Table(name, column_count);
    return true;
}

void ut::tui::endTable()
{
    g_table.print();
}

void ut::tui::tableHeader(char const* name, int index)
{
    //nopath_impl;
}

void ut::tui::tableCell(int x, int y, char const* fmt, ...)
{
    array<char, 1000> buffer{};

    va_list args;
    va_start(args, fmt);

    int res = vsnprintf(buffer.data(), buffer.size(), fmt, args);
    check_msg(res >= 0 && res < (int)buffer.size(), "format failed, this is YOUR fault!");

    va_end(args);

    g_table.setCell(x, y, string(buffer.data(), res));

}
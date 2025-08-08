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
bool g_table_flag=false;
std::string g_table_cell_style = TERM_RESET;

string sanitize(char const* s, size_t n)
{
    string res;
    for (size_t i = 0; i < n; ++i)
        if (isprint(s[i]))
            res += s[i];
    return res;
}

bool ut::beginTable(strparam name)
{
    if (g_table_flag)
        return false;

    g_table = Table();
    g_table.title = name.str();
    g_table_flag = true;
    return true;
}

void ut::endTable()
{
    if (g_table_flag)
    {
        g_table.print(cout);
        g_table_flag = false;
    }
}

void ut::tableHeader(int x, strparam text)
{
    if (g_table_flag)
    {
        g_table.setHeader(x, text.str());
    }
}

void ut::tableCell(int x, int y, char const* fmt, ...)
{
    if (g_table_flag)
    {
        array<char, 1000> buffer{};

        va_list args;
        va_start(args, fmt);

        int res = vsnprintf(buffer.data(), buffer.size(), fmt, args);
        check_msg(res >= 0 && res < static_cast<int>(buffer.size()), "format failed, this is YOUR fault!");

        va_end(args);

        g_table.setCell(x, y, sanitize(buffer.data(), res), g_table_cell_style);
        g_table_cell_style = TERM_RESET;
    }
}

void ut::tableStyle(char const* style)
{
    g_table_cell_style = style;
}

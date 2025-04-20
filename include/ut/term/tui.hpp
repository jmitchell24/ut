//
// Created by james on 20/04/25.
//

#pragma once

namespace ut::tui
{
    bool beginTable(char const* name, int column_count, int table_flags);
    void endTable();
    void tableHeader(char const* name, int index);
    void tableCell(int x, int y, char const* fmt, ...);
}

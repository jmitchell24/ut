//
// Created by james on 20/04/25.
//

#pragma once

#include "ut/string.hpp"
namespace ut
{
    bool beginTable(strparam title = ""_sv);
    void endTable();

    void tableHeader(int x, strparam text);
    void tableCell(int x, int y, char const* fmt, ...);
    void tableStyle(char const* style);
}

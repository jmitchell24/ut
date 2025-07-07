//
// Created by james on 19/04/25.
//

//
// ut
//

#include "ut/check.hpp"
using namespace ut;

#include "tui_table.hpp"
using namespace ut::tui;

//
// std
//
#include <iostream>
#include <iomanip>
using namespace std;

void Table::setCell(size_t x, size_t y, std::string const& text, char const* styles)
{
    check_msg(x < m_width, "x is too big");

    if (y >= m_height)
    {
        m_height = y+1;
        m_cells.resize(m_width * m_height);
        m_cell_widths.resize(m_width);
    }

    auto&& it = m_cells[y * m_width + x];
    it.text = text;
    it.styles = styles;

    if (auto&& sz = m_cell_widths[x]; sz < it.text.size())
        sz = it.text.size();
}

TableCell const& Table::cell(size_t x, size_t y) const
{
    check_msg(x < m_width, "x is too big");
    check_msg(y < m_height, "y is too big");

    return m_cells[y * m_width + x];
}

size_t Table::getCellWidthsSum() const
{
    size_t acc=0;
    for (auto&& it: m_cell_widths)
        acc+=it;
    return acc;
}


void Table::print(ostream& os) const
{
    // draw title

    if (!title.empty() )
    {
        auto sz = m_cell_widths.empty() ? 0 : getCellWidthsSum() + m_cell_widths.size()*3 - 1;

        os << box_chars.tl;

        for (size_t i = 0; i < sz; ++i)
            os << box_chars.hz;

        os
            << box_chars.tr
            << "\n"
            << box_chars.vt
            << left
            << setw(sz)
            << title
            << box_chars.vt
            << "\n"
            << box_chars.vtr;
    }
    else
    {
        os << box_chars.tl;
    }

    // draw top border

    for (auto&& it: m_cell_widths)
    {
        for (size_t i = 0; i < it; ++i)
            os << (box_chars.hz);

        os
            << box_chars.hz
            << box_chars.hz
            << box_chars.hzd;
    }

    os
        << "\b"
        << ( title.empty() ? box_chars.tr : box_chars.vtl )
        << "\n";


    // draw cells

    for (size_t j = 0; j < m_height; ++j)
    {
        os << box_chars.vt;
        for (size_t i = 0; i < m_width; ++i)
        {
            auto&& it = cell(i, j);

            os
                << " " TERM_RESET
                << it.styles
                << left
                << setw(m_cell_widths[i])
                << it.text
                << " " TERM_RESET
                << box_chars.vt;
        }
        os << "\n";
    }

    // draw bottom border

    os << (box_chars.bl);
    for (auto&& it: m_cell_widths)
    {
        for (size_t i = 0; i < it; ++i)
            os << (box_chars.hz);
        os << (box_chars.hz);
        os << (box_chars.hz);
        os << (box_chars.hzu);
    }

    os
        << "\b"
        << box_chars.br
        << "\n";
}

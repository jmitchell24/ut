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
using namespace std;

void draw_box(int width, int height, BoxChars box_chars = {})
{
    if (width < 2 || height < 2)
    {
        printf("Box dimensions must be at least 2x2\n");
        return;
    }

    // Box drawing characters


    // Draw top border
    printf("%s", box_chars.tl);
    for (int i = 0; i < width - 2; i++)
    {
        printf("%s", box_chars.hz);
    }
    printf("%s\n", box_chars.tr);

    // Draw middle rows
    for (int i = 0; i < height - 2; i++) {
        printf("%s", box_chars.vt);
        for (int j = 0; j < width - 2; j++) {
            printf(" ");
        }
        printf("%s\n", box_chars.vt);
    }

    // Draw bottom border
    printf("%s", box_chars.bl);
    for (int i = 0; i < width - 2; i++) {
        printf("%s", box_chars.hz);
    }
    printf("%s\n", box_chars.br);
}

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


void Table::print() const
{
    // draw title

    if (!title.empty() )
    {

        auto sz = m_cell_widths.empty() ? 0 : getCellWidthsSum() + m_cell_widths.size()*3 - 1;
        puts(box_chars.tl);
        for (size_t i = 0; i < sz; ++i)
            puts(box_chars.hz);
        puts(box_chars.tr);
        puts("\n");

        puts(box_chars.vt);
        printf(" %*s", 1-(int)sz, title.c_str());
        puts(box_chars.vt);
        printf("\n");
        puts(box_chars.vtr);
    }
    else
    {
        puts(box_chars.tl);
    }

    // draw top border

    for (auto&& it: m_cell_widths)
    {
        for (size_t i = 0; i < it; ++i)
            puts(box_chars.hz);
        puts(box_chars.hz);
        puts(box_chars.hz);
        puts(box_chars.hzd);
    }
    printf("\b%s\n", title.empty() ? box_chars.tr : box_chars.vtl);


    // draw cells

    for (size_t j = 0; j < m_height; ++j)
    {
        puts(box_chars.vt);
        for (size_t i = 0; i < m_width; ++i)
        {
            auto&& it = cell(i, j);
            printf(" %s%*s %s" TERM_RESET,
                it.styles.c_str(),
                static_cast<int>(m_cell_widths[i]),
                it.text.c_str(),
                box_chars.vt);
        }
        puts("\n");
    }

    // draw bottom border

    puts(box_chars.bl);
    for (auto&& it: m_cell_widths)
    {
        for (size_t i = 0; i < it; ++i)
            puts(box_chars.hz);
        puts(box_chars.hz);
        puts(box_chars.hz);
        puts(box_chars.hzu);
    }
    printf("\b%s\n", box_chars.br);
}

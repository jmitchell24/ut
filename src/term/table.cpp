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
#include <iomanip>
using namespace std;

#include "ut/term/table.hpp"
using namespace ut;

string sanitize(strparam s)
{
    string res;
    for (auto&& it: s)
        if (isprint(it))
            res += it;
    return res;
}

//
// Table -> Implementation
//

void Table::reset()
{
    m_width=0;
    m_height=0;
    m_cells.clear();
    m_widths.clear();
    m_headers.clear();
}

size_t Table::getCellIndex(size_t x, size_t y) const
{
    return y * m_width + x;
}

void Table::expandTo(size_t width, size_t height)
{
    if (width <= m_width && height <= m_height)
        return;

    size_t new_width = max(width, m_width);
    size_t new_height = max(height, m_height);

    // Create new cell vector
    clist_type new_cells(new_width * new_height);

    // Copy existing cells
    for (size_t y = 0; y < m_height; ++y)
    {
        for (size_t x = 0; x < m_width; ++x)
        {
            size_t old_idx = y * m_width + x;
            size_t new_idx = y * new_width + x;
            new_cells[new_idx] = m_cells[old_idx];
        }
    }

    // Update dimensions and cells
    m_width = new_width;
    m_height = new_height;
    m_cells = move(new_cells);

    // Expand widths and headers vectors
    m_widths.resize(m_width, 0);
    m_headers.resize(m_width, {});
}

Table::Cell& Table::getCell(size_t x, size_t y)
{
    expandTo(x + 1, y + 1);
    return m_cells[getCellIndex(x, y)];
}

Table::Cell& Table::getHeader(size_t x)
{
    if (x >= m_headers.size())
    {
        m_headers.resize(x + 1, {});
        m_widths.resize(x + 1, 0);
        m_width = max(m_width, x + 1);
    }
    return m_headers[x];
}

void Table::setCell(size_t x, size_t y, Cell const& c)
{
    getCell(x, y) = c;
    updateColumnWidths();
}

void Table::setHeader(size_t x, Cell const& c)
{
    getHeader(x) = c;
    updateColumnWidths();
}

void Table::updateColumnWidths()
{
    // Reset widths
    fill(m_widths.begin(), m_widths.end(), 0);

    // Check header widths
    for (size_t x = 0; x < m_headers.size(); ++x)
    {
        if (x < m_widths.size())
            m_widths[x] = max(m_widths[x], m_headers[x].text.length());
    }

    // Check cell widths
    for (size_t y = 0; y < m_height; ++y)
    {
        for (size_t x = 0; x < m_width; ++x)
        {
            auto&& cell = m_cells[getCellIndex(x, y)];
            m_widths[x] = max(m_widths[x], cell.text.length());
        }
    }
}

size_t Table::getCellWidthsSum() const
{
    size_t acc = 0;
    for (auto&& width : m_widths)
        acc += width;
    return acc;
}

void Table::print(ostream& os, BoxChars const& box_chars) const
{
    if (m_width == 0 || (m_height == 0 && m_headers.empty()))
    {
        // Empty table
        os << box_chars.tl << box_chars.tr << "\n"
           << box_chars.bl << box_chars.br << "\n";
        return;
    }

    // Create working copy of column widths
    wlist_type working_widths = m_widths;

    // Calculate minimum table width based on columns
    size_t min_table_width = 0;
    if (m_width > 0)
    {
        for (size_t w : working_widths) min_table_width += w;
        min_table_width += (m_width * 2) + (m_width + 1); // padding + borders
    }
    else
    {
        min_table_width = 4; // Minimum for empty table
    }

    // If title is present and longer than table, expand column widths
    if (!title.text.empty())
    {
        size_t title_needed_width = title.text.length() + 4; // +4 for "│ " and " │"
        if (title_needed_width > min_table_width && m_width > 0)
        {
            size_t extra_width = title_needed_width - min_table_width;
            size_t width_per_column = extra_width / m_width;
            size_t remainder = extra_width % m_width;

            // Distribute extra width among columns
            for (size_t i = 0; i < m_width; ++i)
            {
                working_widths[i] += width_per_column;
                if (i < remainder) working_widths[i]++;
            }
        }
    }

    // Recalculate content width with working widths
    size_t content_width = 0;
    if (m_width > 0)
    {
        for (size_t w : working_widths) content_width += w;
        content_width += (m_width * 2) + (m_width + 1); // padding + borders
    }
    else
    {
        content_width = 4;
    }

    // Draw title if present
    if (!title.text.empty())
    {
        os << box_chars.tl;
        for (size_t i = 0; i < content_width - 2; ++i)
            os << box_chars.hz;
        os << box_chars.tr << "\n";

        // Apply title styles and properly format
        os << box_chars.vt << " " << title.styles << left << setw(content_width - 4)
           << title.text << TERM_RESET << " " << box_chars.vt << "\n";

        // Title separator
        os << box_chars.vtr;
        for (size_t i = 0; i < m_width; ++i)
        {
            for (size_t j = 0; j < working_widths[i] + 2; ++j)
                os << box_chars.hz;
            if (i < m_width - 1)
                os << box_chars.hzd;
        }
        os << box_chars.vtl << "\n";
    }
    else
    {
        // Top border
        os << box_chars.tl;
        for (size_t i = 0; i < m_width; ++i)
        {
            for (size_t j = 0; j < working_widths[i] + 2; ++j)
                os << box_chars.hz;
            if (i < m_width - 1)
                os << box_chars.hzd;
        }
        os << box_chars.tr << "\n";
    }

    // Draw headers if present
    bool has_headers = false;
    for (auto&& header : m_headers)
    {
        if (!header.text.empty())
        {
            has_headers = true;
            break;
        }
    }

    if (has_headers)
    {
        os << box_chars.vt;
        for (size_t i = 0; i < m_width; ++i)
        {
            const auto& header = (i < m_headers.size()) ? m_headers[i] : Cell{};
            os << " " << header.styles << left << setw(working_widths[i])
               << header.text << TERM_RESET << " " << box_chars.vt;
        }
        os << "\n";

        // Header separator
        os << box_chars.vtr;
        for (size_t i = 0; i < m_width; ++i)
        {
            for (size_t j = 0; j < working_widths[i] + 2; ++j)
                os << box_chars.hz;
            if (i < m_width - 1)
                os << box_chars.cross;
        }
        os << box_chars.vtl << "\n";
    }

    // Draw data rows
    for (size_t y = 0; y < m_height; ++y)
    {
        os << box_chars.vt;
        for (size_t x = 0; x < m_width; ++x)
        {
            const auto& cell = m_cells[getCellIndex(x, y)];
            os << " " << cell.styles << left << setw(working_widths[x])
               << cell.text << TERM_RESET << " " << box_chars.vt;
        }
        os << "\n";
    }

    // Bottom border
    os << box_chars.bl;
    for (size_t i = 0; i < m_width; ++i)
    {
        for (size_t j = 0; j < working_widths[i] + 2; ++j)
            os << box_chars.hz;
        if (i < m_width - 1)
            os << box_chars.hzu;
    }
    os << box_chars.br << "\n";
}
//
// TableBuilder -> Implementation
//

void TableBuilder::reset()
{
    m_table.reset();
    m_x=-1;
    m_y=-1;
}

void TableBuilder::title(strparam text, strparam styles)
{
    m_table.title = {text.str(),styles.str()};
}

void TableBuilder::header(int x, strparam text, strparam styles)
{
    m_table.setHeader(x, {text.str(), styles.str()});
}

void TableBuilder::content(strparam text, strparam styles)
{
    if (m_x < 0 || m_y < 0) return;
    m_table.setCell(m_x, m_y, {text.str(), styles.str()});
}

void TableBuilder::cell(int x, int y) { m_x = x; m_y = y; }
void TableBuilder::column(int x) { m_x = x; }
void TableBuilder::row(int y) { m_y = y;  }
void TableBuilder::nextColumn(){ ++m_x; }
void TableBuilder::nextRow() { m_x=-1; ++m_y; }
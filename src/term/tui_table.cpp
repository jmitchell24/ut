//
// Created by james on 19/04/25.
//

//
// ut
//

#include "ut/check.hpp"
using namespace ut;

#include "tui_table.hpp"
using namespace ut;

//
// std
//
#include <iostream>
#include <iomanip>
using namespace std;




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
    m_headers.resize(m_width, "");
}

TableCell& Table::getCell(size_t x, size_t y)
{
    expandTo(x + 1, y + 1);
    return m_cells[getCellIndex(x, y)];
}

string& Table::getHeader(size_t x)
{
    if (x >= m_headers.size())
    {
        m_headers.resize(x + 1, "");
        m_widths.resize(x + 1, 0);
        m_width = max(m_width, x + 1);
    }
    return m_headers[x];
}

void Table::setCell(size_t x, size_t y, string const& text, string const& styles)
{
    auto& cell = getCell(x, y);
    cell.text = text;
    cell.styles = styles;
    updateColumnWidths();
}

void Table::setHeader(size_t x, string const& text)
{
    getHeader(x) = text;
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
            m_widths[x] = max(m_widths[x], m_headers[x].length());
    }

    // Check cell widths
    for (size_t y = 0; y < m_height; ++y)
    {
        for (size_t x = 0; x < m_width; ++x)
        {
            const auto& cell = m_cells[getCellIndex(x, y)];
            m_widths[x] = max(m_widths[x], cell.text.length());
        }
    }
}

size_t Table::getCellWidthsSum() const
{
    size_t acc = 0;
    for (const auto& width : m_widths)
        acc += width;
    return acc;
}

void Table::print(ostream& os) const
{
    if (m_width == 0 || (m_height == 0 && m_headers.empty()))
    {
        // Empty table
        os << box_chars.tl << box_chars.tr << "\n"
           << box_chars.bl << box_chars.br << "\n";
        return;
    }

    // Calculate total table width (content + borders + padding)
    size_t content_width = getCellWidthsSum() + (m_width > 0 ? (m_width - 1) * 3 + 4 : 0);

    // Draw title if present
    if (!title.empty())
    {
        os << box_chars.tl;
        for (size_t i = 0; i < content_width - 2; ++i)
            os << box_chars.hz;
        os << box_chars.tr << "\n";

        os << box_chars.vt << " " << left << setw(content_width - 4) << title
           << " " << box_chars.vt << "\n";

        // Title separator
        os << box_chars.vtr;
        for (size_t i = 0; i < m_width; ++i)
        {
            for (size_t j = 0; j < m_widths[i] + 2; ++j)
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
            for (size_t j = 0; j < m_widths[i] + 2; ++j)
                os << box_chars.hz;
            if (i < m_width - 1)
                os << box_chars.hzd;
        }
        os << box_chars.tr << "\n";
    }

    // Draw headers if present
    bool has_headers = false;
    for (const auto& header : m_headers)
    {
        if (!header.empty())
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
            os << " " << left << setw(m_widths[i])
               << (i < m_headers.size() ? m_headers[i] : "")
               << " " << box_chars.vt;
        }
        os << "\n";

        // Header separator
        os << box_chars.vtr;
        for (size_t i = 0; i < m_width; ++i)
        {
            for (size_t j = 0; j < m_widths[i] + 2; ++j)
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
            os << " " << cell.styles << left << setw(m_widths[x])
               << cell.text << TERM_RESET << " " << box_chars.vt;
        }
        os << "\n";
    }

    // Bottom border
    os << box_chars.bl;
    for (size_t i = 0; i < m_width; ++i)
    {
        for (size_t j = 0; j < m_widths[i] + 2; ++j)
            os << box_chars.hz;
        if (i < m_width - 1)
            os << box_chars.hzu;
    }
    os << box_chars.br << "\n";
}

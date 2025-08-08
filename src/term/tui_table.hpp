//
// Created by james on 19/04/25.
//

#pragma once

//
// ut
//
#include "ut/term/escapes.hpp"
#include "ut/string.hpp"

//
// std
//
#include <ostream>
#include <string>

namespace ut
{
    struct BoxChars
    {
        std::string tl = "┌";  // top left
        std::string tr = "┐";  // top right
        std::string bl = "└";  // bottom left
        std::string br = "┘";  // bottom right
        std::string hz = "─";  // horizontal
        std::string vt = "│";  // vertical
        std::string hzd = "┬"; // horizontal down (T-junction)
        std::string hzu = "┴"; // horizontal up (inverted T)
        std::string vtl = "┤"; // vertical left (right T)
        std::string vtr = "├"; // vertical right (left T)
        std::string cross = "┼"; // cross junction
    };

    struct TableCell
    {
        std::string text = "";
        std::string styles = TERM_RESET;
    };

    class Table
    {
    public:
        std::string title;
        BoxChars box_chars;

        inline size_t width() const { return m_width; }
        inline size_t height() const { return m_height; }

        TableCell& getCell(size_t x, size_t y);
        std::string& getHeader(size_t x);

        /// Sets cell content with optional styling
        void setCell(size_t x, size_t y, std::string const& text, std::string const& styles = TERM_RESET);

        /// Sets header content
        void setHeader(size_t x, std::string const& text);

        /// Prints the table to output stream
        void print(std::ostream& os) const;

    private:
        using clist_type = std::vector<TableCell>;
        using wlist_type = std::vector<size_t>;
        using hlist_type = std::vector<std::string>;

        size_t m_width = 0;
        size_t m_height = 0;
        clist_type m_cells;
        wlist_type m_widths;
        hlist_type m_headers;

        /// Gets the index for a cell in the flat vector
        size_t getCellIndex(size_t x, size_t y) const;

        /// Calculates and updates column widths
        void updateColumnWidths();

        /// Gets sum of all column widths
        size_t getCellWidthsSum() const;

        /// Expands internal storage to accommodate given coordinates
        void expandTo(size_t width, size_t height);
    };
}
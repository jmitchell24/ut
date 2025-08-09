//
// Created by james on 20/04/25.
//

#pragma once

#include "escapes.hpp"
#include "ut/string.hpp"
namespace ut
{
    struct BoxChars
    {
        std::string
        tl,    // top left
        tr,    // top right
        bl,    // bottom left
        br,    // bottom right
        hz,    // horizontal
        vt,    // vertical
        hzd,   // horizontal down (T-junction)
        hzu,   // horizontal up (inverted T)
        vtl,   // vertical left (right T)
        vtr,   // vertical right (left T)
        cross; // cross junction
    };

    namespace box_chars
    {
        BoxChars static const regular = { "┌", "┐", "└", "┘", "─", "│", "┬", "┴", "┤", "├", "┼" };
        BoxChars static const double_line = { "╔", "╗", "╚", "╝", "═", "║", "╦", "╩", "╣", "╠", "╬" };
        BoxChars static const rounded = { "╭", "╮", "╰", "╯", "─", "│", "┬", "┴", "┤", "├", "┼" };
        BoxChars static const bold = { "┏", "┓", "┗", "┛", "━", "┃", "┳", "┻", "┫", "┣", "╋" };
        BoxChars static const ascii = { "+", "+", "+", "+", "-", "|", "+", "+", "+", "+", "+" };
        BoxChars static const mixed = { "╒", "╕", "╘", "╛", "═", "│", "╤", "╧", "╡", "╞", "╪" };
        BoxChars static const dashed = { "┌", "┐", "└", "┘", "┄", "┆", "┬", "┴", "┤", "├", "┼" };
    }

    class Table
    {
    public:
        struct Cell { std::string text="", styles=TERM_RESET; };

        Cell title;

        inline size_t width() const
        { return m_width; }

        inline size_t height() const
        { return m_height; }

        void reset();

        /// Sets cell content with optional styling
        void setCell(size_t x, size_t y, Cell const& c);

        /// Sets header content
        void setHeader(size_t x, Cell const& c);

        /// Prints the table to output stream
        void print(std::ostream& os, BoxChars const& box_chars) const;

    private:
        using clist_type = std::vector<Cell>;
        using wlist_type = std::vector<size_t>;
        using hlist_type = std::vector<Cell>;

        size_t m_width=0;
        size_t m_height=0;
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

        Cell& getCell(size_t x, size_t y);
        Cell& getHeader(size_t x);
    };

    class TableBuilder
    {
    public:
        void reset();

        void title(strparam text, strparam styles = TERM_RESET);
        void header(int x, strparam text, strparam style = TERM_RESET);

        void cell(int x, int y);
        void column(int x);
        void row(int y);
        void nextColumn();
        void nextRow();

        void content(strparam text, strparam styles = TERM_RESET);

        inline void label(strparam text) { content(text, TERM_FG_BRIGHT_CYAN); }
        inline void text(strparam text) { content(text, TERM_FG_BRIGHT_YELLOW); }
        inline void integer(int i) { content(std::to_string(i), TERM_FG_BRIGHT_MAGENTA); }
        inline void decimal(double d) { content(std::to_string(d), TERM_FG_BRIGHT_MAGENTA); }
        inline void boolean(bool b, strparam t="true", strparam f="false")
        { content(b ? t : f, b ? TERM_FG_BRIGHT_GREEN : TERM_FG_BRIGHT_RED); }

        void print(std::ostream& os, BoxChars const& box_chars = box_chars::regular) const
        { m_table.print(os, box_chars); }

    private:
        int m_x=-1;
        int m_y=-1;
        Table m_table;
    };
}

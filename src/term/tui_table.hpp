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
#include <string>

namespace ut::tui
{
    enum TableFlags { TBL_NONE };

    struct BoxChars
    {
        char const* tl = "┌";
        char const* tr = "┐";
        char const* bl = "└";
        char const* br = "┘";
        char const* hz = "─";
        char const* vt = "│";

        char const* hzd = "┬";
        char const* hzu = "┴";
        char const* vtl = "┤";
        char const* vtr = "├";
    };

    struct TableCell
    {
        std::string text = "";
        std::string styles = TERM_RESET;
    };

    class Table
    {
    public:
        using cells_type = std::vector<TableCell>;

        std::string title;
        BoxChars box_chars;

        Table(std::string const& title="", size_t width=0)
            : title{title}, m_width{width}, m_height{}, m_cells{}
        {}

        inline size_t width() const { return m_width; }
        inline size_t height() const { return m_height; }



        void setCell(size_t x, size_t y, std::string const& text, char const* styles = TERM_RESET);
        TableCell const& cell(size_t x, size_t y) const;

        void print() const;

    private:
        size_t                  m_width=0;
        size_t                  m_height=0;
        std::vector<TableCell>  m_cells;
        std::vector<size_t>     m_cell_widths;

        size_t getCellWidthsSum() const;
    };
}
//
// Created by james on 4/19/25.
//

#pragma once

// ESC character and common sequence beginnings
#define ESC                     "\033"  // ASCII escape character
#define CSI                     ESC "[" // Control Sequence Introducer

// ==================== Text Formatting ====================

// Text style/attributes
#define TERM_RESET             CSI "0m"    // Reset all text attributes to default
#define TERM_BOLD              CSI "1m"    // Bold or increased intensity
#define TERM_DIM               CSI "2m"    // Dimmed/decreased intensity
#define TERM_ITALIC            CSI "3m"    // Italic text (not widely supported)
#define TERM_UNDERLINE         CSI "4m"    // Underlined text
#define TERM_BLINK             CSI "5m"    // Blinking text (not widely supported)
#define TERM_REVERSE           CSI "7m"    // Reverse video (swap foreground/background)
#define TERM_HIDDEN            CSI "8m"    // Hidden text (invisible)
#define TERM_STRIKETHROUGH     CSI "9m"    // Strikethrough text (not widely supported)

// Disable text styles
#define TERM_BOLD_OFF          CSI "22m"   // Normal intensity (not bold or dim)
#define TERM_ITALIC_OFF        CSI "23m"   // Not italic
#define TERM_UNDERLINE_OFF     CSI "24m"   // Not underlined
#define TERM_BLINK_OFF         CSI "25m"   // Not blinking
#define TERM_REVERSE_OFF       CSI "27m"   // Not reversed
#define TERM_HIDDEN_OFF        CSI "28m"   // Not hidden
#define TERM_STRIKETHROUGH_OFF CSI "29m"   // Not strikethrough

// Foreground (text) colors - standard
#define TERM_FG_BLACK          CSI "30m"   // Black foreground
#define TERM_FG_RED            CSI "31m"   // Red foreground
#define TERM_FG_GREEN          CSI "32m"   // Green foreground
#define TERM_FG_YELLOW         CSI "33m"   // Yellow foreground
#define TERM_FG_BLUE           CSI "34m"   // Blue foreground
#define TERM_FG_MAGENTA        CSI "35m"   // Magenta foreground
#define TERM_FG_CYAN           CSI "36m"   // Cyan foreground
#define TERM_FG_WHITE          CSI "37m"   // White foreground
#define TERM_FG_DEFAULT        CSI "39m"   // Default foreground color

// Background colors - standard
#define TERM_BG_BLACK          CSI "40m"   // Black background
#define TERM_BG_RED            CSI "41m"   // Red background
#define TERM_BG_GREEN          CSI "42m"   // Green background
#define TERM_BG_YELLOW         CSI "43m"   // Yellow background
#define TERM_BG_BLUE           CSI "44m"   // Blue background
#define TERM_BG_MAGENTA        CSI "45m"   // Magenta background
#define TERM_BG_CYAN           CSI "46m"   // Cyan background
#define TERM_BG_WHITE          CSI "47m"   // White background
#define TERM_BG_DEFAULT        CSI "49m"   // Default background color

// Foreground (text) colors - bright variants
#define TERM_FG_BRIGHT_BLACK   CSI "90m"   // Bright black foreground (gray)
#define TERM_FG_BRIGHT_RED     CSI "91m"   // Bright red foreground
#define TERM_FG_BRIGHT_GREEN   CSI "92m"   // Bright green foreground
#define TERM_FG_BRIGHT_YELLOW  CSI "93m"   // Bright yellow foreground
#define TERM_FG_BRIGHT_BLUE    CSI "94m"   // Bright blue foreground
#define TERM_FG_BRIGHT_MAGENTA CSI "95m"   // Bright magenta foreground
#define TERM_FG_BRIGHT_CYAN    CSI "96m"   // Bright cyan foreground
#define TERM_FG_BRIGHT_WHITE   CSI "97m"   // Bright white foreground

// Background colors - bright variants
#define TERM_BG_BRIGHT_BLACK   CSI "100m"  // Bright black background (gray)
#define TERM_BG_BRIGHT_RED     CSI "101m"  // Bright red background
#define TERM_BG_BRIGHT_GREEN   CSI "102m"  // Bright green background
#define TERM_BG_BRIGHT_YELLOW  CSI "103m"  // Bright yellow background
#define TERM_BG_BRIGHT_BLUE    CSI "104m"  // Bright blue background
#define TERM_BG_BRIGHT_MAGENTA CSI "105m"  // Bright magenta background
#define TERM_BG_BRIGHT_CYAN    CSI "106m"  // Bright cyan background
#define TERM_BG_BRIGHT_WHITE   CSI "107m"  // Bright white background

// 256-color mode - templated values shown, replace n with color number (0-255)
// #define TERM_FG_256(n)        CSI "38;5;nm"  // 256-color foreground
// #define TERM_BG_256(n)        CSI "48;5;nm"  // 256-color background

// RGB color - templated values shown, replace r,g,b with values 0-255
// #define TERM_FG_RGB(r,g,b)    CSI "38;2;r;g;bm"  // RGB foreground
// #define TERM_BG_RGB(r,g,b)    CSI "48;2;r;g;bm"  // RGB background

// ==================== Cursor Control ====================

// Move cursor by a specified number of positions
#define TERM_CURSOR_UP(n)        CSI #n "A"  // Move cursor up n lines
#define TERM_CURSOR_DOWN(n)      CSI #n "B"  // Move cursor down n lines
#define TERM_CURSOR_RIGHT(n)     CSI #n "C"  // Move cursor right n columns
#define TERM_CURSOR_LEFT(n)      CSI #n "D"  // Move cursor left n columns
#define TERM_CURSOR_NEXT_LINE(n) CSI #n "E"  // Move cursor to beginning of line n lines down
#define TERM_CURSOR_PREV_LINE(n) CSI #n "F"  // Move cursor to beginning of line n lines up
#define TERM_CURSOR_COLUMN(n)    CSI #n "G"  // Move cursor to column n

// Set cursor position (1-based coordinates)
#define TERM_CURSOR_POS(_row,_col) CSI #_row ";" #_col "H"  // Move cursor to row, col
#define TERM_CURSOR_HOME         CSI "H"     // Move cursor to home position (1,1)

// Cursor visibility
#define TERM_CURSOR_SAVE       ESC "7"     // Save cursor position
#define TERM_CURSOR_RESTORE    ESC "8"     // Restore cursor position
#define TERM_CURSOR_HIDE       CSI "?25l"  // Hide cursor
#define TERM_CURSOR_SHOW       CSI "?25h"  // Show cursor

// ==================== Terminal Bell ====================
#define TERM_BELL                   "\a"       // Audible bell
#define TERM_VISUAL_BELL            CSI "?5h"  // Enable visual bell mode (flash screen)
#define TERM_VISUAL_BELL_OFF        CSI "?5l"  // Disable visual bell mode

// ==================== Screen Control ====================

// Clear screen parts
#define TERM_CLEAR_SCREEN           CSI "2J"     // Clear entire screen
#define TERM_CLEAR_SCREEN_TO_END    CSI "0J"     // Clear from cursor to end of screen
#define TERM_CLEAR_SCREEN_TO_BEGIN  CSI "1J"     // Clear from cursor to beginning of screen

// Clear line parts
#define TERM_CLEAR_LINE             CSI "2K"     // Clear entire line
#define TERM_CLEAR_LINE_TO_END      CSI "0K"     // Clear from cursor to end of line
#define TERM_CLEAR_LINE_TO_BEGIN    CSI "1K"     // Clear from cursor to beginning of line

// Scrolling
#define TERM_SCROLL_UP(_n)          CSI #_n "S"  // Scroll up n lines
#define TERM_SCROLL_DOWN(_n)        CSI #_n "T"  // Scroll down n lines

// ==================== Terminal Control ====================

// Terminal window title (for xterm compatible terminals)
#define TERM_SET_TITLE(_title)  ESC "]0;" _title ESC "\\"  // Set terminal window title

// Terminal modes
#define TERM_ALTERNATE_SCREEN_ON  CSI "?1049h"  // Enable alternate screen buffer
#define TERM_ALTERNATE_SCREEN_OFF CSI "?1049l"  // Disable alternate screen buffer

// ==================== Helper Macros ====================

// Convenience macros for common color combinations
#define TERM_ERROR             TERM_FG_RED       // Error messages
#define TERM_WARNING           TERM_FG_YELLOW    // Warning messages
#define TERM_SUCCESS           TERM_FG_GREEN     // Success messages
#define TERM_INFO              TERM_FG_CYAN      // Informational messages
#define TERM_HIGHLIGHT         TERM_FG_MAGENTA   // Highlighted text
#define TERM_NEWLINE           "\r\n"

//
// ut
//

#include "ut/color.hpp"

//
// std
//

#include <ostream>
#include <sstream>

namespace ut::esc
{
    inline static std::ostream& reset(std::ostream& os) { return os << TERM_RESET; }
    inline static std::ostream& rendl(std::ostream& os) { return os << TERM_RESET << std::endl; }

    struct TrueColorFg{ color c; };
    inline static TrueColorFg trueColorFg(color const& c) { return TrueColorFg{c}; }
    inline static std::ostream& operator<< (std::ostream& os, TrueColorFg const& tc)
    { return os << tc.c.toFgEscCode(); }

    struct TrueColorBg{ color c; };
    inline static TrueColorBg trueColorBg(color const& c) { return TrueColorBg{c}; }
    inline static std::ostream& operator<< (std::ostream& os, TrueColorBg const& tc)
    { return os << tc.c.toFgEscCode(); }

    struct TrueColor{ color fg, bg; };
    inline static TrueColor trueColor(color const& fg, color const& bg) { return TrueColor{fg, bg}; }
    inline static std::ostream& operator<< (std::ostream& os, TrueColor const& tc)
    { return os << tc.fg.toFgEscCode() << tc.bg.toBgEscCode(); }

#define DECL_COLOR(_x, _y, _z) \
inline static std::ostream& _x(std::ostream& os) { return os << _y; } \
inline static std::ostream& _x##Bg(std::ostream& os) { return os << _z; }

    DECL_COLOR(black    , TERM_FG_BLACK,    TERM_BG_BLACK)
    DECL_COLOR(red      , TERM_FG_RED,      TERM_BG_RED)
    DECL_COLOR(green    , TERM_FG_GREEN,    TERM_BG_GREEN)
    DECL_COLOR(yellow   , TERM_FG_YELLOW,   TERM_BG_YELLOW)
    DECL_COLOR(blue     , TERM_FG_BLUE,     TERM_BG_BLUE)
    DECL_COLOR(magenta  , TERM_FG_MAGENTA,  TERM_BG_MAGENTA)
    DECL_COLOR(cyan     , TERM_FG_CYAN,     TERM_BG_CYAN)
    DECL_COLOR(white    , TERM_FG_WHITE,    TERM_BG_WHITE)
    DECL_COLOR(original , TERM_FG_DEFAULT,  TERM_BG_DEFAULT)

    DECL_COLOR(brightBlack,     TERM_FG_BRIGHT_BLACK,   TERM_BG_BRIGHT_BLACK)
    DECL_COLOR(brightRed,       TERM_FG_BRIGHT_RED,     TERM_BG_BRIGHT_RED)
    DECL_COLOR(brightGreen,     TERM_FG_BRIGHT_GREEN,   TERM_BG_BRIGHT_GREEN)
    DECL_COLOR(brightYellow,    TERM_FG_BRIGHT_YELLOW,  TERM_BG_BRIGHT_YELLOW)
    DECL_COLOR(brightBlue,      TERM_FG_BRIGHT_BLUE,    TERM_BG_BRIGHT_BLUE)
    DECL_COLOR(brightMagenta,   TERM_FG_BRIGHT_MAGENTA, TERM_BG_BRIGHT_MAGENTA)
    DECL_COLOR(brightCyan,      TERM_FG_BRIGHT_CYAN,    TERM_BG_BRIGHT_CYAN)
    DECL_COLOR(brightWhite,     TERM_FG_BRIGHT_WHITE,   TERM_BG_BRIGHT_WHITE)
#undef DECL_COLOR

    //
    // True Color
    //
namespace tc
{
#define TRUE_COLORS(_x, _y) \
    inline static std::ostream& _x(std::ostream& os) { return os << ut::esc::trueColorFg(ut::colors::_x); } \
    inline static std::ostream& _x##Bg(std::ostream& os) { return os << ut::esc::trueColorBg(ut::colors::_x); }
UT_EXPAND_COLORS(TRUE_COLORS)
#undef TRUE_COLORS
}

#define DECL_ESC_INT(_name, _suffix) \
    inline static std::string _name(unsigned arg) \
    { \
        std::ostringstream oss; \
        oss << CSI << arg << _suffix; \
        return oss.str(); \
    }

    DECL_ESC_INT(termScrollUp, "S")
    DECL_ESC_INT(termScrollDown, "T")
    DECL_ESC_INT(termCursorUp, "A")
    DECL_ESC_INT(termCursorDown, "B")
    DECL_ESC_INT(termCursorRight, "C")
    DECL_ESC_INT(termCursorLeft, "D")
    DECL_ESC_INT(termCursorNextLine, "E")
    DECL_ESC_INT(termCursorPrevLine, "F")
    DECL_ESC_INT(termCursorColumn, "G")
#undef DECL_DYN_ESC
}

//
// Created by james on 20/04/25.
//

#pragma once

#include <string>
#include <functional>

#define ut_shell ( ut::Shell::instance() )

namespace ut
{
    class Shell
    {
    public:
        using hint_type = std::function<void(strparam)>;
        hint_type hint = nullptr;



        Shell();

        inline std::string const& lineBuffer() const
        { return m_line_buffer; }



        bool getLine(std::string& line);



        static Shell& instance();

    private:
        std::string m_line_buffer;

        void putHint(strparam s);
    };
}
//
// Created by james on 20/04/25.
//

#pragma once

#if 0

#include <string>

namespace ut
{
    class Shell
    {
    public:
        using prompt_type = void(*)();
        prompt_type prompt;


        static Shell& instance();

        Shell();

        bool getLine(std::string& line);

        inline std::string const& lineBuffer() const
        { return m_line_buffer; }

    private:
        std::string m_line_buffer;
    };

    [[maybe_unused]]
    static Shell& SHELL = Shell::instance();


    inline static bool shell(Shell::prompt_type prompt, std::string& line)
    {
        SHELL.prompt = prompt;
        return SHELL.getLine(line);
    }
}

#endif
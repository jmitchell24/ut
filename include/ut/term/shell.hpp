//
// Created by james on 20/04/25.
//

#pragma once

#include <string>

namespace ut
{
    class Shell
    {
    public:
        using prompt_type = void(*)();
        prompt_type prompt = nullptr;

        using hint_type = std::string(*)(strparam);
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

    [[maybe_unused]]
    static Shell& SHELL = Shell::instance();


    inline static bool shell(Shell::prompt_type prompt, std::string& line)
    {
        SHELL.prompt = prompt;
        return SHELL.getLine(line);
    }
}
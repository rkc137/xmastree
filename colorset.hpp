#pragma once

#include <string>
#include <vector>

namespace clr
{
    std::string color(uint8_t r, uint8_t g, uint8_t b)
    {
        std::string s = "\033[38;2;" 
                      + std::to_string(+r)
                      + ";"
                      + std::to_string(+g) 
                      + ";"
                      + std::to_string(+b) 
                      + "m";
        return s;
    }

    std::string 
        white = "\033[00m",
        green = "\033[32m",
        on_red = "\033[41m",
        on_yellow = "\033[43m",
        on_blue = "\033[44m",
        on_magenta = "\033[45m",
        on_cyan = "\033[46m",
        bright_yellow = "\033[93m",
        underline = "\033[4m",
        brown = color(101, 67, 33);
    ;

    std::vector<std::string*> toys_clrs = {
        &on_red,
        &on_yellow,
        &on_blue,
        &on_magenta,
        &on_cyan
    };
}


#pragma once

#include <string>


struct error
{
    enum error_type
    {
        LEXICAL,
        SYNTAX,
        SYNTAX_OVER,
        SYNTAX_EOF,
        MESSAGE,
        EMPTY,
    };

    error() 
        : type(EMPTY) {}

    error(error_type type) 
        : type { type } {}

    error(error_type type, char c, int line, int col)
        : type(type), c(c), line(line), col(col) {}

    error(error_type type, int lex, int line, int col)
        : type(type), lex(lex), line(line), col(col) {}

    error(const std::string& msg)
        : type(MESSAGE), msg(msg) {}

    static error lexical(char c, int line, int col)
    {
        return error(LEXICAL, c, line, col);
    }

    error_type type {};
    char c {};
    int line {};
    int col {};
    int lex {};

    std::string msg;
};
#include "print_error.hpp"
#include <error.hpp>
#include <iostream>
#include <format>
#include "strings.hpp"

std::string make_string(const error& err)
{
    switch (err.type)
    {
        case error::LEXICAL: return std::format("Lexical Error - Unexpected char '{}' at line: {}, col: {}.", err.c, err.line, err.col);
        case error::SYNTAX: return std::format("Syntax Error - Unexpected token '{}' at (line: {}, col: {})", to_string(static_cast<lexeme>(err.lex)), err.line, err.col);
        case error::SYNTAX_EOF: return "Syntax Error - End of file, incomplete program.";
        case error::SYNTAX_OVER: return "Syntax Error - Unexpected token at end of program.";
        case error::MESSAGE: return err.msg;
        default: return "Unkown error";
    }
}

void print_error(const error& err)
{
    std::cout << make_string(err) << "\n";
}
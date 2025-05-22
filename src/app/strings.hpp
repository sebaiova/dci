#pragma once 

#include <iostream>
#include <lexemes.hpp>
#include "keywords.hpp"

constexpr std::string_view to_string(lexeme l) 
{
    switch (l) 
    {
        case lexeme::UNDETERMINATED: return "undetermined";
        case lexeme::COMMENT: return "comment";
        case lexeme::IDENTIFIER: return "identifier";
        case lexeme::NUMBER: return "number";
        case lexeme::ASSIGNATION: return "assignation";
        case lexeme::ADD : return "+";
        case lexeme::SUB : return "-";
        case lexeme::MULT : return "*";
        case lexeme::RELATIONAL_OPERATOR: return "relational_operator";
        case lexeme::PROGRAM: return "program";
        case lexeme::DOT: return ".";
        case lexeme::COMMA: return ",";
        case lexeme::COLON: return ":";
        case lexeme::SEMI_COLON: return ";";
        case lexeme::OPEN_PARENTHESIS: return "open_parenthesis";
        case lexeme::CLOSE_PARENTHESIS: return "close_parenthesis";
        case lexeme::OPEN_CURLY_BRACKET: return "open_curly_bracket";
        case lexeme::CLOSE_CURLY_BRACKET: return "close_curly_bracket";
        case lexeme::IF: return "if";
        case lexeme::ELSE: return "else";
        case lexeme::WHILE: return "while";
        case lexeme::DO: return "do";
        case lexeme::THEN: return "then";
        case lexeme::VAR: return "var";
        case lexeme::FUNCTION: return "function";
        case lexeme::PROCEDURE: return "procedure";
        case lexeme::BEGIN: return "begin";
        case lexeme::END: return "end";
        case lexeme::READ: return "read";
        case lexeme::WRITE: return "write";
        case lexeme::INTEGER: return "integer";
        case lexeme::BOOLEAN: return "boolean";
        case lexeme::TRUE: return "true";
        case lexeme::FALSE: return "false";
        case lexeme::DIV: return "div";
        default: return "unknown";
    }
}

template<class T> void print_symbol_table(const T& identifiers)
{
    std::cout << "---------- Symbol table ----------\n";

    for(auto& id : identifiers)
        std::cout << "Identifier: " << id << "\n";
    
    std::cout << "********************************** \n\n";
}

template<class T> void print_token_stream(const T& tokens)
{
    std::cout << "---------- Token Stream ----------\n";

    for(auto& token : tokens)
        std::cout << to_string(token) << "\n";

    std::cout << "********************************** \n\n";
}

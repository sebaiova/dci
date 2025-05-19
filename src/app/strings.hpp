#pragma once 

#include <iostream>
#include <lexemes.hpp>
#include "keywords.hpp"

constexpr std::string_view to_string(lexeme l) 
{
    switch (l) 
    {
        case UNDETERMINATED: return "undetermined";
        case COMMENT: return "comment";
        case IDENTIFIER: return "identifier";
        case NUMBER: return "number";
        case ASSIGNATION: return "assignation";
        case ARITHMETIC_OPERATOR: return "arithmetic_operator";
        case RELATIONAL_OPERATOR: return "relational_operator";
        case PROGRAM: return "program";
        case DOT: return "dot";
        case COLON: return "colon";
        case SEMI_COLON: return "semi_colon";
        case OPEN_PARENTHESIS: return "open_parenthesis";
        case CLOSE_PARENTHESIS: return "close_parenthesis";
        case OPEN_CURLY_BRACKET: return "open_curly_bracket";
        case CLOSE_CURLY_BRACKET: return "close_curly_bracket";
        case IF: return "if";
        case ELSE: return "else";
        case WHILE: return "while";
        case DO: return "do";
        case THEN: return "then";
        case VAR: return "var";
        case FUNCTION: return "function";
        case PROCEDURE: return "procedure";
        case BEGIN: return "begin";
        case END: return "end";
        case READ: return "read";
        case WRITE: return "write";
        case INTEGER: return "integer";
        case BOOLEAN: return "boolean";
        case TRUE: return "true";
        case FALSE: return "false";
        case DIV: return "div";
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

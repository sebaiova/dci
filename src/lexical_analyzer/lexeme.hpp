#pragma once

#include <string>

struct lexeme
{
    enum lexeme_type 
    {
        UNDETERMINATED, 
        IDENTIFIER, 
        NUMBER, 
        ASSIGNATION, 
        ARITHMETIC_OPERATOR, 
        RELATIONAL_OPERATOR,
        ERROR
    };

    lexeme()
    {

    };

    lexeme(lexeme_type token, const std::string& attribute) : 
        token { token }, attribute { attribute }
    {

    };

    lexeme(lexeme_type token, char attribute) : 
        token { token }, attribute { std::string(1, attribute) }
    {

    };

    bool operator<(const lexeme& other) const
    {
        return this->attribute < other.attribute;
    }

    lexeme_type token       { UNDETERMINATED };
    std::string attribute   { "" };
};
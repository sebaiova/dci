#pragma once

#include <string>

enum lexeme
{
    UNDETERMINATED,
    COMMENT,
    IDENTIFIER, 
    NUMBER, 
    ASSIGNATION, 
    ARITHMETIC_OPERATOR, 
    RELATIONAL_OPERATOR,
    PROGRAM,
    DOT,
    COMMA,
    COLON,
    SEMI_COLON,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_CURLY_BRACKET,
    CLOSE_CURLY_BRACKET,
    IF,
    ELSE,  
    WHILE,
    DO,
    THEN,
    VAR,
    FUNCTION,
    PROCEDURE,
    BEGIN,
    END,
    READ,
    WRITE,
    INTEGER,
    BOOLEAN,
    TRUE,
    FALSE,
    DIV
};
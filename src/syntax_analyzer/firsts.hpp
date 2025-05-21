#pragma once 

#include "non_terminal.hpp"
#include "ll1.hpp"

template<beta<non_terminal>> struct first;

template<> struct first<beta<non_terminal>{non_terminal::SENTENCIA_CONDICIONAL}>
{
    using get = rh<lexeme::IF>;
};

template<> struct first<beta<non_terminal>{non_terminal::SUBRUTINA}>
{
    using get = rh<lexeme::FUNCTION, lexeme::PROCEDURE>;
};

template<> struct first<beta<non_terminal>{non_terminal::EXPRESION}>
{
    using get = rh<lexeme::ADD, lexeme::SUB, lexeme::UNDETERMINATED>;
};

template<> struct first<beta<non_terminal>{non_terminal::FUNCION}>
{
    using get = rh<lexeme::FUNCTION>;
};

template<> struct first<beta<non_terminal>{non_terminal::PROCEDIMIENTO}>
{
    using get = rh<lexeme::PROCEDURE>;
};

template<> struct first<beta<non_terminal>{non_terminal::OPERADOR_FACTORES}>
{
    using get = rh<lexeme::MULT, lexeme::AND, lexeme::DIV>;
};

template<> struct first<beta<non_terminal>{non_terminal::FACTOR}>
{
    using get = rh<lexeme::NUMBER, lexeme::NOT, lexeme::OPEN_PARENTHESIS, lexeme::IDENTIFIER>;
};

template<> struct first<beta<non_terminal>{non_terminal::AREA_VARIABLES}>
{
    using get = rh<lexeme::VAR, lexeme::UNDETERMINATED>;
};

template<> struct first<beta<non_terminal>{non_terminal::SIGNO}>
{
    using get = rh<lexeme::ADD, lexeme::SUB, lexeme::UNDETERMINATED>;
};

template<> struct first<beta<non_terminal>{non_terminal::EXPRESION_SIMPLE}>
{
    using get = rh<lexeme::ADD, lexeme::SUB, lexeme::UNDETERMINATED>;
};

template<> struct first<beta<non_terminal>{non_terminal::OPERADOR_TERMINOS}>
{
    using get = rh<lexeme::ADD, lexeme::SUB, lexeme::OR>;
};

template<> struct first<beta<non_terminal>{non_terminal::PARAMETROS_FORMALES}>
{
    using get = rh<lexeme::OPEN_PARENTHESIS>;
};

template<> struct first<beta<non_terminal>{non_terminal::SENTENCIA_REPETITIVA}>
{
    using get = rh<lexeme::WHILE>;
};

template<> struct first<beta<non_terminal>{non_terminal::PARAMETRO_FORMAL}>
{
    using get = rh<lexeme::IDENTIFIER>;
};

template<> struct first<beta<non_terminal>{non_terminal::SENTENCIA_SIMPLE}>
{
    using get = rh<lexeme::IDENTIFIER, lexeme::IF, lexeme::WHILE>;
};

template<> struct first<beta<non_terminal>{non_terminal::SENTENCIA_COMPUESTA}>
{
    using get = rh<lexeme::BEGIN>;
};
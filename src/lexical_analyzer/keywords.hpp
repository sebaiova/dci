#pragma once
#include <tuple>
#include "lexeme.hpp"

template<lexeme Lexeme, char... C>
struct keyword
{
    static constexpr lexeme token = Lexeme;
    static constexpr size_t size = sizeof...(C); 
    static constexpr std::array<char, size + 1> str{C..., '\0'};
};

using keywords = std::tuple<
    keyword<PROGRAM, 'p', 'r', 'o', 'g', 'r', 'a', 'm'>,
    keyword<IF, 'i', 'f'>,
    keyword<ELSE, 'e', 'l', 's', 'e'>,
    keyword<WHILE, 'w', 'h', 'i', 'l', 'e'>,
    keyword<DO, 'd', 'o'>,
    keyword<THEN, 't', 'h', 'e', 'n'>,
    keyword<VAR, 'v', 'a', 'r'>,
    keyword<FUNCTION, 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n'>,
    keyword<PROCEDURE, 'p', 'r', 'o', 'c', 'e', 'd', 'u', 'r', 'e'>,
    keyword<BEGIN, 'b', 'e', 'g', 'i', 'n'>,
    keyword<END, 'e', 'n', 'd'>,
    keyword<READ, 'r', 'e', 'a', 'd'>,
    keyword<WRITE, 'w', 'r', 'i', 't', 'e'>,
    keyword<INTEGER, 'i', 'n', 't', 'e', 'g', 'e', 'r'>,
    keyword<BOOLEAN, 'b', 'o', 'o', 'l', 'e', 'a', 'n'>,
    keyword<TRUE, 't', 'r', 'u', 'e'>,
    keyword<FALSE, 'f', 'a', 'l', 's', 'e'>
>;
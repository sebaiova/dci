#pragma once
#include <tuple>
#include "lexemes.hpp"

template<lexeme Lexeme, char... C>
struct keyword
{
    static constexpr lexeme token = Lexeme;
    static constexpr size_t size = sizeof...(C); 
    static constexpr std::array<char, size + 1> str{C..., '\0'};
};

using keywords = std::tuple<
    keyword<lexeme::PROGRAM, 'p', 'r', 'o', 'g', 'r', 'a', 'm'>,
    keyword<lexeme::IF, 'i', 'f'>,
    keyword<lexeme::ELSE, 'e', 'l', 's', 'e'>,
    keyword<lexeme::WHILE, 'w', 'h', 'i', 'l', 'e'>,
    keyword<lexeme::DO, 'd', 'o'>,
    keyword<lexeme::THEN, 't', 'h', 'e', 'n'>,
    keyword<lexeme::VAR, 'v', 'a', 'r'>,
    keyword<lexeme::FUNCTION, 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n'>,
    keyword<lexeme::PROCEDURE, 'p', 'r', 'o', 'c', 'e', 'd', 'u', 'r', 'e'>,
    keyword<lexeme::BEGIN, 'b', 'e', 'g', 'i', 'n'>,
    keyword<lexeme::END, 'e', 'n', 'd'>,
    keyword<lexeme::INTEGER, 'i', 'n', 't', 'e', 'g', 'e', 'r'>,
    keyword<lexeme::BOOLEAN, 'b', 'o', 'o', 'l', 'e', 'a', 'n'>,
    keyword<lexeme::TRUE, 't', 'r', 'u', 'e'>,
    keyword<lexeme::FALSE, 'f', 'a', 'l', 's', 'e'>,
    keyword<lexeme::NOT, 'n', 'o', 't'>,
    keyword<lexeme::OR, 'o', 'r'>,
    keyword<lexeme::AND, 'a', 'n', 'd'>
>;
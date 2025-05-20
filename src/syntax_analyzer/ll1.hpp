#pragma once 

#include <lexemes.hpp>
#include <error.hpp>
#include "non_terminal.hpp"

enum beta_type
{
    RECURSIVE
};

struct syntax_analyzer;

template<class T> struct beta
{
    constexpr beta(T value) : value { value } { }
    const T value;

    static constexpr bool is_terminal() { return std::is_same_v<T, lexeme>; }
    static constexpr bool is_non_terminal() { return std::is_same_v<T, non_terminal>; }
};

template<typename... Rh> struct rules {};
template<beta... Beta> struct rh {};

template<class Rule> struct rule 
{ 

};




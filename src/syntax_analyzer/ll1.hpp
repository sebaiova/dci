#pragma once 

#include <lexemes.hpp>
#include <error.hpp>
#include <semantic_rule.hpp>
#include "non_terminal.hpp"

struct syntax_analyzer;

template<class T> struct beta
{
    constexpr beta(T value) : value { value } { }
    const T value;

    static constexpr bool is_terminal() { return std::is_same_v<T, lexeme>; }
    static constexpr bool is_non_terminal() { return std::is_same_v<T, non_terminal>; }
    static constexpr bool is_semantic_rule() { return std::is_same_v<T, semantic_rule>; }
};

template<typename... Rh> struct rules {};
template<beta... Beta> struct rh {};

template<class Rule> struct rule 
{ 

};




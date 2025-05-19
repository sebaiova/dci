#pragma once 

#include <lexemes.hpp>
#include <error.hpp>

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
    static constexpr bool is_recursive() { return std::is_same_v<T, beta_type>; }

    std::expected<void, error> operator()(syntax_analyzer& p) const
    {
        if constexpr (not is_terminal() && not is_recursive())
            return value(p);
        return {};
    }
};

template<typename... Rh> struct rules {};
template<beta... Beta> struct rh {};

template<class Rule> struct rule 
{ 

};




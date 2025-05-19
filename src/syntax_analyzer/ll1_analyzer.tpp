#pragma once

#include <expected>
#include <error.hpp>
#include "ll1.hpp"

template<class T> struct first {};

template<> struct first<rh<>>
{
    static constexpr lexeme value()
    {
        return lexeme::UNDETERMINATED;
    }
};

template<beta First, beta...Rest> struct first<rh<First, Rest...>>
{
    static constexpr lexeme value()
    {
        if constexpr (decltype(First)::is_terminal())
            return First.value;
        return first<rh<Rest...>>::value();
    };
};

template<class...RHs> struct rule <rules<RHs...>>
{
    static constexpr auto run(syntax_analyzer& p) -> std::expected<void, error>
    {
        std::expected<void, error> result;

        if (((p.pre_analysis(first<RHs>::value()) && (result = rule<RHs>::run(p), true)) || ...) && true)
            return result;

        return std::unexpected(error(std::format("Syntax Error - Unexpected token '{}'.", (int)p.next_token())));
    }
};

template<> struct rule <rh<>>
{
    static constexpr auto run(syntax_analyzer& p) -> std::expected<void, error>
    {
        return {};
    }
};

template<beta...Bs> struct rule <rh<Bs...>>
{
    static constexpr auto run(syntax_analyzer& p) -> std::expected<void, error>
    {
        std::expected<void, error> result;
        if (((result = 
                decltype(Bs)::is_terminal() ? (p.match(Bs.value)) : 
                (decltype(Bs)::is_recursive() ? rule<rh<Bs...>>::run(p) : 
                Bs(p)))
            && ...))
            ;
        return result;
    }
};

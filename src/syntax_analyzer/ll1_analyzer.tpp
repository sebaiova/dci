#pragma once

#include <expected>
#include <error.hpp>
#include "non_terminal.hpp"
#include "ll1.hpp"
#include "grammar.hpp"
#include "syntax_analyzer.hpp"

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

auto table(non_terminal nt, syntax_analyzer& p) -> std::expected<void, error>
{
    switch(nt)
    {
        #define X(name) \
        case non_terminal::name : return rule<name>::run(p);
        NON_TERMINAL_LIST
        #undef X
        default: return std::unexpected(error("Panic"));
    }
}

template<beta...Bs> struct rule <rh<Bs...>>
{
    static constexpr auto run(syntax_analyzer& p) -> std::expected<void, error>
    {
        std::expected<void, error> result;

        if (((result = [&p](){
            if constexpr (decltype(Bs)::is_terminal())
                return p.match(Bs.value);
            else if constexpr (decltype(Bs)::is_non_terminal())
                return table(Bs.value, p);
            else 
                return std::expected<void, error>{std::unexpected(error("Panic"))};
        }()) && ...))
                ;

        return result;
    }
};


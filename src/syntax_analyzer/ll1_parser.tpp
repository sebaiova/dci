#pragma once

#include <expected>
#include <error.hpp>
#include "non_terminal.hpp"
#include "ll1.hpp"
#include "ll1_analyzer.tpp"
#include "grammar.hpp"
#include "syntax_analyzer.hpp"

template<class T> struct pre_analysis;
template<beta<lexeme>...Bts> struct pre_analysis<rh<Bts...>>
{
    static constexpr bool run(syntax_analyzer& p)
    {
        return (p.pre_analysis(Bts.value) || ...);
    }
};

template<class...RHs> struct rule <rules<RHs...>>
{
    static constexpr auto run(syntax_analyzer& p) -> std::expected<void, error>
    {
        std::expected<void, error> result;
        if (((pre_analysis< typename get_firsts<RHs>::lex >::run(p) && (result = rule<RHs>::run(p), true)) || ...))
            return result;

        return std::unexpected(error());
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


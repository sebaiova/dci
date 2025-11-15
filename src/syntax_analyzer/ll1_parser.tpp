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
    static constexpr auto run(syntax_analyzer& p, semantic_analyzer& s, mepa_generator& g) -> std::expected<void, error>
    {
        if(auto result = p.demand_token(); not result)
            return result;

        std::expected<void, error> result;
        if (((pre_analysis< typename get_firsts<RHs>::lex >::run(p) && (result = rule<RHs>::run(p, s, g), true)) || ...))
            return result;

        return std::unexpected(error(error::SYNTAX, (int)*p.current_token, p._lexical.get_line(), p._lexical.get_col()));
    }
};

template<> struct rule <rh<>>
{
    static constexpr auto run(syntax_analyzer& p, semantic_analyzer& s, mepa_generator& g) -> std::expected<void, error>
    {
        return {};
    }
};

auto table(non_terminal nt, syntax_analyzer& p, semantic_analyzer& s, mepa_generator& g) -> std::expected<void, error>
{
    switch(nt)
    {
        #define X(name) \
        case non_terminal::name : return rule<name>::run(p, s, g);
        NON_TERMINAL_LIST
        #undef X
        default: return std::unexpected(error("Panic"));
    }
}

template<beta...Bs> struct rule <rh<Bs...>>
{
    static constexpr auto run(syntax_analyzer& p, semantic_analyzer& s, mepa_generator& g) -> std::expected<void, error>
    {
        std::expected<void, error> result;

        if (((result = [&p, &s, &g]() -> std::expected<void, error> {
            if constexpr (decltype(Bs)::is_terminal())
                return p.match(Bs.value);
            else if constexpr (decltype(Bs)::is_non_terminal())
                return table(Bs.value, p, s, g);
            else if constexpr (decltype(Bs)::is_semantic_rule())
            {
                auto ok = (s.*Bs.value)();
                if(not ok)
                    return std::unexpected(error(ok.error().msg + std::format(" Seen at line: {} col: {}", p._lexical.get_line(), p._lexical.get_col())));
                return {};
            }
            else if constexpr (decltype(Bs)::is_mepa_rule())
            {
                return (g.*Bs.value)();
            }
            else 
                return std::expected<void, error>{std::unexpected(error("Panic"))};
        }()) && ...))
                ;

        return result;
    }
};


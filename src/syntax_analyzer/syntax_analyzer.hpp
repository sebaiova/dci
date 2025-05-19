#pragma once

#include <list>
#include <expected>
#include <lexemes.hpp>
#include <error.hpp>
#include "ll1.hpp"

struct syntax_analyzer 
{
    syntax_analyzer(std::list<lexeme>::const_iterator it) : it { it }
    {

    }

    std::expected<void, error> start();

private:

    std::list<lexeme>::const_iterator it;

    constexpr bool pre_analysis(lexeme expected) const;
    constexpr auto match(lexeme expected)  -> std::expected<void, error>;
    constexpr auto match(beta_type)  -> std::expected<void, error>;
    constexpr auto match(std::expected<void, error>(*)(syntax_analyzer&)) -> std::expected<void, error>;

    constexpr lexeme next_token() const;


    template<class Rule> friend struct rule;
};
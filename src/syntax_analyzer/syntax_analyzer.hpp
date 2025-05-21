#pragma once

#include <list>
#include <expected>
#include <lexemes.hpp>
#include <error.hpp>
#include "ll1.hpp"
#include <lexical_analyzer.hpp>

struct syntax_analyzer 
{
    syntax_analyzer(const std::list<symbol>& token_stream);
    std::expected<void, error> start();
    constexpr bool pre_analysis(lexeme expected) const;

private:

    std::list<symbol>::const_iterator end;
    std::list<symbol>::const_iterator it;

    constexpr auto match(lexeme expected)  -> std::expected<void, error>;
    constexpr lexeme next_token() const;

    template<class Rule> friend struct rule;
};
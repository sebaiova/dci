#pragma once

#include <list>
#include <expected>
#include <lexemes.hpp>
#include <error.hpp>
#include "ll1.hpp"
#include <lexical_analyzer.hpp>

struct syntax_analyzer 
{
    syntax_analyzer(lexical_analyzer& lexical);
    std::expected<void, error> start();
    constexpr bool pre_analysis(lexeme expected) const;

private:

    constexpr auto match(lexeme expected)  -> std::expected<void, error>;
    constexpr auto demand_token() -> std::expected<void, error>;

    std::optional<lexeme> current_token {std::nullopt};
    lexical_analyzer& lexical;

    template<class Rule> friend struct rule;
};
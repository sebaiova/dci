#pragma once

#include <list>
#include <expected>
#include <lexemes.hpp>
#include <error.hpp>
#include "ll1.hpp"
#include <lexical_analyzer.hpp>
#include <semantic_analyzer.hpp>
#include <mepa_generator.hpp>

struct syntax_analyzer 
{
    syntax_analyzer(lexical_analyzer& lexical, semantic_analyzer& semantical, mepa_generator& mepa);
    std::expected<void, error> start();
    constexpr bool pre_analysis(lexeme expected);

private:

    bool token_consumed { false };

    constexpr auto match(lexeme expected)  -> std::expected<void, error>;
    constexpr auto demand_token() -> std::expected<void, error>;

    std::optional<lexeme> current_token {std::nullopt};
    lexical_analyzer& _lexical;
    semantic_analyzer& _semantical;
    mepa_generator& _mepa;

    template<class Rule> friend struct rule;
};

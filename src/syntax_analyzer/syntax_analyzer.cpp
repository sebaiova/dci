#include <iostream>
#include <format>
#include <variant>
#include "syntax_analyzer.hpp"
#include "ll1_parser.tpp"
#include "grammar.hpp"
#include <error.hpp>

syntax_analyzer::syntax_analyzer(lexical_analyzer& lexical, semantic_analyzer& semantical) :
    _lexical { lexical }, _semantical { semantical }
{

}

constexpr auto syntax_analyzer::demand_token() -> std::expected<void, error>
{
    if(not token_consumed)
        return {};

    auto result = _lexical.next_token();
    if(result)
    {
        current_token = result.value().token;
        token_consumed = false;
        return {};
    }
    return std::unexpected(result.error());
}

constexpr bool syntax_analyzer::pre_analysis(lexeme expected)
{
    return ( expected == current_token || expected == lexeme::UNDETERMINATED );
}

constexpr auto syntax_analyzer::match(lexeme expected) -> std::expected<void, error>
{
    if(auto result = demand_token(); not result)
        return result;

    if (expected != current_token)
        return std::unexpected(error(error::SYNTAX, (int)*current_token, _lexical.get_line(), _lexical.get_col()));

    token_consumed = true;

    return {}; 
}

auto syntax_analyzer::start() -> std::expected<void, error>
{
    token_consumed = true;

    if(auto result = demand_token(); not result)
        return result;


    if(auto result = rule<START>::run(*this, _semantical); not result)
        return std::unexpected(result.error());

    if(auto result = demand_token(); not result)
        return result;

    if(current_token!=lexeme::END_OF_FILE)
        return std::unexpected(error(error::SYNTAX_OVER));

    return {};
}

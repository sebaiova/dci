#include <iostream>
#include <format>
#include <variant>
#include "syntax_analyzer.hpp"
#include "ll1_parser.tpp"
#include "grammar.hpp"
#include <error.hpp>

syntax_analyzer::syntax_analyzer(lexical_analyzer& lexical) :
    lexical { lexical }
{

}

constexpr auto syntax_analyzer::demand_token() -> std::expected<void, error>
{
    auto result = lexical.next_token();
    if(result)
    {
        current_token = result.value().token;
        return {};
    }
    return std::unexpected(result.error());
}

constexpr bool syntax_analyzer::pre_analysis(lexeme expected) const
{
    return ( expected == current_token || expected == lexeme::UNDETERMINATED );
}

constexpr auto syntax_analyzer::match(lexeme expected) -> std::expected<void, error>
{
    if (expected != current_token)
        return std::unexpected(error(error::SYNTAX, (int)*current_token, lexical.get_line(), lexical.get_col())); 

    if(auto result = demand_token(); not result)
        return result;

    return {}; 
}

auto syntax_analyzer::start() -> std::expected<void, error>
{
    if(auto result = demand_token(); not result)
        return result;

    if(auto result = rule<START>::run(*this); not result)
        return std::unexpected(error(error::SYNTAX, (int)*current_token, lexical.get_line(), lexical.get_col())); 

    if(current_token!=lexeme::END_OF_FILE)
        return std::unexpected(error(error::SYNTAX_OVER));

    return {};
}
#include <iostream>
#include <format>
#include <variant>
#include "syntax_analyzer.hpp"
#include "ll1_parser.tpp"
#include "grammar.hpp"
#include <error.hpp>

syntax_analyzer::syntax_analyzer(const std::list<symbol>& token_stream) :
    it { token_stream.begin() }, end { token_stream.end() }
{

}

constexpr lexeme syntax_analyzer::next_token() const
{
    return it->token;
}

constexpr bool syntax_analyzer::pre_analysis(lexeme expected) const
{
    return it!=end && ( expected == it->token || expected == lexeme::UNDETERMINATED );
}

constexpr auto syntax_analyzer::match(lexeme expected) -> std::expected<void, error>
{

    if (it!=end && expected == it->token)
    {
        it++;
        return {};
    }
    return std::unexpected(error());
}

auto syntax_analyzer::start() -> std::expected<void, error>
{
    auto result { rule<START>::run(*this)  };
    
    if(not result)
    {
        if(it==end)
            return std::unexpected(error(error::SYNTAX_EOF));
        return std::unexpected(error(error::SYNTAX, (int)it->token, it->line, it->col));    
    }

    if(it!=end)
        return std::unexpected(error(error::SYNTAX_OVER));

    return result;
}
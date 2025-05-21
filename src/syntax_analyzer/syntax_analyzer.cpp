#include <iostream>
#include <format>
#include <variant>
#include "syntax_analyzer.hpp"
#include "ll1_analyzer.tpp"
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
    return std::unexpected(error(std::format("Sintax Error - Expected '{}')", (int)expected)));
}

auto syntax_analyzer::start() -> std::expected<void, error>
{
    auto result { rule<START>::run(*this)  };
    
    if(not result)
    {
        if(it==end)
            return std::unexpected(error("Syntax Error - End of file, incomplete program."));
        return std::unexpected(error(std::format("Syntax Error - Unexpected token '{}' at (line: {}, col: {})", (int)it->token, it->line, it->col)));    
    }

    if(it!=end)
        return std::unexpected(error("Syntax Error - Unexpected token at end of program."));

    return result;
}
#include "lexical_analyzer.hpp"
#include "state.hpp"
#include <iostream>
#include <format>
#include <semantic_analyzer.hpp>

auto lexical_analyzer::operator>>(std::expected<symbol, error>& output) -> lexical_analyzer&
{
    output = next_token();
    return *this;
}

auto lexical_analyzer::next_token() -> std::expected<symbol, error>
{
    symbol output;
    std::expected<state::result, error> current_state { state::result{} };
    skip_spaces();
    output.line=line;
    output.col=col;
    start = it;
    
    while(it != buffer.end())
    {
        char c = next_char();

        if(!(current_state = current_state->next_transition(c)))
            return std::unexpected(error(error::LEXICAL, c, line, col));
        
        if(current_state->token == lexeme::COMMENT)
        {
            backtrack();
            return next_token();
        }

        if(current_state->token != lexeme::UNDETERMINATED)
        {
            backtrack();
            auto attribute { current_state->token==lexeme::IDENTIFIER ? std::make_optional(std::string(start, it)) : std::nullopt };
            output.token = current_state->token;
            output.attribute = attribute;
            auto ok = token_register(output);
            if(not ok)
                return std::unexpected(error(ok.error()));
            return output;
        }
    }
    return std::unexpected(error(error::SYNTAX_EOF));
}

char lexical_analyzer::next_char()
{
    char c = *it;
    it++;
    col++;
    return std::tolower(c);
}

void lexical_analyzer::backtrack()
{
    it--;
    col--;
}

void lexical_analyzer::skip_spaces()
{
    while(std::isspace(*it))
    {
        if(*it=='\n')
        {
            col=-1;
            line++;
        }
        it++;
        col++;
    }
}

bool has_value(const lexeme lex)
{
    return lex == lexeme::NUMBER || lex == lexeme::FALSE || lex == lexeme::TRUE || lex == lexeme::IDENTIFIER;
}

std::expected<void, error> lexical_analyzer::token_register(const symbol& s)
{   
    token_stream.push_back(s);
    if(s.attribute)
       return _semantic.analyze_symbol(*s.attribute);

    if(has_value(s.token))
        _semantic._last_value = s.token;

    return {};
}

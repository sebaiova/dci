#include "lexical_analyzer.hpp"
#include "state.hpp"
#include <iostream>
#include <format>

auto lexical_analyzer::operator>>(std::expected<symbol, error>& output) -> lexical_analyzer&
{
    std::expected<state::result, error> current_state { state::result{} };

    skip_spaces();

    output->line=line;
    output->col=col;

    start = it;
    while(it != buffer.end())
    {
        char c = next_char();

        if(!(current_state = current_state->next_transition(c)))
        {
            output = std::unexpected(error(error::LEXICAL, c, line, col));
            break;
        }

        if(current_state->token != lexeme::UNDETERMINATED)
        {
            backtrack();
            auto attribute { current_state->token==lexeme::IDENTIFIER ? std::make_optional(std::string(start, it)) : std::nullopt };
            output->token = current_state->token;
            output->attribute = attribute;
            break;
        }
    }
    return *this;
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
#include "state.hpp"
#include "keywords_dfa.tpp"

namespace state 
{
    auto start(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case 'a'...'z': return keyword_state<keywords>(c);
            case '0'...'9': return result { lexeme::UNDETERMINATED, state::number  };
            case '+':       return result { lexeme::UNDETERMINATED, state::space<lexeme::ADD> };
            case '-':       return result { lexeme::UNDETERMINATED, state::space<lexeme::SUB> };
            case '*':       return result { lexeme::UNDETERMINATED, state::space<lexeme::MULT> };
            case '.':       return result { lexeme::UNDETERMINATED, state::space<lexeme::DOT> };
            case ',':       return result { lexeme::UNDETERMINATED, state::space<lexeme::COMMA> };
            case ';':       return result { lexeme::UNDETERMINATED, state::space<lexeme::SEMI_COLON> };
            case '(':       return result { lexeme::UNDETERMINATED, state::space<lexeme::OPEN_PARENTHESIS> };
            case ')':       return result { lexeme::UNDETERMINATED, state::space<lexeme::CLOSE_PARENTHESIS> };
            case '{':       return result { lexeme::UNDETERMINATED, state::comment };
            case '}':       return result { lexeme::UNDETERMINATED, state::space<lexeme::CLOSE_CURLY_BRACKET> };
            case '=':       return result { lexeme::UNDETERMINATED, state::space<lexeme::RELATIONAL_OPERATOR> };
            case '<':
            case '>':       return result { lexeme::UNDETERMINATED, state::relop };
            case ':':       return result { lexeme::UNDETERMINATED, state::assign };
            case ' ':
            case '\n':
            case '\t':      return result { lexeme::UNDETERMINATED, state::start };
            case '\x1A':    return result { lexeme::UNDETERMINATED, state::space<lexeme::END_OF_FILE> }; 
            default:        return { std::unexpected( error()) };
        }
    };

    auto identifier(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case 'a'...'z':
            case '0'...'9': return result { lexeme::UNDETERMINATED, state::identifier };
            default:        return result { lexeme::IDENTIFIER, state::start };
        }
    };

    auto number(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case '0'...'9': return result { lexeme::UNDETERMINATED, state::number };
            default:        return result { lexeme::NUMBER, state::start };
        }
    };

    auto relop(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case '=': return result { lexeme::UNDETERMINATED, state::space<lexeme::RELATIONAL_OPERATOR> };
            default : return result { lexeme::RELATIONAL_OPERATOR, state::start };
        }
    };

    auto assign(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case '=': return result { lexeme::UNDETERMINATED, state::space<lexeme::ASSIGNATION> };
            default : return result { lexeme::COLON, state::start };
        }
    };

    auto comment(char c) -> std::expected<result, error>
    {
        switch(c)
        {
            case '}': return result { lexeme::UNDETERMINATED, state::space<lexeme::COMMENT> };
            default: return result { lexeme::UNDETERMINATED, state::comment };
        }
    };

    template<lexeme T>auto space(char c) -> std::expected<result, error>
    {
        return result {T, state::start };
    }
}




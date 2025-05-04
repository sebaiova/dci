#include "state.hpp"

namespace state 
{

    template <lexeme LEXEME> auto keyword(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case '0'...'9':
            case 'a'...'z': return result { lexeme::UNDETERMINATED, state::identifier };
            default: return result { LEXEME, state::start };
        }
    };

    template <lexeme LEXEME, char HEAD, char... TAIL> auto keyword(char c) -> std::expected<result, error>
    {
        if(c == HEAD)
            return result { lexeme::UNDETERMINATED, state::keyword<LEXEME, TAIL...> };
        return state::identifier(c);
    };

    template <lexeme LEXEME1, char HEAD1, char... TAIL1, lexeme LEXEME2, char HEAD2, char... TAIL2> auto keyword(char c) -> std::expected<result, error>
    {
        if(c == HEAD1 && c == HEAD2)
            return result { lexeme::UNDETERMINATED, state::keyword<LEXEME1, TAIL1..., LEXEME2, TAIL2...> };

        switch(c)
        {
            case HEAD1: return result { lexeme::UNDETERMINATED, state::keyword<LEXEME1, TAIL1...> };
            case HEAD2: return result { lexeme::UNDETERMINATED, state::keyword<LEXEME2, TAIL2...> };
            default: return state::identifier(c);
        }
    };

    auto start(char c) -> std::expected<result, error>
    {
        switch (c)
        {
            case 'a'...'z': 
            switch (c)
            {
                case 'w': return result { lexeme::UNDETERMINATED, state::keyword<lexeme::WHILE, 'h', 'i', 'l', 'e'> };
                case 'p': return result { lexeme::UNDETERMINATED, state::keyword<lexeme::BEGIN_PROGRAM, 'r', 'o', 'g', 'r', 'a', 'm', lexeme::PROCEDURE, 'r', 'o', 'c', 'e', 'd', 'u', 'r', 'e'> };
                default: return result { lexeme::UNDETERMINATED, state::identifier };
            }
                
            case '0'...'9': return result { lexeme::UNDETERMINATED, state::number  };
            case '+':   
            case '-':      
            case '*':       return result { lexeme::UNDETERMINATED, state::space<lexeme::ARITHMETIC_OPERATOR> };
            case '.':       return result { lexeme::UNDETERMINATED, state::space<lexeme::END_PROGRAM> };
            case ';':       return result { lexeme::UNDETERMINATED, state::space<lexeme::END_STATEMENT> };
            case '(':       return result { lexeme::UNDETERMINATED, state::space<lexeme::OPEN_PARENTHESIS> };
            case ')':       return result { lexeme::UNDETERMINATED, state::space<lexeme::CLOSE_PARENTHESIS> };
            case '{':       return result { lexeme::UNDETERMINATED, state::space<lexeme::OPEN_CURLY_BRACKET> };
            case '}':       return result { lexeme::UNDETERMINATED, state::space<lexeme::CLOSE_CURLY_BRACKET> };
            case '=':       return result { lexeme::UNDETERMINATED, state::space<lexeme::RELATIONAL_OPERATOR> };
            case '<':
            case '>':       return result { lexeme::UNDETERMINATED, state::relop };
            case ':':       return result { lexeme::UNDETERMINATED, state::assign };
            case ' ':
            case '\n':
            case '\t':      return result { lexeme::UNDETERMINATED, state::start };
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
            default : return std::unexpected( error() );
        }
    };

    template<lexeme T>auto space(char c) -> std::expected<result, error>
    {
        return result {T, state::start };
    }

}




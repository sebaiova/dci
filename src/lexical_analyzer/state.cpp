#include "state.hpp"

namespace state 
{
    state_result::state_result() : 
        lex {lexeme::UNDETERMINATED, ""}, next_transition { state::start }
    {

    }

    state_result::state_result(const lexeme& lex, const std::function<state_result(char, const std::string&)>& next_transition) :
        lex { lex }, next_transition { next_transition }
    {
        
    }

    state_result start(char c, const std::string& acc)
    {
        switch (c)
        {
            case 'a'...'z': return {{ lexeme::UNDETERMINATED, c}, state::identifier };
            case '0'...'9': return {{ lexeme::UNDETERMINATED, c}, state::number  };
            case '+':   
            case '-':       return {{ lexeme::ARITHMETIC_OPERATOR, c}, state::start };
            case '=':       return {{ lexeme::RELATIONAL_OPERATOR, c}, state::start };
            case '<':
            case '>':       return {{ lexeme::UNDETERMINATED, c}, state::relop };
            case ':':       return {{ lexeme::UNDETERMINATED, c}, state::assign };
            case ' ':
            case '\n':
            case '\t':      return {{ lexeme::UNDETERMINATED, c}, state::start };
            default:        return {{ lexeme::ERROR, c}, state::start };
        }
    };

    state_result identifier(char c, const std::string& acc)
    {
        switch (c)
        {
            case 'a'...'z':
            case '0'...'9': return {{ lexeme::UNDETERMINATED, acc+c}, state::identifier };
            default:        return {{ lexeme::IDENTIFIER, acc}, state::start };
        }
    };

    state_result number(char c, const std::string& acc)
    {
        switch (c)
        {
            case '0'...'9': return {{ lexeme::UNDETERMINATED, acc+c}, state::number };
            case 'a'...'z': return {{ lexeme::ERROR, c},    state::start };
            default:        return {{ lexeme::NUMBER, acc}, state::start };
        }
    };

    state_result relop(char c, const std::string& acc)
    {
        switch (c)
        {
            case '=': return {{ lexeme::RELATIONAL_OPERATOR, acc+c}, state::space };
            default : return {{ lexeme::RELATIONAL_OPERATOR, acc},   state::start };
        }
    };

    state_result assign(char c, const std::string& acc)
    {
        switch (c)
        {
            case '=': return {{ lexeme::ASSIGNATION, acc+c}, state::space };
            default : return {{ lexeme::ERROR, acc+c}, state::start };
        }
    };

    state_result space(char c, const std::string& acc)
    {
        return {{lexeme::UNDETERMINATED, ""}, state::start };
    };
}




#pragma once

#include <string>
#include <memory>
#include <functional>
#include "lexeme.hpp"

namespace state 
{    
    struct state_result
    {
        state_result();
        state_result(const lexeme& lex, const std::function<state_result(char, const std::string&)>& next_transition);
        
        lexeme lex {};
        std::function<state_result(char, const std::string&)> next_transition {};
    };

    state_result start(char c, const std::string& acc = "");
    state_result identifier(char c, const std::string& acc);
    state_result number(char c, const std::string& acc);
    state_result relop(char c, const std::string& acc);
    state_result assign(char c, const std::string& acc);
    state_result space(char c, const std::string& acc);
};
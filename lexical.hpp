#pragma once
#include <iostream>
#include <functional>
#include <sstream>
#include <cctype>

struct lexeme
{
    enum token
    {
        PROCESSING, SPACE, IDENTIFIER, NUMBER, OPERATOR, RELOP, ASSIGN, ERROR
    };

    lexeme(token type, const std::string& value)
        : type { type }, value { value }
    {

    }

    lexeme(token type, char value)
        : type { type }, value { std::string(1, value) }
    {

    }

    token type;
    std::string value;
};

class state
{
public:

    struct forward
    {
        std::function<forward(char, const std::string&)> transition { state::start };
        lexeme token { lexeme::token::PROCESSING, "" };
    };

    static forward start(char next, const std::string& acc = "")
    {
        switch (next)
        {
            case 'a'...'z': return { .transition =  identifier, .token =  {lexeme::PROCESSING, next} };
            case '0'...'9': return { .transition =  number,     .token =  {lexeme::PROCESSING, next} };
            case '+':
            case '-':       return {                            .token =  {lexeme::OPERATOR,   next} };
            case '=':       return {                            .token =  {lexeme::RELOP,      next} };
            case '<':
            case '>':       return { .transition =  relop,      .token =  {lexeme::PROCESSING, next} };
            case ':':       return { .transition =  assign,     .token =  {lexeme::PROCESSING, next} };
            case ' ':
            case '\n':
            case '\t':      return {                            .token =  {lexeme::PROCESSING, next} };
            default:        return {                            .token =  {lexeme::ERROR,      next} };
        }
    }

    static forward identifier(char next, const std::string& acc = "")
    {
        switch (next)
        {
            case 'a'...'z':
            case '0'...'9': return { .transition =  identifier, .token =  {lexeme::PROCESSING, acc+next} };
            default:        return {                            .token =  {lexeme::IDENTIFIER, acc     } };
        }
    }

    static forward number(char next, const std::string& acc = "")
    {
        std::cout << "number\n";
        switch (next)
        {
            case '0'...'9': return { .transition =  number, .token =  {lexeme::PROCESSING, acc+next} };
            case 'a'...'z': return {                        .token =  {lexeme::ERROR,      next    } };
            default:        return {                        .token =  {lexeme::NUMBER,     acc     } };
        }
    }

    static forward relop(char next, const std::string& acc = "")
    {
        switch (next)
        {
            case '=': return { .transition =  space,    .token =  { lexeme::RELOP, acc+next } };
            default : return {                          .token =  { lexeme::RELOP, acc      } };
        }
    }

    static forward assign(char next, const std::string& acc = "")
    {
        switch (next)
        {
            case '=': return { .transition =  space,    .token =  { lexeme::ASSIGN, acc+next } };
            default : return {                          .token =  { lexeme::ERROR,  acc+next } };
        }
    }

    static forward space(char next, const std::string& acc = "")
    {
        return { .token =  { lexeme::PROCESSING, "" } };
    }
};


class lexical
{
public:

    void analisys(std::stringstream& sstream)
    {
        char c;
        state::forward fwd;

        while(sstream.get(c))
        {
            c = std::tolower(c);
            fwd = fwd.transition(c, fwd.token.value);

            if(fwd.token.type == lexeme::ERROR)
            {
                std::cout << "Error: " << fwd.token.value << "\n";
                std::terminate();
            }

            if(fwd.token.type != lexeme::PROCESSING)
            {
                sstream.putback(c);
                lexeme_table.emplace_back(fwd.token.type, fwd.token.value);
            }
        }
    }

    const auto& table() const { return lexeme_table; }

private:

    std::vector<std::pair<lexeme::token, std::string>> lexeme_table;

};

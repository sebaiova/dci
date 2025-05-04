#pragma once

#include <expected>
#include <sstream>
#include <set>
#include <list>
#include "lexeme.hpp"
#include "error.hpp"

struct lexical_analyzer
{
    lexical_analyzer(const std::string& text) : 
        buffer { construct_buffer(text) }
    {
        it = buffer.begin();
    }

    [[nodiscard]] auto operator>>(std::expected<lexeme, error>& other) -> lexical_analyzer&;

    operator bool()
    {
        return it != buffer.end();
    }

private:
    
    constexpr static std::string construct_buffer(const std::string& text)
    {
        return text+"\n";
    }

    char next_char();
    void backtrack();
    void skip_spaces();

    const std::string buffer;
    std::string::const_iterator it;
    std::string::const_iterator start;

    std::size_t line { 1 };
    std::size_t col { 0 }; 
};


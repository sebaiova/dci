#pragma once
#include <optional>
#include <expected>
#include <sstream>
#include <set>
#include <list>
#include "lexemes.hpp"
#include "error.hpp"

struct symbol 
{
    lexeme token;
    std::optional<std::string> attribute;
    std::size_t col;
    std::size_t line;
};

struct lexical_analyzer
{
    lexical_analyzer(const std::string& text) : 
        buffer { construct_buffer(text) }
    {
        it = buffer.begin();
    }

    [[nodiscard]] auto operator>>(std::expected<symbol, error>& other) -> lexical_analyzer&;
    [[nodiscard]] auto next_token() -> std::expected<symbol, error>;

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


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
    lexeme token {lexeme::UNDETERMINATED};
    std::optional<std::string> attribute;
    std::size_t col {0};
    std::size_t line {0};
};

struct lexical_analyzer
{
    lexical_analyzer(const std::string& text, std::list<symbol>& token_stream, std::set<std::string>& attribute_table) : 
        buffer { construct_buffer(text) },
        token_stream { token_stream },
        attribute_table { attribute_table }
    {
        it = buffer.begin();
    }

    [[nodiscard]] auto operator>>(std::expected<symbol, error>& other) -> lexical_analyzer&;
    [[nodiscard]] auto next_token() -> std::expected<symbol, error>;

    operator bool() { return not eof(); }
    constexpr bool eof() const { return it == buffer.end(); }
    constexpr std::size_t get_line() const { return line; }
    constexpr std::size_t get_col() const { return col; }

private:
    
    constexpr static std::string construct_buffer(const std::string& text)
    {
        return text+"\x1A ";
    }

    char next_char();
    void backtrack();
    void skip_spaces();

    const std::string buffer;
    std::string::const_iterator it;
    std::string::const_iterator start;

    void token_register(const symbol&);
    std::list<symbol>& token_stream;
    std::set<std::string>& attribute_table;

    std::size_t line { 1 };
    std::size_t col { 0 }; 
};


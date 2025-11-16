#pragma once
#include <optional>
#include <expected>
#include <list>
#include "lexemes.hpp"
#include <error.hpp>
#include <semantic_analyzer.hpp>
#include <string>

struct symbol 
{
    lexeme token {lexeme::UNDETERMINATED};
    std::string attribute;
    std::size_t col {0};
    std::size_t line {0};
};

struct lexical_analyzer
{
    lexical_analyzer(const std::string& text, std::list<symbol>& token_stream, semantic_analyzer& semantic) : 
        buffer { construct_buffer(text) },
        token_stream { token_stream },
        _semantic { semantic }
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

    std::expected<void, error> token_register(const symbol&);
    std::list<symbol>& token_stream;
    semantic_analyzer& _semantic;

    std::size_t line { 1 };
    std::size_t col { 0 }; 
};


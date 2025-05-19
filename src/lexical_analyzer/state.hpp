#pragma once

#include <string>
#include <memory>
#include <functional>
#include <expected>
#include "lexemes.hpp"
#include "error.hpp"

namespace state 
{   
    struct result;
    using transition = std::expected<result, error>(*)(char);

    [[nodiscard]] auto start(char c)        -> std::expected<result, error>;
    [[nodiscard]] auto identifier(char c)   -> std::expected<result, error>;
    [[nodiscard]] auto number(char c)       -> std::expected<result, error>;
    [[nodiscard]] auto relop(char c)        -> std::expected<result, error>;
    [[nodiscard]] auto assign(char c)       -> std::expected<result, error>;
    [[nodiscard]] auto comment(char c)      -> std::expected<result, error>;

    template<lexeme T> [[nodiscard]] auto space(char c) -> std::expected<result, error>;

    struct result
    {
        lexeme token { lexeme::UNDETERMINATED };
        transition next_transition { state::start };
    };
};
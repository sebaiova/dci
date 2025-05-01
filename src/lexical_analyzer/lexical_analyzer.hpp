#pragma once

#include <expected>
#include <sstream>
#include <set>
#include <list>
#include "lexeme.hpp"

namespace lexical 
{
    struct result
    {
        std::set<lexeme> lexeme_table;
        std::list<lexeme> lexeme_stream;
    };

    struct error 
    {
        char c; int line; int col;
    };

    [[nodiscard]] auto analyzer(std::stringstream& sstream) -> std::expected<result, error>;
}


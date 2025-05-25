#include "parser.hpp"
#include <lexical_analyzer.hpp>
#include <syntax_analyzer.hpp>
#include <iostream>
#include "logger.hpp"

parser::parser(const std::string& str) : str { str } 
{

};

parser::~parser() 
{
    log_token_stream(token_stream);
    log_attribute_table(attribte_table);
};

auto parser::run() -> std::expected<void, error>
{
    auto lexical { lexical_analyzer(str, token_stream, attribte_table) };
    auto syntax  { syntax_analyzer(lexical) };

    return syntax.start();
}
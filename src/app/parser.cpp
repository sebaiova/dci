#include "parser.hpp"
#include <lexical_analyzer.hpp>
#include <syntax_analyzer.hpp>
#include <semantic_analyzer.hpp>
#include <mepa_generator.hpp>
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
    auto semantical { semantic_analyzer() };
    auto mepa { mepa_generator(semantical) };
    auto lexical { lexical_analyzer(str, token_stream, semantical) };
    auto syntax  { syntax_analyzer(lexical, semantical, mepa) };

    return syntax.start();
}
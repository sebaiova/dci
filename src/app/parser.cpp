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
    if(auto result = lexical_analysis(); !result)
        return result;

    if(auto result = syntax_analysis(); !result)
        return result;

    return {};
}

auto parser::lexical_analysis() -> std::expected<void, error>
{
    lexical_analyzer lexical(str);
    std::expected<lexical_analyzer::output, error> output;

    while(lexical >> output)
    {
        if(not output)
            return std::unexpected(output.error());
        
        if(output->token != lexeme::COMMENT)
            token_stream.push_back(output->token);

        if(output->attribute)
            attribte_table.insert(*output->attribute);
    }

    std::cout << "Lexical analysis completed successfully.\n";
    return {};
}

auto parser::syntax_analysis() -> std::expected<void, error>
{
    syntax_analyzer syntax(token_stream.begin());
    return syntax.start();
}

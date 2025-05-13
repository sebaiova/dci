#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>
#include <lexical_analyzer.hpp>
#include "debug.hpp"

std::set<std::string> identifiers;
std::list<lexeme> token_stream;

[[nodiscard]] auto open(const std::string& file_name) -> std::expected<std::string, error>
{
    std::ifstream file(file_name);
    
    if(file.is_open())
    {
        return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }

    return std::unexpected(error("Input file not found."));
}

[[nodiscard]] auto parse(const std::string& string) -> std::expected<bool, error>
{
    lexical_analyzer lexical(string);
    std::expected<lexical_analyzer::output, error> output;

    while(lexical >> output)
    {
        if(not output)
            return std::unexpected(output.error());
        
        if(output->token != lexeme::COMMENT)
            token_stream.push_front(output->token);

        if(output->attribute)
            identifiers.insert(*output->attribute);
    }
    return true;
}

bool print_error(error& err)
{
    std::cout << err.msg << "\n";
    return true;
}

int main(int argc, char** argv)
{
    if(argc<2)
        std::cout << "No input file\n";
    else
    {
        auto success { open(argv[1]).and_then(parse) };
        //print_symbol_table(identifiers);
        //print_token_stream(token_stream);

        if(not success)
            std::cout << success.error().msg << "\n";
        else{
            std::cout << "Lexical analysis completed successfully.\n";
            std::cout << "Number of identifiers: " << identifiers.size() << "\n";
            std::cout << "Number of tokens: " << token_stream.size() << "\n";
        }
    }

    return 0;
}

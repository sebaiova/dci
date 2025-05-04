#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>
#include <lexical_analyzer.hpp>


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
    std::expected<lexeme, error> token;

    while(lexical >> token)
    {
        if(not token)
            return std::unexpected(token.error());
        std::cout << token.value() << "\n";
    };
    return true;
}

bool print_error(error& err)
{
    std::cout << err.msg << "\n";
    return true;
}

int main(int argc, char** argv)
{
    if(0)
        std::cout << "No input file\n";
    else
    {
        auto success { open("../../example.txt").and_then(parse) };
        if(not success)
            std::cout << success.error().msg << "\n";
    }

    return 0;
}

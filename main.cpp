#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>
#include "lexical.hpp"

struct error
{
    error(const std::string& msg)
    {
        std::cerr << msg << "\n";
    }
};

[[nodiscard]] auto open(const std::string& file_name) -> std::expected<std::stringstream, error>
{
    std::ifstream file(file_name);

    if(file.is_open())
    {
        std::stringstream sstring;
        sstring << file.rdbuf();
        return sstring;
    }

    return std::unexpected(error("Input file not found."));
}

[[nodiscard]] auto parse(std::stringstream sstring) -> std::expected<bool, error>
{
    std::cout << sstring.str() << std::endl;

    lexical lex;
    lex.analisys(sstring);

    auto& table { lex.table() };
    for(auto& t : table)
        std::cout << t.first << "\t" << t.second << "\n";


    return true;
}

int main(int argc, char** argv)
{
    if(0)
        std::cout << "No input file\n";
    else
    {
        open("example.txt").and_then(parse);
    }

    return 0;
}

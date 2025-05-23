#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>
#include "print_error.hpp"
#include "parser.hpp"

[[nodiscard]] auto open(const std::string& file_name) -> std::expected<std::string, error>
{
    std::ifstream file(file_name);

    if(file.is_open())
        return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    
    return std::unexpected(error("Input file not found."));
}

int main(int argc, char** argv)
{
    if(argc<2)
        std::cout << "No input file\n";
    else 
    {
        auto success { open(argv[1]).and_then([](auto str){ 
                parser p(str); 
                return p.run(); 
            })};

        if(not success)
            print_error(success.error());
    }
    return 0;
}

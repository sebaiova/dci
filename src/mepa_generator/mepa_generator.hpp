#pragma once 

#include <expected>
#include <error.hpp>
#include <iostream>

struct mepa_generator 
{
    std::expected<void, error> program()
    {
        std::cout << "Mepa Hello!\n";
        return {};
    }

};
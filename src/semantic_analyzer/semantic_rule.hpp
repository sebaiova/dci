#pragma once

#include <functional>
#include <expected>
#include <error.hpp>
#include <iostream>

namespace semantic_rule 
{
    auto hella = []() -> std::expected<void, error> {
            std::cout << "hella semantic rule!\n";
            return {};
        };
}
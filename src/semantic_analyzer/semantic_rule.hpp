#pragma once

#include <functional>
#include <expected>
#include <error.hpp>
#include <iostream>
#include <symbol_table.hpp>

namespace semantic_rule 
{
    auto hella = []() -> std::expected<void, error> {
            std::cout << "hella semantic rule!\n";
            return {};
        };


    auto insert_symbol = [](const std::string& name, const std::string& type, SymbolTable& st)
        -> std::expected<void, error>
    {
            if (!st.insert(name, type)) {
                return std::unexpected(error("Semantic Error: Duplicate symbol '" + name + "'"));
            }
        std::cout << "Inserted symbol: " << name << " of type " << type << "\n";
        return {};
    };

    auto create_program_symbol_table = [](const std::string& programName) 
        -> std::shared_ptr<SymbolTable>
    {
        auto st = std::make_shared<SymbolTable>();
        auto result = insert_symbol(programName, "program", *st);
        if (!result) {
            std::cerr << result.error().what() << "\n";
        }
        return st;
    };

}


#pragma once
#include <unordered_map>
#include <string>
#include <memory>

struct symbol_table {
    std::unordered_map<std::string, std::string> table;
    std::shared_ptr<SymbolTable> parent = nullptr; // optional, for nested scopes

    bool insert(const std::string& name, const std::string& type) {
        if (table.find(name) != table.end()) return false;
        table[name] = type;
        return true;
    }

    bool exists(const std::string& name) const {
        if (table.find(name) != table.end()) return true;
        if (parent) return parent->exists(name);
        return false;
    }
};

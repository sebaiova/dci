#pragma once
 
#include <map>
#include <string>
#include <vector>

struct symbol_table 
{
    enum class type { VOID, PROGRAM, PROCEDURE, FUNCTION, INTEGER, BOOLEAN };

    bool push(const std::string& symbol, type symbol_type)
    {
        /* Si se inserta (no duplicados) retorna true */
        return _types.insert({symbol, symbol_type}).second;
    }

    void push_fparam(const std::string& subrutine, type symbol_type)
    {
        _fparams_types[subrutine].push_back(symbol_type);
    }

    bool check(const std::string& symbol) const
    {
        return _types.contains(symbol);
    }

    type get_type(const std::string& symbol) const
    {
        return _types.at(symbol);
    }

    void set_scope_return(type t)
    {
        scope_return = t;
    }

    type scope_return = type::VOID;
    std::map<std::string, type> _types {};
    std::map<std::string, std::vector<type>> _fparams_types {};
};

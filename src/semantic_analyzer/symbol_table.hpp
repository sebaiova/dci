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
        bool inserted = _types.insert({symbol, symbol_type}).second;
        if(inserted && (symbol_type == type::FUNCTION || symbol_type == type::PROCEDURE))
            _fparams_types.insert({symbol, {}});


        return inserted;
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

    std::vector<type> get_fparams(const std::string& subrutine_symbol) const
    {
        return _fparams_types.at(subrutine_symbol);
    }

    void set_scope_return(type t)
    {
        scope_return = t;
    }

    void set_return(const std::string& symbol, type t)
    {
        _return_types[symbol] = t;   
    }

    type get_return(const std::string& symbol) const
    {
        return _return_types.at(symbol);
    }


    type scope_return = type::VOID;
    std::map<std::string, type> _types {};
    std::map<std::string, std::vector<type>> _fparams_types {};
    std::map<std::string, type> _return_types {};
};

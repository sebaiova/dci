#pragma once

#include "error.hpp"
#include <lexemes.hpp>
#include "symbol_table.hpp"
#include <string>
#include <vector>
#include <deque>
#include <expected>
#include <format>

struct semantic_analyzer 
{
    enum class state { READY, PROGRAM, VAR, PRODCEDURE, FUNCTION, FPARAM };

    state _current_state { state::READY };

    std::expected<void, error> analyze_symbol(const std::string& symbol)
    {
        switch (_current_state) 
        {
            case state::READY:      return is_declared(symbol);
            case state::PROGRAM:    _current_scope_name = symbol;   return push_symbol(symbol, symbol_table::type::PROGRAM);
            case state::VAR:        _tmp_vars.push_back(symbol);    return {};
            case state::PRODCEDURE: _current_scope_name = symbol;   return push_symbol(symbol, symbol_table::type::PROCEDURE);
            case state::FUNCTION:   _current_scope_name = symbol;   return push_symbol(symbol, symbol_table::type::FUNCTION);
            case state::FPARAM:     _tmp_fparams.push_back(symbol); return {};
        }
        return {};
    }

    std::expected<void, error>  push_symbol(const std::string& symbol, symbol_table::type symbol_type)
    {
        if(current_scope().push(symbol, symbol_type))
            return {};
        return std::unexpected(error(std::format("Semantic Error: \"{}\" is already defined.", symbol)));
    }

    std::expected<void, error>  push_scope()
    {
        _scopes.push_front({});
        return {};
    }

    std::expected<void, error> pop_scope()
    {
        _scopes.pop_front();
        return {};
    }

    std::expected<void, error> is_declared(const std::string& symbol)
    {
        _last_id = symbol;
        for(auto& scope : _scopes)
            if (scope.check(symbol))
                return {};
        return std::unexpected(std::format("Semantic Error: {} is not declared.", symbol));
    }

    std::expected<void, error> ready()
    {
        _current_state = state::READY;
        return {};
    }

    std::expected<void, error> var()
    {
        _current_state = state::VAR;
        return {};
    }

    std::expected<void, error> fparam()
    {
        _current_state = state::FPARAM;
        return {};
    }

    std::expected<void, error> program()
    {
        _current_state = state::PROGRAM;
        return {};
    }

    std::expected<void, error> integer()
    {
        switch (_current_state)
        {
            case state::VAR: return declare_vars(symbol_table::type::INTEGER);
            case state::FPARAM: return declare_fparams(symbol_table::type::INTEGER);
            default : return {};
        }
    }

    std::expected<void, error> boolean()
    {
        switch (_current_state)
        {
            case state::VAR: return declare_vars(symbol_table::type::BOOLEAN);
            case state::FPARAM: return declare_fparams(symbol_table::type::BOOLEAN);
            default : return {};
        }
    }

    std::expected<void, error> vbool()
    {
        return {};
    }

    std::expected<void, error> vint()
    {
        return {};
    }

    std::expected<void, error> lint()
    {
        if(_last_value != lexeme::NUMBER)
            return std::unexpected(std::format("Semantic Error: Expected number."));

        return {};
    }

    std::expected<void, error> lbool()
    {
        if(_last_value == lexeme::TRUE || _last_value == lexeme::FALSE)
            return {};

        return std::unexpected(std::format("Semantic Error: Expected boolean."));
    }




    std::expected<void, error> declare_fparams(symbol_table::type type)
    {
        for(auto& symbol : _tmp_fparams)
        {
            auto ok = push_symbol(symbol, type);
            if(not ok)
                return std::unexpected(ok.error());
            current_scope().push_fparam(_current_scope_name, type);
        }
        _tmp_fparams.clear();
        return {};
    }

    std::expected<void, error> declare_vars(symbol_table::type type)
    {
        for(auto& symbol : _tmp_vars)
        {
            auto ok = push_symbol(symbol, type);
            if(not ok) 
                return std::unexpected(ok.error());
        }
        _tmp_vars.clear();
        return {};
    }

    std::expected<void, error> procedure()
    {
        _current_state = state::PRODCEDURE;
        return {};
    }

    std::expected<void, error> function()
    {
        _current_state = state::FUNCTION;
        return {};
    }

    std::expected<void, error> exp()
    {
        return {};
    }


    std::expected<void, error> nfaci()
    {
        _expected_fac = symbol_table::type::INTEGER;
        return {};
    }

    std::expected<void, error> nfacb()
    {
        _expected_fac = symbol_table::type::BOOLEAN;
        return {};
    }

    std::expected<void, error> faci()
    {
        if( _expected_fac != symbol_table::type::VOID && _expected_fac != symbol_table::type::INTEGER)
        {
            _expected_fac = symbol_table::type::VOID;
            return std::unexpected(error(std::format("Semantic Error: Expected Boolean.")));
        }
        return {};
    }

    std::expected<void, error> facb()
    {
        if( _expected_fac != symbol_table::type::VOID && _expected_fac != symbol_table::type::BOOLEAN)
        {
            _expected_fac = symbol_table::type::VOID;
            return std::unexpected(error(std::format("Semantic Error: Expected Integer.")));
        }
        return {};
    }

    symbol_table& current_scope() { return _scopes.front(); }

    bool exp_bool;

    std::string _last_id {};
    lexeme _last_value{};

    symbol_table::type _expected_fac { symbol_table::type::VOID };

    std::string _current_scope_name {};
    std::vector<std::string> _tmp_vars    {};
    std::vector<std::string> _tmp_fparams {};
    std::deque<symbol_table> _scopes { 1 };
};

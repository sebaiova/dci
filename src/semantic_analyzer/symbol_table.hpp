#pragma once
 
#include <map>
#include <string>
#include <vector>
#include <iostream>

struct symbol_table 
{
    enum class type { VOID, PROGRAM, PROCEDURE, FUNCTION, INTEGER, BOOLEAN };

    symbol_table(const std::string& scope_name) : _scope_name { scope_name }
    {

    }

    const std::string& get_name() const { return _scope_name; }

    bool push(const std::string& symbol, type symbol_type)
    {
        /* Si se inserta (no duplicados) retorna true */
        bool inserted = _types.insert({symbol, symbol_type}).second;
        
        if (inserted) {
            // 1. Asignar dirección de memoria solo a elementos que requieren espacio (vars/params/subrutinas).
            // NOTA: PROGRAM, PROCEDURE, FUNCTION reciben una dirección (donde inicia su código)
            //       INTEGER y BOOLEAN (variables/parámetros) reciben una dirección de datos.

            if (symbol_type == type::INTEGER || symbol_type == type::BOOLEAN) {
                _addresses[symbol] = _current_address++; // Asignar la próxima dirección libre
            } else if (symbol_type == type::FUNCTION || symbol_type == type::PROCEDURE) {
                _fparams_types.insert({symbol, {}});
                // Se podría reservar una dirección de código (ej. una etiqueta) aquí si fuera necesario
                // Pero, por ahora, solo necesitamos contar variables de datos.
            }
        }
        
        return inserted;
    }

    void push_fparam(const std::string& subrutine, type symbol_type)
    {
        _fparams_types[subrutine].push_back(symbol_type);
    }

    bool push_fparam_symbol(const std::string& symbol, type symbol_type)
    {
        // 1. Verificar duplicados en el ámbito actual
        bool inserted = _types.insert({symbol, symbol_type}).second;
        if (inserted)
        {
            // 2. Asignar la dirección negativa (de forma decremental)
            if (symbol_type == type::INTEGER || symbol_type == type::BOOLEAN)
            {
                _addresses[symbol] = _current_fparam_address;
                _current_fparam_address--; // Decrementa para el siguiente parámetro (-2, -3, -4...)
                return true;
            }
        }
        return inserted;
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

    size_t count_variables() const
    {
        size_t count = 0;
        for (const auto& pair : _addresses) {
            // Solo contamos variables locales (direcciones >= 0)
            // Excluye parámetros formales (direcciones < 0)
            if (pair.second >= 0) { 
                count++;
            }
        }
        return count;    
    }
    
    int get_address(const std::string& symbol) const
    {
        if (_addresses.contains(symbol)) {
            return _addresses.at(symbol);
        }
        // Devolver un valor que indique error o no encontrado si es necesario
        return -1; 
    }

    int _current_address = 0;
    int _current_fparam_address = -2;
    std::string _scope_name;
    type scope_return = type::VOID;
    std::map<std::string, int> _addresses {};
    std::map<std::string, type> _types {};
    std::map<std::string, std::vector<type>> _fparams_types {};
    std::map<std::string, type> _return_types {};
};

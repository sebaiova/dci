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
            // Asignar dirección de memoria solo a variables locales (desplazamiento >= 0).
            // NOTA: Los parámetros formales usan direcciones negativas.
            if (symbol_type == type::INTEGER || symbol_type == type::BOOLEAN) {
                _addresses[symbol] = _current_address++; // Asignar la próxima dirección libre
            } else if (symbol_type == type::FUNCTION || symbol_type == type::PROCEDURE) {
                // Inicializar la lista de parámetros para la subrutina
                _fparams_types.insert({symbol, {}});
            }
        }
        
        return inserted;
    }

    void push_fparam(const std::string& subrutine, type symbol_type)
    {
        // Almacena el tipo del parámetro formal en la definición de la subrutina
        _fparams_types[subrutine].push_back(symbol_type);
    }

    bool push_fparam_symbol(const std::string& symbol, type symbol_type)
    {
        // 1. Verificar duplicados en el ámbito actual
        bool inserted = _types.insert({symbol, symbol_type}).second;
        if (inserted)
        {
            // 2. Asignar la dirección negativa para parámetros (-1, -2, -3...)
            if (symbol_type == type::INTEGER || symbol_type == type::BOOLEAN)
            {
                _addresses[symbol] = _current_fparam_address;
                _current_fparam_address--; // Decrementa para el siguiente parámetro
                return true;
            }
        }
        return inserted;
    }

    bool push_return_symbol(const std::string& symbol, type symbol_type)
    {
        _addresses[symbol] = _current_fparam_address;
        _current_fparam_address--; // Decrementa para el siguiente parámetro
        return true;
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
        if (_fparams_types.contains(subrutine_symbol)) {
            return _fparams_types.at(subrutine_symbol);
        }
        return {};
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
        if (_return_types.contains(symbol)) {
            return _return_types.at(symbol);
        }
        return type::VOID;
    }

    size_t count_variables() const
    {
        size_t count = 0;
        for (const auto& pair : _addresses) {
            // Solo contamos variables locales (direcciones >= 0),
            // excluyendo parámetros formales (direcciones < 0).
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
        return -9999; // Usar un valor distinto de -1 para indicar claramente un fallo
    }

    // MEPA: Devuelve el desplazamiento 'n' de un símbolo
    int get_offset(const std::string& symbol) const
    {
        return get_address(symbol);
    }

    int _current_address = 0; // Para variables locales: 0, 1, 2, ...
    int _current_fparam_address = -3; // [CORREGIDO] Para parámetros: -1, -2, -3, ... (El primer parámetro es D[L] - 1)
    std::string _scope_name;
    type scope_return = type::VOID;
    std::map<std::string, int> _addresses {}; // Desplazamiento (offset)
    std::map<std::string, type> _types {};
    std::map<std::string, std::vector<type>> _fparams_types {};
    std::map<std::string, type> _return_types {};
};

#pragma once

#include <error.hpp>
#include <lexemes.hpp>
#include "symbol_table.hpp"
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <expected>
#include <format>
#include <iostream>

struct semantic_analyzer 
{
    enum class state { READY, PROGRAM, VAR, PRODCEDURE, FUNCTION, FPARAM };

    state _current_state { state::READY };

    std::expected<void, error> analyze_symbol(const std::string& symbol)
    {
        switch (_current_state) 
        {
            case state::READY:      return is_declared(symbol);
            case state::PROGRAM:    _current_scope_name = symbol; _scopes.push_front(symbol_table(_current_scope_name)); return push_symbol(symbol, symbol_table::type::PROGRAM);
            case state::VAR:        _tmp_vars.push_back(symbol);    return {};
            case state::PRODCEDURE: _current_scope_name = symbol;   return push_symbol(symbol, symbol_table::type::PROCEDURE);
            case state::FUNCTION:   _current_scope_name = symbol;   return push_symbol(symbol, symbol_table::type::FUNCTION);
            case state::FPARAM:     _tmp_fparams.push_back(symbol); return {};
        }
        return {};
    }

    std::expected<void, error> push_symbol(const std::string& symbol, symbol_table::type symbol_type)
    {
        if(current_scope().push(symbol, symbol_type))
            return {};
        return std::unexpected(error(std::format("Semantic Error: \"{}\" is already defined.", symbol)));
    }

    std::expected<void, error> push_scope()
    {
        _scopes.push_front({_current_scope_name});
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
        return std::unexpected(std::format("Semantic Error: \"{}\" is not declared.", symbol));
    }

    symbol_table::type get_type(const std::string& symbol) const
    {
        for(auto& scope : _scopes)
            if (scope.check(symbol))
                return scope.get_type(symbol);

        return symbol_table::type::VOID;
    }

    std::vector<symbol_table::type> get_fparams(const std::string& symbol) const
    {
        for(auto& scope : _scopes)
            if (scope.check(symbol))
                return scope.get_fparams(symbol);
        return {};
    }

    symbol_table::type get_return(const std::string& symbol) const
    {
        for(auto& scope : _scopes)
            if (scope.check(symbol))
                return scope.get_return(symbol);
        return symbol_table::type::VOID;
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

    std::expected<void, error> set_return(symbol_table::type type)
    {
        current_scope().push_return_symbol(_current_scope_name, type);

        for(auto& scope : _scopes)
            if (scope.check(_current_scope_name))
            {
                scope.set_return(_current_scope_name, type);
            }
        return {};
    }

    std::expected<void, error> integer()
    {
        switch (_current_state)
        {
            case state::VAR: return declare_vars(symbol_table::type::INTEGER);
            case state::FPARAM: return declare_fparams(symbol_table::type::INTEGER);
            case state::FUNCTION: return set_return(symbol_table::type::INTEGER);
            default : return {};
        }
    }

    std::expected<void, error> boolean()
    {
        switch (_current_state)
        {
            case state::VAR: return declare_vars(symbol_table::type::BOOLEAN);
            case state::FPARAM: return declare_fparams(symbol_table::type::BOOLEAN);
            case state::FUNCTION: return  set_return(symbol_table::type::BOOLEAN);
            default : return {};
        }
    }

    std::expected<void, error> lint()
    {
        if(_last_type == symbol_table::type::INTEGER)
            return {};

        return std::unexpected(std::format("Semantic Error: Expected integer."));
    }

    std::expected<void, error> lbool()
    {
        if(_last_type == symbol_table::type::BOOLEAN)
            return {};

        return std::unexpected(std::format("Semantic Error: Expected boolean."));
    }

    std::expected<void, error> lproc()
    {
        if(get_type(_last_id) == symbol_table::type::PROCEDURE)
        {
            auto fparams = get_fparams(_last_id);
            _tmp_aparams.emplace(fparams.begin(), fparams.end()); 
            return {};
        }
        return std::unexpected(std::format("Semantic Error: \"{}\" is not a procedure.", _last_id));
    }

    std::expected<void, error> lfunc()
    {
        if(get_type(_last_id) == symbol_table::type::FUNCTION)
        {
            auto fparams = get_fparams(_last_id);
            _tmp_aparams.emplace(fparams.begin(), fparams.end()); 
            return {};
        }

        return std::unexpected(std::format("Semantic Error: \"{}\" is not a function.", _last_id));
    }

    std::expected<void, error> checkf()
    {
        if((not _called.empty()) && _called.top()==_last_id)
        {
            if(_callcheck.top() == 1)
                return std::unexpected(std::format("Semantic Error: \"{}\" require () for call.", _last_id));
            _callcheck.pop();
        } 
        return {};
    }

    std::expected<void, error> check_param()
    {
        if(_tmp_aparams.top().empty())
            return std::unexpected(std::format("Semantic Error: Incorrect number of parameters."));

        if(_tmp_aparams.top().front()!=_last_type)
            return std::unexpected(std::format("Semantic Error: Type mismatch in subrutine call."));

        _tmp_aparams.top().pop_front();
        return {};
    }

    std::expected<void, error> check_call()
    {
        if(not _tmp_aparams.top().empty())
            return std::unexpected(std::format("Semantic Error: Incorrect number of parameters."));
        _tmp_aparams.pop();
        _called.pop();
        
        _callcheck.top() = 0;
        return {};
    }

    std::expected<void, error> declare_fparams(symbol_table::type type)
    {
        for(auto it = _tmp_fparams.rbegin(); it != _tmp_fparams.rend(); ++it)
        {
            auto symbol = *it;
            auto ok = current_scope().push_fparam_symbol(symbol, type);
            if(not ok)
                return std::unexpected(error(std::format("Semantic Error: \"{}\" is already defined.", symbol)));

            for(auto& scope : _scopes)
                if (scope.check(_current_scope_name))
                    scope.push_fparam(_current_scope_name, type);
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

    std::expected<void, error> nint()
    {
        _next_type.top()=symbol_table::type::INTEGER;
        return {};
    }

    std::expected<void, error> nbool()
    {
        _next_type.top()=symbol_table::type::BOOLEAN;
        return {};
    }

    std::expected<void, error> faci()
    {
        _last_type =  symbol_table::type::INTEGER;
        _expression_type = symbol_table::type::INTEGER;
        _assigned_type = _last_type;
        return check_expected(_last_type);
    }

    std::expected<void, error> facs()
    {
        _last_type = get_type(_last_id);

        if(_last_type == symbol_table::type::FUNCTION || _last_type == symbol_table::type::PROCEDURE)
        {
            _called.push(_last_id);         
            _callcheck.push(1);                   
        }

        if(_last_type == symbol_table::type::FUNCTION)
            _last_type = get_return(_last_id);

        _expression_type = _last_type;
        _assigned_type = _last_type;
        return check_expected(_last_type);
    }

    std::expected<void, error> facb()
    {
        _last_type = symbol_table::type::BOOLEAN;
        _expression_type = symbol_table::type::BOOLEAN;
        _assigned_type = _last_type;
        return check_expected(_last_type);
    }

    std::expected<void, error> opencall()
    {
        _last_type = get_return(_last_id);
        _return_type.push(_last_type);
        return check_expected(_last_type);
    }

    std::expected<void, error> closecall()
    {
        _last_type = _return_type.top();
        _assigned_type = _last_type;
        _return_type.pop();
        return check_expected(_last_type);
    }

    std::expected<void, error> check_expected(symbol_table::type type)
    {
        switch(_next_type.top())
        {
            case symbol_table::type::INTEGER: return check_expected_int(type);
            case symbol_table::type::BOOLEAN: return check_expected_bool(type);
            default: return {};
        }
    }

    std::expected<void, error> check_expected_int(symbol_table::type type)
    {
        if(symbol_table::type::INTEGER != type)
            return std::unexpected(error("Semantic Error: Expected Integer."));
        _next_type.top() = symbol_table::type::VOID;
        return {};
    }

    std::expected<void, error> check_expected_bool(symbol_table::type type)
    {    
        if(symbol_table::type::BOOLEAN != type)
            return std::unexpected(error("Semantic Error: Expected Boolean."));
        _next_type.top() = symbol_table::type::VOID;
        return {};
    }

    std::expected<void, error> openp()
    {
        _next_type.push(symbol_table::type::VOID);
        return {};
    }

    std::expected<void, error> closep()
    {
        _next_type.pop();
        return {};
    }

    std::expected<void, error> assign_open()
    {
        _assign_type = _last_type;
        _assign_id = _last_id;

        if(get_return(_assign_id) != symbol_table::type::VOID && _assign_id != _current_scope_name)
            return std::unexpected(error("Semantic Error! Can not assign to non owned function."));
        
        if(_current_scope_name==_last_id)
        {
            _called.pop();
            _callcheck.pop();
        }
        
        return {};
    }

    std::expected<void, error> assign_close()
    {
        if(_assign_type==_assigned_type)
            return {};

        if(_assign_type==symbol_table::type::INTEGER)
            return std::unexpected(error("Semantic Error! Can not assign boolean to integer."));

        if(_assign_type==symbol_table::type::BOOLEAN)
            return std::unexpected(error("Semantic Error! Can not assign integer to boolean."));

        if(_assign_type==symbol_table::type::PROCEDURE)
            return std::unexpected(error("Semantic Error! Can not assign to procedure."));

        if(_assign_type==symbol_table::type::FUNCTION)
        {
           // if(_assign_id != _current_scope_name)
           //     return std::unexpected(error("Semantic Error! Can not assign to non owned function."));
            if(get_return(_assign_id)!=_assigned_type)
                return std::unexpected(error("Semantic Error! Return value type missmatch."));
        }
        return {};
    }

    std::expected<void, error> expi()
    {
        _expression_type = symbol_table::type::INTEGER;
        return {};
    }

    std::expected<void, error> expb()
    {
        _expression_type = symbol_table::type::BOOLEAN;
        return {};
    }


    std::expected<void, error> exp()
    {
        if(_next_type.top()==symbol_table::type::VOID)
            return {};

        return check_expected(_expression_type);
        return {};
    }

    // Nuevo método para que el generador de código verifique si se está asignando al retorno de la función
    bool is_function_return_assignment(const std::string& symbol_id) const
    {
        // 1. Debe estar en un ámbito de función
        if (_current_state != state::FUNCTION && _current_state != state::PRODCEDURE) {
             // Si el estado es VAR/READY/FPARAM, no estamos dentro del cuerpo de la función/procedimiento
             // donde el nombre del scope es el actual. Necesitamos el nombre del scope actual.
             // Asumo que _scopes.front() es el scope actual.
        }
        
        // 2. Comprobar si el símbolo es el nombre del ámbito más interno (el actual).
        if (!_scopes.empty() && symbol_id == _scopes.front().get_name()) {
            
            // 3. Verificar que ese ámbito es una FUNCIÓN.
            // Para verificar el tipo del ámbito actual, primero necesitamos encontrarlo en un scope superior
            // ya que el nombre de la función/procedimiento solo se registra en el scope padre.
            
            // Buscamos el nombre del scope actual en el scope superior (si existe)
            if (_scopes.size() > 1) {
                const symbol_table& parent_scope = _scopes[1];
                if (parent_scope.check(symbol_id)) {
                    symbol_table::type scope_type = parent_scope.get_type(symbol_id);
                    return scope_type == symbol_table::type::FUNCTION;
                }
            }
        }
        return false;
    }


          symbol_table& current_scope()         { return _scopes.front(); }
    const symbol_table& current_scope() const   { return _scopes.front(); }

    size_t get_local_var_count(const std::string& scope_name) const
    {
        return current_scope().count_variables(); 
    }

    int get_address(const std::string& symbol) const
    {
        for(auto& scope : _scopes)
            if (scope.check(symbol))
                return scope.get_address(symbol); // Asumiendo que get_address devuelve la dirección MEPA/memoria
        
        return -9999; // Usar el valor de error de symbol_table
    }

    // MEPA: Devuelve el nivel léxico del ámbito actual (profundidad).
    // Si _scopes.size() es 1 (solo global), devuelve 0.
    int get_current_level() const
    {
        if (_scopes.empty()) return 0; // Debería ser imposible si 'global' está siempre.
        return _scopes.size() - 1;
    }

    // MEPA: Devuelve la diferencia de nivel léxico (m) entre el ámbito actual (_scopes[0])
    // y el ámbito donde se declara el símbolo (i).
    int get_level(const std::string& symbol) const
    {
        // 1. Encontrar el offset del símbolo
        int offset = get_offset(symbol);

        // 2. Caso especial: Si el offset es negativo (-2, -3, ...), es un PARÁMETRO FORMAL.
        // El parámetro se encuentra en el marco de activación inmediatamente superior (el llamador).
        if (offset < 0 && offset != -9999) {
            return 1; // La diferencia de nivel (m) es siempre 1 para acceder al llamador.
        }

        int level = _scopes.size();
        for(auto& scope : _scopes)
        {
            level--;
            if (_scopes[i].check(symbol))
                return level;   
        }

        return -9999; // Símbolo no encontrado (usamos el valor de error)
    }

    // MEPA: Devuelve el desplazamiento (n) del símbolo dentro de su marco de activación.
    int get_offset(const std::string& symbol) const
    {
        for(auto& scope : _scopes) {
            if ( scope._addresses.contains(symbol) ) {
                // get_offset en symbol_table devuelve el desplazamiento (n)
                return scope.get_offset(symbol); 
            }
        }
        return -9999; // Símbolo no encontrado
    }

    std::expected<void, error>  is_writable()
    {
        if(get_type(_last_id)==symbol_table::type::INTEGER)
            return {};
        return std::unexpected(error(std::format("Semantic Error: Cannot read into \"{}\".", _last_id)));
    }

    std::string _last_id {};
    std::string _assign_id {};
    lexeme _last_value{};
    std::string _last_attribute {};

    symbol_table::type _assign_type { symbol_table::type::VOID };
    symbol_table::type _assigned_type { symbol_table::type::VOID };
    std::stack<symbol_table::type> _return_type {};

    symbol_table::type _expression_type { symbol_table::type::VOID };


    symbol_table::type _last_type{ symbol_table::type::VOID };
    std::stack<symbol_table::type> _next_type { std::deque<symbol_table::type>{ symbol_table::type::VOID } };

    std::string _current_scope_name {};
    std::stack<std::deque<symbol_table::type>> _tmp_aparams {};
    std::stack<std::string> _called {};
    std::vector<std::string> _tmp_vars    {};
    std::vector<std::string> _tmp_fparams {};
    std::deque<symbol_table> _scopes { };

    std::stack<int> _callcheck {};
};

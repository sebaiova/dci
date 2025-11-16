#pragma once 

#include <iostream>
#include <fstream>
#include <stack>
#include <semantic_analyzer.hpp>

struct mepa_generator 
{
    mepa_generator(const semantic_analyzer& sem) : _sem { sem } 
    {

    }

    void program_begin()
    {
        file << "INPP\n";
        file << "JMP MAIN\n";
    }

    void program_end()
    {
        file << "PARA\n";
    }

    void program_tag()
    {
        file << std::format("MAIN:\n", _sem._current_scope_name);
    }
// ----------------------------------------------------------------------
// 2. ESTRUCTURAS DE CONTROL DE FLUJO
// ----------------------------------------------------------------------

    // G(if_begin) - Después de evaluar la EXPRESION y S(lbool)
    void if_begin()
    {
        // Genera JUMP-F (JUMP-FALSE) a la etiqueta de salto (ELSE o IF_END).
        file << std::format("JMPF L{}\n", label_counter);
        label_stack.push(label_counter);
        label_counter++;
    }

    // G(else_begin) - Antes de la SENTENCIA del ELSE
    void else_begin()
    {
        // Recupera la etiqueta L_if_skip (donde debería saltar el JMPF).
        int if_label = label_stack.top();
        label_stack.pop();

        // 1. Genera JUMP incondicional para saltar el bloque ELSE.
        file << std::format("JMP L{}\n", label_counter);
        
        // 2. Coloca la etiqueta L_if_skip para el inicio del ELSE.
        file << std::format("L{}:\n", if_label);
        
        // 3. Empuja la nueva etiqueta (L_else_end) para el final de la estructura.
        label_stack.push(label_counter);
        label_counter++;
    }

    // G(if_end) - Al final de SENTENCIA_ELSE (o SENTENCIA si no hay ELSE)
   void if_end()
    {
        // Recupera la etiqueta L_end.
        int end_label = label_stack.top();
        label_stack.pop();
        
        // Coloca la etiqueta de final.
        file << std::format("L{}:\n", end_label);
    }

    // G(while_mark) - Antes de la EXPRESION en el WHILE
    void while_mark()
    {
        // Marca la etiqueta de inicio del bucle.
        label_stack.push(label_counter);
        file << std::format("L{}:\n", label_counter);
        label_counter++;
    }

    // G(while_begin) - Después de evaluar la EXPRESION y S(lbool)
    void while_begin()
    {
        // Genera JUMP-F (JUMP-FALSE) para salir del bucle.
        file << std::format("JMPF L{}\n", label_counter);

        // Guarda la etiqueta de salida del bucle (L_exit).
        label_stack.push(label_counter);
        label_counter++;
    }

    // G(while_end) - Al final de la SENTENCIA
    void while_end()
    {
        // 1. Recupera la etiqueta de salida (L_exit).
        int exit_label = label_stack.top();
        label_stack.pop();

        // 2. Recupera la etiqueta de inicio (L_start).
        int start_label = label_stack.top();
        label_stack.pop();

        // 3. Genera el salto incondicional al inicio del bucle.
        file << std::format("JMP L{}\n", start_label);

        // 4. Coloca la etiqueta de salida.
        file << std::format("L{}:\n", exit_label);
    }

// ----------------------------------------------------------------------
// 3. EXPRESIONES, ASIGNACIÓN Y CARGA
// ----------------------------------------------------------------------

    // G(load_const) - Carga una constante (número, TRUE, FALSE)
    // Nota: Necesita que S() pase el valor. Aquí simulamos pasando 0.
    void load_const()
    {
        file << std::format("PUSH {}\n", _sem._last_attribute); // Si _last_attribute contiene el número
        //file << "PUSH [valor]\n"; 
    }

    // G(load_var) - Carga el valor de una variable (IDENTIFIER)
    void load_var()
    {
        // Usar la última ID verificada para obtener su dirección del semántico.
        int dir = _sem.get_address(_sem._last_id);
        if(dir!=-1)    // Si no es id de subrutina
            file << std::format("LOAD {}\n", dir);
    }

    // G(assign) - Asignación (el valor ya está en la pila)
    void assign()
    {
        // Usar el ID de asignación del semántico.
        int dir = _sem.get_address(_sem._assign_id);
    
        if(dir!=-1) // Si no es valor de retorno
            file << std::format("POP {}\n", dir);
    }

    // G(add_op) - Operador de suma
    void add_op() { file << "ADD\n"; }
    
    // G(sub_op) - Operador de resta (binario)
    void sub_op() { file << "SUB\n"; }
    
    // G(mult_op) - Operador de multiplicación
    void mult_op() { file << "MULT\n"; }
    
    // G(div_op) - Operador de división
    void div_op() { file << "DIV\n"; }
    
    // G(or_op) - Operador OR lógico
    void or_op() { file << "OR\n"; }
    
    // G(and_op) - Operador AND lógico
    void and_op() { file << "AND\n"; }

    // G(not_op) - Operador NOT lógico
    void not_op() { file << "NOT\n"; } 

    void eq_op() { file << "EQ\n"; } 
    void neq_op() { file << "NE\n"; } 
    void min_op() { file << "LT\n"; } 
    void maj_op() { file << "GT\n"; } 
    void mineq_op() { file << "LE\n"; } 
    void majeq_op() { file << "GE\n"; } 


// ----------------------------------------------------------------------
// 4. SUBRUTINAS Y LLAMADAS
// ----------------------------------------------------------------------

    // G(proc_entry) - Etiqueta de inicio de un procedimiento o función
    void proc_entry()
    {
        file << std::format("PROC_{}:\n", _sem._current_scope_name);
    }

    void stack_enter()
    {
        // La instrucción ENTER se usa para ganar espacio para variables locales.
        int local_size = _sem.get_local_var_count(_sem._current_scope_name);
        file << std::format("ENTER {}\n", local_size); 
    }

    // G(proc_exit) - Fin de un procedimiento
    void proc_exit()
    {
        // RETURN libera espacio y vuelve al punto de llamada.
        int param_count = _sem.get_fparams(_sem._current_scope_name).size();
        file << std::format("RETURN {}\n", param_count); 
    }

    // G(func_entry) - Igual que proc_entry, se distingue en el manejo del retorno.
    void func_entry() { return proc_entry(); }

    // G(func_exit) - Fin de una función
    void func_exit() { return proc_exit(); }

    // G(proc_call) - Llamada a un procedimiento
    // G(func_call) - Llamada a una función
    // Asumen que los parámetros ya fueron pusheados a la pila.
    void proc_call()
    {
        // Llama a la dirección/etiqueta de la subrutina.
        file << std::format("CALL PROC_{}\n", _sem._called.top());
    }
    
    void func_call()
    {
        // Asume que solo funciones definidas por el usuario.
        file << std::format("CALL PROC_{}\n", _sem._called.top());
    }

    void read()
    {
        file << std::format("PUSH {}\n", _sem.get_address(_sem._last_id));
        file << "READ\n";
    }

    void write()
    {
        file << "WRIT\n";
    }

    ~mepa_generator()
    {
        file.close();
    }


    private:

    void write_instruction(const std::string& instruction)
    {
        _current_line++;
    }

    int _current_line { 1 };
    const semantic_analyzer& _sem;
    std::ofstream file { std::ofstream("output.mep") };
    int label_counter = 0;
    std::stack<int> label_stack;
};
#pragma once 

#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <string>
#include <format>
#include <semantic_analyzer.hpp>

struct mepa_generator 
{
    mepa_generator(const semantic_analyzer& sem) : _sem { sem } 
    {
        // Abrir el archivo de salida
        // Podrías añadir un manejo de errores aquí si la apertura falla
    }

    // ----------------------------------------------------------------------
    // 1. ESTRUCTURA DEL PROGRAMA
    // ----------------------------------------------------------------------

    void program_begin()
    {
        // INPP: Inicializa s = -1, D[0] = 0.
        write_instruction("INPP");
        // DSVS TAG_MAIN: Salto al código principal, después de la definición de procedimientos.
        write_instruction("DSVS L0");
    }

 

    void program_end()
    {
        // PARA: Finaliza la ejecución.
        write_instruction("PARA");
    }

    void program_tag()
    {
        // Coloca la etiqueta de inicio del programa.
        set_tag("L0");
        stack_enter();
    }

// ----------------------------------------------------------------------
// 2. ESTRUCTURAS DE CONTROL DE FLUJO
// ----------------------------------------------------------------------

    // G(if_begin) - Después de evaluar la EXPRESION (valor booleano en M[s])
    void if_begin()
    {
        // Genera DSVF (Desvío si Falso) al TAG que marca el inicio del ELSE o IF_END.
        write_instruction(std::format("DSVF L{}", label_counter));
        label_stack.push(label_counter); // Guarda L_if_skip (para ELSE/IF_END)
        label_counter++;
    }

    // G(else_begin) - Antes de la SENTENCIA del ELSE
    void else_begin()
    {
        // Recupera la etiqueta L_if_skip (donde debería saltar el JMPF original).
        int if_label = label_stack.top();
        label_stack.pop();

        // 1. Genera DSVS (Desvío incondicional) para saltar el bloque ELSE si se ejecutó IF.
        write_instruction(std::format("DSVS L{}", label_counter));
        
        // 2. Coloca la etiqueta L_if_skip (inicio del ELSE o IF_END si no hay ELSE).
        set_tag(std::format("L{}", if_label));
        
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
        set_tag(std::format("L{}", end_label));
    }

    // G(while_mark) - Antes de la EXPRESION en el WHILE
    void while_mark()
    {
        // Marca la etiqueta de inicio del bucle (L_start).
        label_stack.push(label_counter);
        set_tag(std::format("L{}", label_counter));
        label_counter++;
    }

    // G(while_begin) - Después de evaluar la EXPRESION (valor booleano en M[s])
    void while_begin()
    {
        // Genera DSVF (Desvío si Falso) para salir del bucle.
        write_instruction(std::format("DSVF L{}", label_counter));

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

        // 3. Genera el salto incondicional al inicio del bucle (L_start).
        write_instruction(std::format("DSVS L{}", start_label));

        // 4. Coloca la etiqueta de salida (L_exit).
        set_tag(std::format("L{}", exit_label));
    }

// ----------------------------------------------------------------------
// 3. EXPRESIONES, ASIGNACIÓN Y CARGA
// ----------------------------------------------------------------------

    // G(load_const) - Carga una constante (número, TRUE=1, FALSE=0)
    void load_const()
    {
        // APCT k: Apila la constante k.
        auto k = _sem._last_attribute=="true" ? "1" : (_sem._last_attribute=="false" ? "0" : _sem._last_attribute);
        write_instruction(std::format("APCT {}", k));
    }

    void load_var()
    {
        if (_sem.get_type(_sem._last_id) == symbol_table::type::FUNCTION)
        {
            write_instruction("RMEM 1");
            return;
        }

        // MEPA usa APVL m, n: Apila el valor de la variable en M[D[m]+n].
        // La diferencia de nivel 'm' ahora es manejada por _sem.get_level()
        // (y devuelve 1 para parámetros).
        int level = _sem.get_level(_sem._last_id); // Usa la función correcta: get_level
        int offset = _sem.get_offset(_sem._last_id);

        // El offset -9999 es el valor de error de symbol_table.hpp.
        if (offset != -9999) {
            write_instruction(std::format("APVL {}, {}", level, offset));
        } 
    }

    // G(assign) - Asignación (el valor a asignar ya está en la pila, M[s])
    void assign()
    {
        // El valor a asignar ya está en M[s].

        // 1. Caso especial: Asignación al valor de retorno de la función (ej: factorial := x;)
       /* if (_sem.is_function_return_assignment(_sem._assign_id))
        {
            // ALVL 1, -1: Asigna M[s] a la celda de retorno (-1) del llamador (diferencia de nivel 1).
            write_instruction(std::format("ALVL 1, -1"));
            return;
        }*/

        // 2. Caso normal: Asignación a una variable (local, parámetro o global).
        // MEPA usa ALVL m, n: Asigna M[s] a M[D[m]+n] y s := s - 1.
        int level = _sem.get_level(_sem._assign_id); // Usa la función correcta: get_level
        int offset = _sem.get_offset(_sem._assign_id);
    
        // Verificar si la búsqueda fue exitosa (no devolvió el valor de error -9999)
        if (offset != -9999) { 
            write_instruction(std::format("ALVL {}, {}", level, offset));
        }
    }

    // Operadores Aritméticos y Lógicos Binarios
    void add_op() { write_instruction("SUMA"); }
    void sub_op() { write_instruction("SUST"); }
    void mult_op() { write_instruction("MULT"); }
    void div_op() { write_instruction("DIVI"); }
    void or_op() { write_instruction("DISJ"); }
    void and_op() { write_instruction("CONJ"); }

    // Operadores Unarios
    void unary_sub_op() { write_instruction("UMEN"); } // Menos unario (-)
    void not_op() { write_instruction("NEGA"); } 

    // Operadores Relacionales
    void eq_op() { write_instruction("CMIG"); } // Igual (=)
    void neq_op() { write_instruction("CMDG"); } // Distinto (!=)
    void min_op() { write_instruction("CMME"); } // Menor (<)
    void maj_op() { write_instruction("CMMA"); } // Mayor (>)
    void mineq_op() { write_instruction("CMNI"); } // Menor o Igual (<=)
    void majeq_op() { write_instruction("CMYI"); } // Mayor o Igual (>=)


// ----------------------------------------------------------------------
// 4. SUBRUTINAS Y LLAMADAS
// ----------------------------------------------------------------------

    // G(proc_entry) - Etiqueta de inicio de un procedimiento o función
    void proc_entry()
    {
        // Etiqueta del procedimiento.
        set_tag(std::format("L{}", _sem._current_scope_name));
    }

    void stack_enter()
    {
        // ENTRADA A PROCEDIMIENTO (MEPA no usa RMEM para variables locales)
        // La MEPA usa ENPR para registrar el display y apuntar al nuevo marco de pila.
        // Asumo que el semántico ya reservó espacio para los parámetros en el marco anterior.
        // Por la especificación de MEPA, las variables locales *siempre* van al inicio del marco,
        // después de los campos de control (D[k] y i+1), pero antes de los parámetros.

        // Ajustamos la lógica para usar RMEM/LMEM para las variables locales, si no son parámetros.
        // Si ENPR se utiliza como entrada al procedimiento, la documentación es confusa.
        // Vamos a implementar la versión más simple:
        // 1. Entrada de control (ENPR)
        // 2. Reserva de espacio para locales (RMEM)
        
        // ENPR: Maneja el display D[k] := s+1
        int level = _sem.get_current_level();
        write_instruction(std::format("ENPR {}", level)); 
        
        // RMEM: Reserva espacio para variables locales (si las hay)
        int local_var_count = _sem.get_local_var_count(_sem._current_scope_name);
        if (local_var_count > 0) {
             write_instruction(std::format("RMEM {}", local_var_count));
        }
    }

    // G(proc_exit) - Fin de un procedimiento (liberación de espacio y retorno)
    void proc_exit()
    {
        // LMEM: Libera variables locales ANTES de RTPR.
        int local_var_count = _sem.get_local_var_count(_sem._current_scope_name);
        if (local_var_count > 0) {
             write_instruction(std::format("LMEM {}", local_var_count));
        }

        // RTPR k, n: Retorna de proc. Libera el marco (parámetros y control) y devuelve el display.
        // k es el nivel léxico del llamador. n es la cantidad de parámetros.
        int caller_level = _sem.get_current_level(); 
        if (caller_level < 0) caller_level = 0; // Para el programa principal (Nivel 0)

        int param_count = _sem.get_fparams(_sem._current_scope_name).size(); 
        write_instruction(std::format("RTPR {}, {}", caller_level, param_count)); 
    }

    // G(func_entry) - Igual que proc_entry.
    void func_entry() { return proc_entry(); }

    // G(func_exit) - Igual que proc_exit. La gestión del valor de retorno debe hacerse
    // antes de llamar a func_exit (ej: APVL del valor de retorno, y luego la instrucción de asignación).
    void func_exit() { return proc_exit(); }

    // G(proc_call) - Llamada a un procedimiento
    void proc_call()
    {
        // LLPR p: Salta a la dirección p (la etiqueta del procedimiento).
        std::string proc_name = _sem._called.top();
        write_instruction(std::format("LLPR L{}", proc_name));
    }
    
    // G(func_call) - Llamada a una función
    void func_call()
    {
        // Es igual a la llamada a procedimiento, pero el código del procedimiento
        // debe dejar el valor de retorno en el lugar correspondiente del marco anterior.
        return proc_call();
    }
    
// ----------------------------------------------------------------------
// 5. ENTRADA Y SALIDA (READ/WRITE)
// ----------------------------------------------------------------------

    void read()
    {
        // LEER: Incrementa s, lee un valor de entrada y lo almacena en M[s].
        write_instruction("LEER"); 

        // Luego, el valor leído (ahora en M[s]) debe ser asignado a la variable.
        int level = _sem.get_level(_sem._last_id);
        int offset = _sem.get_offset(_sem._last_id);
        
        if (level != -1 && offset != -1) {
            // ALVL m, n: Asigna M[s] a M[D[m]+n] y s := s - 1 (consumiendo el valor leído).
            write_instruction(std::format("ALVL {}, {}", level, offset));
        } else {
             std::cerr << "Error: No se encontró dirección para LEER en " << _sem._last_id << "\n";
        }
    }

    void write()
    {
        // Asume que el valor a escribir ya fue apilado por una instrucción previa (ej: load_var o load_const).
        // IMPR: Imprime M[s] y decrementa s (s := s - 1).
        write_instruction("IMLN");
    }

    ~mepa_generator()
    {
        _file.close(); 
    }


    private:

    void write_instruction(const std::string& instruction)
    {
        // Escribe la instrucción con tabulación y actualiza la línea.
        _file << "\t\t" << instruction << "\n";
        _current_line++;
    }

    void set_tag(const std::string& tag)
    {
        // Almacena la etiqueta y la línea actual. La etiqueta se escribe sin tabulación.
        _tags[tag] = _current_line;
        _file << tag;
        write_instruction("NADA");
    }

    int _current_line { 1 };
    const semantic_analyzer& _sem;
    // Usamos un nombre de archivo diferente para el manejo del segundo pase
    std::ofstream _file { std::ofstream("output.mep") }; 
    int label_counter = 1; // Contador de etiquetas (inicia en 1 para evitar conflicto con TAG_MAIN)
    std::stack<int> label_stack; // Pila para etiquetas de control de flujo
    std::map<std::string, int> _tags; // Mapa para almacenar {TAG_nombre: número_línea}
};

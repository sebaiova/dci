#pragma once

#include "ll1.hpp"
#include "non_terminal.hpp"
#include <semantic_analyzer.hpp>
#include <mepa_generator.hpp>

// --- MACROS ---
#define T(TOKEN) lexeme::TOKEN 
#define N(TOKEN) non_terminal::TOKEN
#define S(FUNC) &semantic_analyzer::FUNC
#define G(FUNC) &mepa_generator::FUNC
// --- END MACROS ---

using PARAMETRO_FORMAL = 
    rules<
        rh<T(IDENTIFIER), N(LISTA_IDENTIFICADORES), T(COLON), N(DATA_TYPE)>
    >;

using PARAMETRO_FORMAL1 = 
rules<
    rh<T(SEMI_COLON), N(PARAMETRO_FORMAL), N(PARAMETRO_FORMAL1)>,
    rh<>
>;

using PARAMETROS_ACTUALES = 
rules<
    rh<N(EXPRESION), S(check_param), N(LISTA_EXPRESIONES)>,
    rh<>
>;
    
using LISTA_EXPRESIONES = 
rules<
    rh<T(COMMA), N(EXPRESION), S(check_param), N(LISTA_EXPRESIONES)>,
    rh<>
>;

using LISTA_FACTORES = 
rules<
    rh<T(MULT), S(lint), N(FACTOR), G(mult_op), N(LISTA_FACTORES)>,
    rh<T(AND), S(lbool), N(FACTOR), G(and_op), N(LISTA_FACTORES)>,
    rh<T(DIV), S(lint), N(FACTOR), G(div_op), N(LISTA_FACTORES)>,
    rh<>
>;

using LISTA_IDENTIFICADORES = 
rules<
    rh<T(COMMA), T(IDENTIFIER), N(LISTA_IDENTIFICADORES)>,
    rh<>
>;

using LISTA_PARAMETROS_FORMALES = 
rules<
    rh<N(PARAMETRO_FORMAL), N(PARAMETRO_FORMAL1)>,
    rh<>
>;

using LISTA_SENTENCIAS = 
rules<
    rh<N(SENTENCIA_SIMPLE), T(SEMI_COLON), N(LISTA_SENTENCIAS)>,
    rh<>
>;

using LISTA_TERMINOS = 
    rules<
        rh<T(ADD), S(lint), N(TERMINO), G(add_op), N(LISTA_TERMINOS)>,
        rh<T(SUB), S(lint), N(TERMINO), G(sub_op), N(LISTA_TERMINOS)>,
        rh<T(OR), S(lbool), N(TERMINO), G(or_op), N(LISTA_TERMINOS)>,
        rh<>
    >;

using SENTENCIA = 
    rules<
        rh<N(SENTENCIA_SIMPLE)>,
        rh<N(SENTENCIA_COMPUESTA)>
    >;

using SENTENCIA_CONDICIONAL = 
    rules<
        // G(if_begin) para generar JMPF después de evaluar la EXPRESION
        rh<T(IF), N(EXPRESION), S(lbool), G(if_begin), T(THEN), N(SENTENCIA), N(SENTENCIA_ELSE), G(if_end)>
    >;

using SENTENCIA_ELSE = 
    rules<
        // G(else_begin) para generar JMP y colocar la etiqueta de entrada del ELSE
        rh<T(ELSE), G(else_begin), N(SENTENCIA)>,
        rh<>
    >;

using SENTENCIA_REPETITIVA = 
    rules<
        // G(while_mark) para marcar la etiqueta de inicio del bucle
        // G(while_begin) para generar JMPF de salida
        // G(while_end) para generar JMP de retorno al inicio y colocar la etiqueta de salida
        rh<T(WHILE), G(while_mark), N(EXPRESION), S(lbool), G(while_begin), T(DO), N(SENTENCIA), G(while_end)>
    >;

using SENTENCIA_SIMPLE = 
    rules<
        rh<N(SENTENCIA_CONDICIONAL)>,
        rh<N(SENTENCIA_REPETITIVA)>,
        rh<T(IDENTIFIER), S(facs), N(SENTENCIA_SIMPLE1)>,
        rh<T(WRITE), T(OPEN_PARENTHESIS), N(EXPRESION_SIMPLE), G(write), T(CLOSE_PARENTHESIS)>,
        rh<T(READ), T(OPEN_PARENTHESIS), T(IDENTIFIER), S(is_writable), G(read), T(CLOSE_PARENTHESIS)>
    >;

using SENTENCIA_SIMPLE1 = 
    rules<
        rh<T(OPEN_PARENTHESIS), S(lproc), N(PARAMETROS_ACTUALES), G(proc_call), S(check_call), T(CLOSE_PARENTHESIS)>,
        rh<T(ASSIGNATION), S(assign_open), N(EXPRESION), S(assign_close), G(assign)>
    >;

using SENTENCIA_COMPUESTA = 
    rules<
        rh<T(BEGIN), N(LISTA_SENTENCIAS), T(END)>
    >;

using EXPRESION = 
    rules<
        rh<N(EXPRESION_SIMPLE), N(EXPRESION1)>
    >;

using EXPRESION1 =  
    rules<
        rh<T(REL_EQ), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), G(eq_op), S(expb)>,
        rh<T(REL_NEQ), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), G(neq_op), S(expb)>,
        rh<T(REL_MIN), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), G(min_op), S(expb)>,
        rh<T(REL_MAJ), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), G(maj_op), S(expb)>,
        rh<T(REL_MINEQ), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), G(mineq_op), S(expb)>,
        rh<T(REL_MAJEQ), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), G(majeq_op), S(expb)>,
        rh<>
    >;

using EXPRESION_SIMPLE = 
    rules<
        rh<T(ADD), N(TERMINO), N(LISTA_TERMINOS)>,
        rh<T(SUB), N(TERMINO), N(LISTA_TERMINOS)>,
        rh<N(TERMINO), N(LISTA_TERMINOS)>
    >;

using FACTOR = 
    rules<
        rh<T(NUMBER), S(faci), G(load_const)>, // G(load_const) para PUSH [valor]
        rh<T(TRUE), S(facb), G(load_const)>,   // G(load_const) para PUSH 1
        rh<T(FALSE), S(facb), G(load_const)>,  // G(load_const) para PUSH 0
        rh<T(NOT), N(FACTOR), S(lbool), G(not_op)>,
        rh<T(OPEN_PARENTHESIS), S(openp), N(EXPRESION), T(CLOSE_PARENTHESIS), S(closep), S(exp)>,
        rh<T(IDENTIFIER), S(facs), G(load_var), N(FACTOR1)> 
    >;

using FACTOR1 = 
    rules<
        // Función: G(func_call) para emitir CALL. El resultado queda en la pila.
        rh<T(OPEN_PARENTHESIS), S(lfunc), S(opencall), N(PARAMETROS_ACTUALES), G(func_call), S(check_call), T(CLOSE_PARENTHESIS), S(closecall)>,
        rh<>
    >;

using DATA_TYPE = 
    rules<
        rh<T(INTEGER), S(integer)>,
        rh<T(BOOLEAN), S(boolean)>
    >;

using TERMINO = 
    rules<
        rh<N(FACTOR), N(LISTA_FACTORES)>
    >;


using LISTA_DECLARACION_VARIABLES = 
    rules<
        rh<T(IDENTIFIER), N(LISTA_IDENTIFICADORES), T(COLON), N(DATA_TYPE), T(SEMI_COLON), N(LISTA_DECLARACION_VARIABLES)>,
        rh<>
    >;

using PARAMETROS_FORMALES = 
    rules<
        rh<T(OPEN_PARENTHESIS), S(fparam), N(LISTA_PARAMETROS_FORMALES), T(CLOSE_PARENTHESIS), S(ready)>
    >;

using PROCEDIMIENTO = 
    rules<
        // G(proc_entry) para colocar la etiqueta y generar ENTER
        // G(proc_exit) para generar RETURN
        rh<T(PROCEDURE), S(procedure), T(IDENTIFIER), S(ready), S(push_scope), G(proc_entry), N(PARAMETROS_FORMALES), T(SEMI_COLON), N(BLOQUE), G(proc_exit), S(pop_scope)>
    >;

using FUNCION = 
    rules<
        // G(func_entry) para colocar la etiqueta y generar ENTER
        // G(func_exit) para generar RETURN
        rh<T(FUNCTION), S(function), T(IDENTIFIER), S(ready), S(push_scope), G(func_entry), N(PARAMETROS_FORMALES), T(COLON), S(function), N(DATA_TYPE), T(SEMI_COLON), S(ready), N(BLOQUE), G(func_exit), S(pop_scope)>
    >;

using SUBRUTINA = 
    rules<
        rh<N(FUNCION)>,
        rh<N(PROCEDIMIENTO)>
    >;

using AREA_VARIABLES =
    rules<
        rh<T(VAR), S(var), N(LISTA_DECLARACION_VARIABLES), S(ready)>,
        rh<>
    >;

using AREA_SUBRUTINAS = 
    rules<
        rh<N(SUBRUTINA), T(SEMI_COLON), N(AREA_SUBRUTINAS)>,
        rh<>
    >;

using BLOQUE =      
    rules<
        rh<N(AREA_VARIABLES), N(AREA_SUBRUTINAS), G(stack_enter), N(SENTENCIA_COMPUESTA)>
    >;

using PROGRAMA =    
    rules<
        rh<T(PROGRAM), S(program), T(IDENTIFIER), G(program_begin), S(ready), T(SEMI_COLON), N(AREA_VARIABLES), N(AREA_SUBRUTINAS), G(program_tag), N(SENTENCIA_COMPUESTA), G(program_end), T(DOT)>
    >;

using START = PROGRAMA;
#pragma once

#include "ll1.hpp"
#include "non_terminal.hpp"
#include <semantic_analyzer.hpp>

#define T(TOKEN) lexeme::TOKEN 
#define N(TOKEN) non_terminal::TOKEN
#define S(FUNC) &semantic_analyzer::FUNC

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
        rh<N(OPERADOR_FACTORES), N(FACTOR), N(LISTA_FACTORES)>,
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
        rh<N(OPERADOR_TERMINOS), N(TERMINO), N(LISTA_TERMINOS)>,
        rh<>
    >;

using SENTENCIA = 
    rules<
        rh<N(SENTENCIA_SIMPLE)>,
        rh<N(SENTENCIA_COMPUESTA)>
    >;

using SENTENCIA_CONDICIONAL = 
    rules<
        rh<T(IF), N(EXPRESION), S(lbool), T(THEN), N(SENTENCIA), N(SENTENCIA_ELSE)>
    >;

using SENTENCIA_ELSE = 
    rules<
        rh<T(ELSE), N(SENTENCIA)>,
        rh<>
    >;

using SENTENCIA_REPETITIVA = 
    rules<
        rh<T(WHILE), N(EXPRESION), S(lbool), T(DO), N(SENTENCIA)>
    >;

using SENTENCIA_SIMPLE = 
    rules<
        rh<N(SENTENCIA_CONDICIONAL)>,
        rh<N(SENTENCIA_REPETITIVA)>,
        rh<T(IDENTIFIER), S(facs), N(SENTENCIA_SIMPLE1)>
    >;

using SENTENCIA_SIMPLE1 = 
    rules<
        rh<T(OPEN_PARENTHESIS), S(lproc), N(PARAMETROS_ACTUALES), S(check_call),T(CLOSE_PARENTHESIS)>,
        rh<T(ASSIGNATION), S(assign_open), N(EXPRESION), S(assign_close)>
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
        rh<T(RELATIONAL_OPERATOR), S(lint), N(EXPRESION_SIMPLE), S(lint), S(facb), S(expb)>,
        rh<>
    >;

using EXPRESION_SIMPLE = 
    rules<
        rh<T(ADD), S(nint), N(TERMINO), N(LISTA_TERMINOS)>,
        rh<T(SUB), S(nint), N(TERMINO), N(LISTA_TERMINOS)>,
        rh<N(TERMINO), N(LISTA_TERMINOS)>
    >;

using FACTOR = 
    rules<
        rh<T(NUMBER), S(faci)>,
        rh<T(TRUE), S(facb)>,
        rh<T(FALSE), S(facb)>,
        rh<T(NOT), N(FACTOR), S(lbool)>,
        rh<T(OPEN_PARENTHESIS), S(openp), N(EXPRESION), T(CLOSE_PARENTHESIS), S(closep), S(exp)>,
        rh<T(IDENTIFIER), S(facs), N(FACTOR1)>
    >;

using FACTOR1 = 
    rules<
        rh<T(OPEN_PARENTHESIS), S(lfunc), S(opencall), N(PARAMETROS_ACTUALES), S(check_call), T(CLOSE_PARENTHESIS), S(closecall)>,
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

using OPERADOR_FACTORES = 
    rules<
        rh<T(MULT), S(lint), S(nint)>,
        rh<T(AND), S(lbool), S(nbool)>,
        rh<T(DIV), S(lint), S(nint)>
    >;

using OPERADOR_TERMINOS = 
    rules<
        rh<T(ADD), S(lint), S(nint)>,
        rh<T(SUB), S(lint), S(nint)>,
        rh<T(OR), S(lbool), S(nbool)>
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
        rh<T(PROCEDURE), S(procedure), T(IDENTIFIER), S(ready), S(push_scope), N(PARAMETROS_FORMALES), T(SEMI_COLON), N(BLOQUE), S(pop_scope)>
    >;

using FUNCION = 
    rules<
        rh<T(FUNCTION), S(function), T(IDENTIFIER), S(ready), S(push_scope), N(PARAMETROS_FORMALES), T(COLON), S(function), N(DATA_TYPE), T(SEMI_COLON), S(ready), N(BLOQUE), S(pop_scope)>
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
        rh<N(AREA_VARIABLES), N(AREA_SUBRUTINAS), N(SENTENCIA_COMPUESTA)>
    >;

using PROGRAMA =    
    rules<
        rh<T(PROGRAM), S(program), T(IDENTIFIER), S(ready), T(SEMI_COLON), N(BLOQUE), T(DOT)>
    >;

using START = PROGRAMA;

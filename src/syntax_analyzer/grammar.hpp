#pragma once

#include "ll1.hpp"
#include "non_terminal.hpp"
#include <semantic_rule.hpp>

#define T(TOKEN) lexeme::TOKEN 
#define N(TOKEN) non_terminal::TOKEN
#define S(TOKEN) semantic_rule::TOKEN

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
    rh<N(EXPRESION), N(LISTA_EXPRESIONES)>,
    rh<>
>;
    
using LISTA_EXPRESIONES = 
rules<
    rh<T(COMMA), N(EXPRESION), N(LISTA_EXPRESIONES)>,
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
        rh<T(IF), N(EXPRESION), T(THEN), N(SENTENCIA), N(SENTENCIA_ELSE)>
    >;

using SENTENCIA_ELSE = 
    rules<
        rh<T(ELSE), N(SENTENCIA)>,
        rh<>
    >;

using SENTENCIA_REPETITIVA = 
    rules<
        rh<T(WHILE), N(EXPRESION), T(DO), N(SENTENCIA)>
    >;

using SENTENCIA_SIMPLE = 
    rules<
        rh<N(SENTENCIA_CONDICIONAL)>,
        rh<N(SENTENCIA_REPETITIVA)>,
        rh<T(IDENTIFIER), N(SENTENCIA_SIMPLE1)>
    >;

using SENTENCIA_SIMPLE1 = 
    rules<
        rh<T(OPEN_PARENTHESIS), N(PARAMETROS_ACTUALES), T(CLOSE_PARENTHESIS)>,
        rh<T(ASSIGNATION), N(EXPRESION)>
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
        rh<T(RELATIONAL_OPERATOR), N(EXPRESION_SIMPLE)>,
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
        rh<T(NUMBER)>,
        rh<T(NOT), N(FACTOR)>,
        rh<T(OPEN_PARENTHESIS), N(EXPRESION), T(CLOSE_PARENTHESIS)>,
        rh<T(IDENTIFIER), N(FACTOR1)>
    >;

using FACTOR1 = 
    rules<
        rh<T(OPEN_PARENTHESIS), N(PARAMETROS_ACTUALES), T(CLOSE_PARENTHESIS)>,
        rh<>
    >;

using DATA_TYPE = 
    rules<
        rh<T(INTEGER)>,
        rh<T(BOOLEAN)>
    >;

using TERMINO = 
    rules<
        rh<N(FACTOR), N(LISTA_FACTORES)>
    >;

using OPERADOR_FACTORES = 
    rules<
        rh<T(MULT)>,
        rh<T(AND)>,
        rh<T(DIV)>
    >;

using OPERADOR_TERMINOS = 
    rules<
        rh<T(ADD)>,
        rh<T(SUB)>,
        rh<T(OR)>
    >;

using LISTA_DECLARACION_VARIABLES = 
    rules<
        rh<T(IDENTIFIER), N(LISTA_IDENTIFICADORES), T(COLON), N(DATA_TYPE), T(SEMI_COLON), N(LISTA_DECLARACION_VARIABLES)>,
        rh<>
    >;

using PARAMETROS_FORMALES = 
    rules<
        rh<T(OPEN_PARENTHESIS), N(LISTA_PARAMETROS_FORMALES), T(CLOSE_PARENTHESIS)>
    >;

using PROCEDIMIENTO = 
    rules<
        rh<T(PROCEDURE), T(IDENTIFIER), N(PARAMETROS_FORMALES), T(SEMI_COLON), N(BLOQUE)>
    >;

using FUNCION = 
    rules<
        rh<T(FUNCTION), T(IDENTIFIER), N(PARAMETROS_FORMALES), T(COLON), N(DATA_TYPE), T(SEMI_COLON), N(BLOQUE)>
    >;

using SUBRUTINA = 
    rules<
        rh<N(FUNCION)>,
        rh<N(PROCEDIMIENTO)>
    >;

using AREA_VARIABLES =
    rules<
        rh<T(VAR), N(LISTA_DECLARACION_VARIABLES)>,
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
        rh<T(PROGRAM), S(hella), T(IDENTIFIER), T(SEMI_COLON), N(BLOQUE), T(DOT)>
    >;

using START = PROGRAMA;
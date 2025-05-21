#pragma once

#include "ll1.hpp"
#include "non_terminal.hpp"
#include "firsts.hpp"

#define T(TOKEN) lexeme::TOKEN 
#define N(TOKEN) non_terminal::TOKEN

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
    rh<N(SENTENCIA_SIMPLE), T(SEMI_COLON) ,N(LISTA_SENTENCIAS)>,
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
        rh<N(SIGNO), N(TERMINO), N(LISTA_TERMINOS)>
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

using SIGNO = 
    rules<
        rh<T(ADD)>,
        rh<T(SUB)>,
        rh<>
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
        rh<T(PROGRAM), T(IDENTIFIER), T(SEMI_COLON), N(BLOQUE), T(DOT)>
    >;

using START = PROGRAMA;


template <class T> struct search_lexemes {};

template <> struct search_lexemes<rh<>>
{
    using lex = rh<lexeme::UNDETERMINATED>;
};

template <beta First, beta...Rest> struct search_lexemes<rh<First, Rest...>>
{
    using lex = rh<First>;
};

template<class T> struct deep_search 
{
    using lex = rh<lexeme::UNDETERMINATED>; 
};

template<typename ...> struct rh_concat;

template<> struct rh_concat<> 
{ 
    using type = rh<>; 
};

template<beta... Bs> struct rh_concat<rh<Bs...>> 
{ 
    using type = rh<Bs...>; 
};

template<beta... Bs1, beta... Bs2, typename... Rest> struct rh_concat<rh<Bs1...>, rh<Bs2...>, Rest...> 
{
    using type = typename rh_concat<rh<Bs1..., Bs2...>, Rest...>::type;
};

template <class...RHs> struct search_lexemes<rules<RHs...>> 
{
    using lex = typename rh_concat<typename search_lexemes<RHs>::lex...>::type;
};

template <> struct search_lexemes<rules<>>
{
    using lex = rh<>;
};

template<beta Bt> struct transform
{
    using type = decltype([](){
        if constexpr (decltype(Bt)::is_terminal())
            return rh<Bt>{};
        else if constexpr (decltype(Bt)::is_non_terminal()) 
            return (typename first<Bt>::get){};
        else 
            return rh<Bt>{};
    }());
};

template <beta...Bts> struct deep_search<rh<Bts...>>
{
    using lex = typename rh_concat<typename transform<Bts>::type...>::type;
};




template<class T> struct get_firsts
{
    using tmp = typename search_lexemes<T>::lex;
    using lex = typename deep_search<tmp>::lex;
};


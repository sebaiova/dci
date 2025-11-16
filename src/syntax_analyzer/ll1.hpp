#pragma once 

#include <lexemes.hpp>
#include <error.hpp>
#include <semantic_analyzer.hpp>
#include <mepa_generator.hpp>
#include "non_terminal.hpp"

struct syntax_analyzer;

template<class T> struct beta
{
    constexpr beta(T value) : value { value } { }
    const T value;

    static constexpr bool is_terminal() { return std::is_same_v<T, lexeme>; }
    static constexpr bool is_non_terminal() { return std::is_same_v<T, non_terminal>; }
    static constexpr bool is_semantic_rule() { return std::is_invocable_v<T, semantic_analyzer&>; }
    static constexpr bool is_mepa_rule() { return std::is_invocable_v<T, mepa_generator&>; }

  //  static constexpr bool is_semantic_rule() { 
  //      return std::is_member_function_pointer_v<T> && std::is_invocable_v<T, semantic_analyzer&>; 
  //  }
    
    // 2. Regla MEPA: Debe ser un puntero a función miembro Y ser invocable
  //  static constexpr bool is_mepa_rule() { 
   //     return std::is_member_function_pointer_v<T> && std::is_invocable_v<T, mepa_generator&>; 
   // }
};

template<typename... Rh> struct rules {};
template<beta... Beta> struct rh {};

template<class Rule> struct rule 
{ 

};




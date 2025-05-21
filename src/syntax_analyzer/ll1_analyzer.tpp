#pragma once 

#include "ll1.hpp"
#include "non_terminal.hpp"
#include "firsts.hpp"

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


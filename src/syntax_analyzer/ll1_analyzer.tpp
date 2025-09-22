/** 
    template<class T> struct get_firsts;

    Busca de forma estática y recursiva el conjunto Primeros
    para la regla T, definida en grammar.hpp
**/

#pragma once 

#include "ll1.hpp"
#include "grammar.hpp"

template<class T> struct get_firsts;

namespace 
{
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

    template <class T> struct head{};
    template <class... RHs> struct head<rules<RHs...>>
    {
        using type = typename rh_concat<typename head<RHs>::type...>::type;
    };

    template <beta First, beta...Rest> struct head<rh<First, Rest...>>
    {
        using type = rh<First>;
    };

    template <> struct head<rh<>>
    {
        using type = rh<lexeme::UNDETERMINATED>;
    };

    template <beta Bt> struct ff 
    {
        static consteval auto get()
        {
            #define X(name) \
                if constexpr (Bt.value == non_terminal::name) return typename head<name>::type{};
            NON_TERMINAL_LIST
            #undef X
        }
    };

    template<beta Bt> struct transform
    {
        static consteval auto get()
        {
            if constexpr( decltype(Bt)::is_terminal() )
                return rh<Bt>{};
            else if constexpr ( decltype(Bt)::is_non_terminal() ) 
                return ff<Bt>::get();
            else 
                return rh<Bt>{};    
        };
    };

    template<beta...Bts> struct transform_all
    {
        using type = typename rh_concat<decltype(transform<Bts>::get())...>::type;
    };

    template<class T> struct deep_search;
    template <beta...Bts> struct deep_search<rh<Bts...>>
    {
        static consteval auto get()
        {
            if constexpr (( decltype(Bts)::is_terminal() && ... ))
                return rh<Bts...>{};
            else 
                return deep_search<typename transform_all<Bts...>::type>::get();
        }
    };

}

template<class T> struct get_firsts
{
    using head = typename head<T>::type;
    using lex = decltype(deep_search<head>::get());
};

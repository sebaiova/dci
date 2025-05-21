#include <iostream>
#include <array>
#include "keywords.hpp"
#include "state.hpp"

namespace state 
{
    template<class ...Keywords> constexpr auto keyword_state(char c) -> std::expected<state::result, error>;
}

namespace 
{
    template<typename KW> struct strip_first_char;
    template<lexeme L, char Head, char... Tail> struct strip_first_char<keyword<L, Head, Tail...>>
    {
        using type = keyword<L, Tail...>;
    };

    template<template<typename> class Transform, typename... Ts> struct transform_tuple;
    template<template<typename> class Transform, typename... Ts> struct transform_tuple<Transform, std::tuple<Ts...>>
    {
        using type = std::tuple<typename Transform<Ts>::type...>;
    };

    template<char Target> struct match_first_char
    {
        template<typename KW> struct apply : std::false_type {};
        template<lexeme L, char Head, char... Tail> struct apply<keyword<L, Head, Tail...>> : std::bool_constant<(Head == Target)> {};
    };

    template<template<typename> class Predicate, typename...> struct filter_types
    {
        using type = std::tuple<>;
    };

    template<template<typename> class Predicate, typename T, typename... Rest> struct filter_types<Predicate, T, Rest...>
    {
        using tail = typename filter_types<Predicate, Rest...>::type;
    public:
        using type = std::conditional_t<Predicate<T>::value, decltype(std::tuple_cat(std::tuple<T>{}, tail{})), tail>;
    };

    template<template<typename> class Predicate, typename Tuple> struct filter_tuple;
    template<template<typename> class Predicate, typename... Ts> struct filter_tuple<Predicate, std::tuple<Ts...>>
    {
        using type = typename filter_types<Predicate, Ts...>::type;
    };

    template<class T> struct sizeof_keywords;
    template<class... Ts> struct sizeof_keywords<std::tuple<Ts...>>
    {
        static constexpr auto value { sizeof...(Ts) };
    };

    template<char C, class T> struct keyword_process;
    template<char C, class... Ts> struct keyword_process<C, std::tuple<Ts...>>
    {
        static constexpr auto call() -> std::expected<state::result, error>
        {
            using filtered = typename filter_tuple<match_first_char<C>::template apply, std::tuple<Ts...>>::type;
            using stripped = typename transform_tuple<strip_first_char, filtered>::type;

            if constexpr ( sizeof_keywords<filtered>::value )
                return state::result {lexeme::UNDETERMINATED, state::keyword_state<stripped> };
            
            return state::result { lexeme::UNDETERMINATED, state::identifier };
        }
    };

    template<class T> struct keyword_process_end;
    template<class Keyword, class ...Type> struct keyword_process_end<std::tuple<Keyword, Type...>>
    {
        static constexpr auto call(char c) -> std::expected<state::result, error>
        {
           if constexpr (Keyword::size)
                return state::identifier(c);
            else 
                return state::result{ Keyword::token, state::start };
        }
    };
}

namespace state
{
    template<class ...Ts> constexpr auto keyword_state(char c) -> std::expected<state::result, error>
    {
        switch(c)
        {
            case 'a': return keyword_process<'a', Ts...>::call();
            case 'b': return keyword_process<'b', Ts...>::call();
            case 'c': return keyword_process<'c', Ts...>::call();
            case 'd': return keyword_process<'d', Ts...>::call();
            case 'e': return keyword_process<'e', Ts...>::call();
            case 'f': return keyword_process<'f', Ts...>::call();
            case 'g': return keyword_process<'g', Ts...>::call();
            case 'h': return keyword_process<'h', Ts...>::call();
            case 'i': return keyword_process<'i', Ts...>::call();
            case 'j': return keyword_process<'j', Ts...>::call();
            case 'k': return keyword_process<'k', Ts...>::call();
            case 'l': return keyword_process<'l', Ts...>::call();
            case 'm': return keyword_process<'m', Ts...>::call();
            case 'n': return keyword_process<'n', Ts...>::call();
            case 'o': return keyword_process<'o', Ts...>::call();
            case 'p': return keyword_process<'p', Ts...>::call();
            case 'q': return keyword_process<'q', Ts...>::call();
            case 'r': return keyword_process<'r', Ts...>::call();
            case 's': return keyword_process<'s', Ts...>::call();
            case 't': return keyword_process<'t', Ts...>::call();
            case 'u': return keyword_process<'u', Ts...>::call();
            case 'v': return keyword_process<'v', Ts...>::call();
            case 'w': return keyword_process<'w', Ts...>::call();
            case 'x': return keyword_process<'x', Ts...>::call();
            case 'y': return keyword_process<'y', Ts...>::call();
            case 'z': return keyword_process<'z', Ts...>::call();
            case '0'...'9': return state::result { lexeme::UNDETERMINATED, state::identifier };
            default:
                return keyword_process_end<Ts...>::call(c);
        }
    }
}
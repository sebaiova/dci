#pragma once 

#include <expected>
#include <string>
#include <list>
#include <set>
#include <error.hpp>
#include <lexemes.hpp>

struct parser 
{
    parser(const std::string& str);;
    ~parser();

    auto run() -> std::expected<void, error>;

    private:

    auto lexical_analysis() -> std::expected<void, error>;
    auto syntax_analysis() -> std::expected<void, error>;

    const std::string& str;
    std::list<lexeme> token_stream {};
    std::set<std::string> attribte_table {};
};
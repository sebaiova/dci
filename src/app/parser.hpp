#pragma once 

#include <expected>
#include <string>
#include <list>
#include <set>
#include <error.hpp>
#include <lexemes.hpp>
#include <lexical_analyzer.hpp>

struct parser 
{
    parser(const std::string& str);;
    ~parser();

    auto run() -> std::expected<void, error>;

    private:

    const std::string& str;
    std::list<symbol> token_stream {};
    std::set<std::string> attribte_table {};
};
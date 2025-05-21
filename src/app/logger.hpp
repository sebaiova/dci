#pragma once 

#include <list>
#include <set>
#include <string>
#include <lexemes.hpp>
#include <lexical_analyzer.hpp>

void log_token_stream(const std::list<symbol>& tokens);
void log_attribute_table(const std::set<std::string>& attributes);
#pragma once 

#include <list>
#include <set>
#include <string>
#include <lexemes.hpp>

void log_token_stream(const std::list<lexeme>& tokens);
void log_attribute_table(const std::set<std::string>& attributes);
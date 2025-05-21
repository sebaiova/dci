#include "logger.hpp"

#include <fstream>
#include "strings.hpp"
#include "lexical_analyzer.hpp"

struct logger
{
    logger(const std::string& file_name) : file { std::ofstream(file_name) } {}
    
    ~logger()
    {
        file.close();
    }

    template<class T> logger& operator<<(T&& str)
    {
        file << str;
        return *this;
    }

    private:

    std::ofstream file;
};

void log_token_stream(const std::list<symbol>& symbols)
{
    logger log { "token_stream.log" };
    log << "Lexical Analyzer: Token Stream Output\n....\n";
    for(auto& symbol : symbols)
        log << to_string(symbol.token) << " \n" ;

    log << "......\n"; 
}

void log_attribute_table(const std::set<std::string>& attributes)
{
    logger log { "attribute_table.log" };
    log << "Lexical Analyzer: Attribute Table\n....\n";
    for(auto& att : attributes)
        log << att << " \n" ;
}
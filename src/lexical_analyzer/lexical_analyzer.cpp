#include "lexical_analyzer.hpp"
#include "state.hpp"

namespace lexical 
{
    [[nodiscard]] auto analyzer(std::stringstream& sstream) -> std::expected<result, error>
    {
        std::set<lexeme> lexeme_table;
        std::list<lexeme> lexeme_stream;
        std::size_t line { 0 };
        std::size_t col { 0 };
        state::state_result current_state;

        char c;
        while(sstream.get(c))
        {
            if(c=='\n')
            {
                line++;
                col=0;
            }
                
            c = std::tolower(c);

            current_state = current_state.next_transition(c, current_state.lex.attribute);

            if(current_state.lex.token == lexeme::ERROR)
                return std::unexpected{error(c, line, col)};

            if(current_state.lex.token != lexeme::UNDETERMINATED)
            {
                sstream.unget();
                lexeme_stream.push_back(*lexeme_table.insert(current_state.lex).first);
            }

            col++;
        }

        return result(lexeme_table, lexeme_stream);
    }
}


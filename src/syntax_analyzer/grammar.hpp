#pragma once

using BLOQUE =      
    rules<
        rh<lexeme::IDENTIFIER>
    >;

using PROGRAMA =    
    rules<
        rh<lexeme::PROGRAM, lexeme::IDENTIFIER, lexeme::SEMI_COLON, rule<BLOQUE>::run, lexeme::DOT, beta_type::RECURSIVE>
    >;

using START = PROGRAMA;

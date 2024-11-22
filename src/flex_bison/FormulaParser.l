%{
#include <iostream>
#include "Scanner.h"

#undef YY_DECL
#define YY_DECL int formula_solver::Scanner::lex(formula_solver::Parser::semantic_type *yylval)
%}

%option c++ noyywrap noyylineno nodefault

%%

[a-z]                   { yylval->emplace<char>(YYText()[0]); return Parser::token::VARIABLE; }
[0-9]+(\.[0-9]+)?       { yylval->emplace<double>(strtod(YYText(), nullptr)); return Parser::token::NUMBER; }
&                       { return Parser::token::AND; }
\<=>                    { return Parser::token::EQUIVALENCE; }
\=>                     { return Parser::token::IMPLICATION; }
\|                      { return Parser::token::OR; }
!                       { return Parser::token::NOT; }
~                       { return Parser::token::NOT; }
\(                      { return Parser::token::LEFT_PARENTHESIS; }
\)                      { return Parser::token::RIGHT_PARENTHESIS; }
\n                      { return Parser::token::END_OF_LINE; }
[ \t]                   { /* skip whitespace */ }
.                       {   std::ostringstream oss;
                            oss << "Error: Unrecognized character '" << yytext << "'\n";
                            throw std::runtime_error(oss.str());
                        }

%%

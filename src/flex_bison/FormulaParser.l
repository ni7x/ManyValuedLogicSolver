%{
#include <iostream>
#include "Scanner.h"
#include <set>
#include "LogicalOperator.h"
#undef YY_DECL
#define YY_DECL int formula_solver::Scanner::lex(formula_solver::Parser::semantic_type *yylval)
std::set<LogicalOperator> used_operators;
    int number_of_formulas;
%}

%option c++ noyywrap noyylineno nodefault

%%

[a-z]                   { yylval->emplace<char>(YYText()[0]); return Parser::token::VARIABLE; }
[0-9]+(\.[0-9]+)?       { yylval->emplace<double>(strtod(YYText(), nullptr)); return Parser::token::NUMBER; }
&                       { used_operators.insert(LogicalOperator::AND); return Parser::token::AND; }
\<=>                    { used_operators.insert(LogicalOperator::EQUIVALENCE); return Parser::token::EQUIVALENCE; }
\=>                     { used_operators.insert(LogicalOperator::IMPLICATION); return Parser::token::IMPLICATION; }
\|                      { used_operators.insert(LogicalOperator::OR); return Parser::token::OR; }
!                       { used_operators.insert(LogicalOperator::NOT); return Parser::token::NOT; }
~                       { return Parser::token::NOT; }
\(                      { return Parser::token::LEFT_PARENTHESIS; }
\)                      { return Parser::token::RIGHT_PARENTHESIS; }
\n                      { return Parser::token::END_OF_LINE; }
\,                      {  number_of_formulas++;return Parser::token::FORMULA_SEPARATOR; }
[ \t]                   { /* skip whitespace */ }
.                       {   std::ostringstream oss;
                            oss << "Error: Unrecognized character '" << yytext << "'\n";
                            throw std::runtime_error(oss.str());
                        }

%%

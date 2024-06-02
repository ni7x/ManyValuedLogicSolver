%{
    
#include <iostream>
#include "LogicParser.tab.h"
#include "Scanner.h"
#define YY_DECL int formula_solver::Scanner::lex(formula_solver::Parser::semantic_type *yylval)

%}

%option c++ interactive noyywrap noyylineno nodefault

%%

[a-z]                   { yylval->emplace<char>(YYText()[0]); return Parser::token::VARIABLE; }
[0-9]+(\.[0-9]+)?       { yylval->emplace<double>(strtod(YYText(), nullptr)); return Parser::token::NUMBER;}
&                       { return Parser::token::AND; }
\<=>                     { return Parser::token::EQUIVALENCE; }
\=>                      { return Parser::token::IMPLICATION; }
\|                      { return Parser::token::OR; }
!                       { return Parser::token::NOT; }
~                       { return Parser::token::NOT; }
\(                      { return Parser::token::LEFT_PARENTHESIS; }
\)                      { return Parser::token::RIGHT_PARENTHESIS; }
\n                      { return Parser::token::END_OF_LINE; }
[ \t]                   { /* skip whitespace */ }
.                       { fprintf(stderr, "Error: Unrecognized character '%s'\n", yytext); }

%%

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

int main() {
     std::string input_line;
    std::getline(std::cin, input_line);

    std::istringstream input_stream(input_line);

    formula_solver::Scanner scanner{ input_stream, std::cerr };
    formula_solver::Parser parser{ &scanner };

    std::cout.precision(10);

    parser.parse();

    return 0;
}
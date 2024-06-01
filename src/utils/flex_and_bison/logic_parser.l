%option noyywrap

%{
#include <stdio.h>
#include "logic_parser.tab.h"
%}

%%

[a-z]                   { yylval.character = *yytext; return VARIABLE; }
[0-9]+(\.[0-9]+)?       { yylval.number = atof(yytext); return NUMBER; }
&                       { return AND; }
\<=>                     { return EQUIVALENCE; }
\=>                      { return IMPLICATION; }
\|                      { return OR; }
!                       { return NOT; }
~                       { return NOT; }
\(                      { return LEFT_PARENTHESIS; }
\)                      { return RIGHT_PARENTHESIS; }
\n                      { return END_OF_LINE; }
[ \t]                   { /* skip whitespace */ }
.                       { fprintf(stderr, "Error: Unrecognized character '%s'\n", yytext); }

%%
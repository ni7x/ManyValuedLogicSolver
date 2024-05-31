%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char* s);
int yylex();

%}

%union {
    float number;
    char character;
}

%token<number> NUMBER
%token<character> VARIABLE
%token AND EQUIVALENCE OR IMPLICATION 
%token NOT
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token END_OF_LINE

%nonassoc LEFT_PARENTHESIS RIGHT_PARENTHESIS
%right NOT
%left AND
%left OR
%left IMPLICATION
%left EQUIVALENCE

%start formula



%%

formula: VARIABLE
        | NUMBER
        | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS
        | NOT formula
        | formula AND formula
        | formula OR formula
        | formula IMPLICATION formula
        | formula EQUIVALENCE formula
;

%%

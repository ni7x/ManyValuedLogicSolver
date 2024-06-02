%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ParserParams.h"

void yyerror (ParserParams *parse_params, const char *msg);
int yylex();

%}

%union {
    float number;
    char character;
}

%parse-param { ParserParams *params }


%token<number> NUMBER
%token<character> VARIABLE
%token AND EQUIVALENCE OR IMPLICATION 
%token NOT
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token END_OF_LINE
 

/* ORDER = NOT AND OR EQUICALENCE IMPLICATION */

%right IMPLICATION
%right EQUIVALENCE
%left OR
%left AND
%right NOT

%nonassoc LEFT_PARENTHESIS RIGHT_PARENTHESIS

%type<number> formula
%type<number> formula_line

%start formula_line

%%
formula_line: formula {$$ = $1; *params->result = $1;}

formula: VARIABLE { $$ = (*params->map_function)($1); }
       | NUMBER { $<number>$ = $1; }
       | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS { $$ = $2; }
       | NOT formula { $$ = 1.0 - $2; }
       | formula AND formula { $$ = fmin($1, $3); printf("val(%.2f & %.2f) = %.2f \n", $1, $3, $$); }
       | formula OR formula { $$ = fmax($1, $3); printf("val(%.2f | %.2f) = %.2f \n", $1, $3, $$); }
       | formula IMPLICATION formula { $$ = fmax(1.0 - $1, $3); printf("val(%.2f => %.2f) = %.2f) \n", $1, $3, $$); }
       | formula EQUIVALENCE formula { $$ = fmin(fmax(1.0 - $1, $3), fmax(1.0 - $3, $1));  printf("val(%.2f <=> %.2f) = %.2f \n", $1, $3, $$); }
       ;

%%
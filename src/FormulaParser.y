%{
#include <iostream>
#include <string>
#include <cmath>
#include "FlexLexer.h"
#include <unordered_map>

%}

%code requires {
    namespace formula_solver {
        class Scanner;
        class FormulaParserParams;
    }
}

%code top {
    #include "Scanner.h"
    #include "FormulaParserParams.h"

#define yylex(x) scanner->lex(x)
}

%require "3.7.4"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {formula_solver}
%define api.value.type variant
%parse-param {Scanner* scanner}
%parse-param {FormulaParserParams* params}


%token<int> NUMBER
%token<char> VARIABLE
%token AND EQUIVALENCE OR IMPLICATION 
%token NOT
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token END_OF_LINE

/* ORDER = NOT AND OR EQUICALENCE IMPLICATION */

%right IMPLICATION
%right EQUIVALENCE
%right OR
%right AND
%right NOT

%nonassoc LEFT_PARENTHESIS RIGHT_PARENTHESIS

%type<int> formula
%type<int> logical_expression

%start logical_expression

%%
logical_expression: formula {$$ = $1; params->evaluation_result = $1;}

formula:
      VARIABLE { $$ = params->evaluations[$1]; }
    | NUMBER { $$ = $1; }
    | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS { $$ = $2; }
    | NOT formula { $$ = 0; }
    | formula AND formula {  $$ =  params->logical_operators[0][$1][$3]; }
    | formula OR formula { $$ = params->logical_operators[1][$1][$3]; }
    | formula IMPLICATION formula { $$ = params->logical_operators[2][$1][$3]; }
    | formula EQUIVALENCE formula { $$ = params->logical_operators[3][$1][$3]; }
    ;

%%
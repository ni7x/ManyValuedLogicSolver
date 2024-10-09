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
        class FormulaSolver;
    }
}

%code top {
    #include "Scanner.h"
    #include "FormulaSolver.h"
    #define yylex(x) scanner->lex(x)
}

%require "3.7.4"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {formula_solver}
%define api.value.type variant
%parse-param {Scanner* scanner}
%parse-param {FormulaSolver* solver}


%token<double> NUMBER
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

%type<double> formula
%type<double> logical_expression

%start logical_expression

%%
logical_expression: formula {$$ = $1; solver->set_formula_evaluation_result($1);}

formula:
      VARIABLE { $$ = solver->get_variable_value($1); }
    | NUMBER { $$ = $1; }
    | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS { $$ = $2; }
    | NOT formula { $$ = (solver->number_of_logical_values - 1) - $2; }
    | formula AND formula {  $$ =  solver->logical_operators[0][$1][$3]; }
    | formula OR formula { $$ = solver->logical_operators[1][$1][$3]; }
    | formula IMPLICATION formula { $$ = solver->logical_operators[2][$1][$3]; }
    | formula EQUIVALENCE formula { $$ = solver->logical_operators[3][$1][$3]; }
    ;

%%
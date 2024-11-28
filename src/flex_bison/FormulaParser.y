%{
#include <iostream>
#include <string>
#include <cmath>
#include "flex_bison/FlexLexer.h"
#include <unordered_map>

%}

%code requires {
    namespace formula_solver {
        class Scanner;
        class FormulaEvaluator;
    }
}

%code top {
    #include "Scanner.h"
    #include "include/FormulaEvaluator.h"

#define yylex(x) scanner->lex(x)
}

%require "3.7.4"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {formula_solver}
%define api.value.type variant
%parse-param {Scanner* scanner}
%parse-param {FormulaEvaluator* parser}


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
logical_expression: formula {$$ = $1; parser->set_formula_evaluation_result($1);}

formula:
      VARIABLE { $$ = parser->get_variable_value($1); }
    | NUMBER { $$ = $1; }
    | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS { $$ = $2; }
    | NOT formula { $$ = parser->unary_logical_operators[LogicalOperator::NOT][$2]; }
    | formula AND formula {  $$ =  parser->binary_logical_operators[LogicalOperator::AND][$1][$3]; }
    | formula OR formula { $$ = parser->binary_logical_operators[LogicalOperator::OR][$1][$3]; }
    | formula IMPLICATION formula { $$ = parser->binary_logical_operators[LogicalOperator::IMPLICATION][$1][$3]; }
    | formula EQUIVALENCE formula { $$ = parser->binary_logical_operators[LogicalOperator::EQUIVALENCE][$1][$3]; }
    ;

%%
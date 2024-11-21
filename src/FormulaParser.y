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
        class ParserContext;
    }
}

%code top {
    #include "Scanner.h"
    #include "ParserContext.h"

#define yylex(x) scanner->lex(x)
}

%require "3.7.4"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {formula_solver}
%define api.value.type variant
%parse-param {Scanner* scanner}
%parse-param {ParserContext* parser_context}


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
logical_expression: formula {$$ = $1; parser_context->evaluation_result = $1;}

formula:
      VARIABLE { $$ = parser_context->evaluations[$1]; }
    | NUMBER { $$ = $1; }
    | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS { $$ = $2; }
    | NOT formula { $$ = 0; }
    | formula AND formula {  $$ =  parser_context->logical_operators[0][$1][$3]; }
    | formula OR formula { $$ = parser_context->logical_operators[1][$1][$3]; }
    | formula IMPLICATION formula { $$ = parser_context->logical_operators[2][$1][$3]; }
    | formula EQUIVALENCE formula { $$ = parser_context->logical_operators[3][$1][$3]; }
    ;

%%
%{
#include <iostream>
#include <string>
#include <cmath>
#include "FlexLexer.h"
#include <unordered_map>
%}

%require "3.7.4"
%language "C++"

%define api.parser.class {Parser}
%define api.namespace {formula_solver}
%define api.value.type variant
%parse-param {Scanner* scanner}

%code requires
{
    namespace formula_solver {
        class Scanner;
    } 
} 

%code
{
    #include "Scanner.h"
    #define yylex(x) scanner->lex(x)
}


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
%type<double> formula_line

%start formula_line

%code
{
    namespace formula_solver {
    std::unordered_map<char, double> variable_values = {
        {'a', 0.1},
        {'b', 0.5},
    };

    double get_variable_value(char variable) {
        if (variable_values.find(variable) != variable_values.end()) {
            return variable_values[variable];
        } else {
            std::cerr << "Error: Variable " << variable << " not found." << std::endl;
            return -1.0; 
        }
    }
}
}

%%
formula_line: formula {$$ = $1;/* *params->result = $1;*/}

formula: VARIABLE { $$ = get_variable_value($1); }
       | NUMBER { $$ = $1; }
       | LEFT_PARENTHESIS formula RIGHT_PARENTHESIS { $$ = $2; }
       | NOT formula { $$ = 1.0 - $2; }
       | formula AND formula { 
                $$ = fmin($1, $3); 
                std::cout << "val(" << $1 << " & " << $3 << ") = " << $$ << std::endl; 
        }
        | formula OR formula { 
            $$ = fmax($1, $3); 
            std::cout << "val(" << $1 << " | " << $3 << ") = " << $$ << std::endl; 
        }
        | formula IMPLICATION formula { 
            $$ = fmax(1.0 - $1, $3); 
            std::cout << "val(" << $1 << " => " << $3 << ") = " << $$ << ")" << std::endl; 
        }
        | formula EQUIVALENCE formula { 
            $$ = fmin(fmax(1.0 - $1, $3), fmax(1.0 - $3, $1));  
            std::cout << "val(" << $1 << " <=> " << $3 << ") = " << $$ << std::endl; 
        }
       ;

%%

void formula_solver::Parser::error(const std::string& msg) {
    std::cerr << msg << '\n';
}
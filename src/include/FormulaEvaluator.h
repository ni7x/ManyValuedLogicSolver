#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <istream>
#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "flex_bison/Scanner.h"
#include <list>
#include "BinaryTruthTable.h"
#include "UnaryTruthTable.h"
#include <algorithm>
#include <iostream>
#include <fstream>

namespace formula_solver {

    class FormulaEvaluator {
    public:

        FormulaEvaluator(std::istream &input_stream, std::ostream &error_stream, int n, int k);

        std::list<int> evaluate_formula(const std::vector<int>& new_variable_evaluations);
        std::list<int> evaluate_formula(const std::vector<int>& new_variable_evaluations, const std::unordered_map<LogicalOperator, BinaryTruthTable> binary_logical_operators, const std::unordered_map<LogicalOperator, UnaryTruthTable> unary_logical_operators);
        void add_formula_evaluation_result(int result) {
            formulas_evaluations.push_back(result);
        }
        int get_variable_value(char variable);
        void set_variables(const std::vector<int>& new_variable_evaluations);

        std::unordered_map<LogicalOperator, BinaryTruthTable> binary_logical_operators;
        std::unordered_map<LogicalOperator, UnaryTruthTable> unary_logical_operators;
        std::vector<int> variable_evaluations;
        std::list<char> variable_names;
        std::unordered_map<char, int> variable_index_map;
        std::set<LogicalOperator> used_operators;
        std::istream& input_stream;
        std::ostream& error_stream;
        int number_of_true_logical_values;
        int number_of_logical_values;

    private:
        FormulaEvaluator(std::istream& input_stream, std::ostream& error_stream);



        Scanner scanner;
        Parser parser;
        bool is_evaluation_mode = false;

        std::list<int> formulas_evaluations;

        bool is_formula_valid();
        void add_variable_name(char variable_name);
        void parse_and_reset();
        void reset_input();
        std::string input_string_content;
        friend class FormulaSolver;
    };
}

#endif
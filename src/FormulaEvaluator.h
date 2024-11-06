#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <istream>
#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "Scanner.h"
#include <list>
#include "BinaryTruthTable.h"
#include <algorithm>
#include <iostream>
#include <fstream>

namespace formula_solver {

    class FormulaEvaluator {
    public:

        FormulaEvaluator(std::istream &input_stream, std::ostream &error_stream, int n, int k);

        int evaluate_formula(const std::vector<int>& new_variable_evaluations, std::vector<BinaryTruthTable> logical_operators);

        int get_variable_value(char variable);
        void set_variables(const std::vector<int>& new_variable_evaluations);

        void set_formula_evaluation_result(int result);
        std::vector<BinaryTruthTable> logical_operators;
        std::vector<int> not_operator; //for now
    private:
        FormulaEvaluator(std::istream& input_stream, std::ostream& error_stream);

        int number_of_true_logical_values;
        int number_of_logical_values;
        std::istream& input_stream;
        std::ostream& error_stream;
        Scanner scanner;
        Parser parser;
        bool is_evaluation_mode = false;
        int formula_evaluation_result;
        std::vector<int> variable_evaluations;
        std::list<char> variable_names;

        bool is_formula_valid();
        void add_variable_name(char variable_name);
        void parse_and_reset();
        void reset_input();

        friend class FormulaSolver;
    };
}

#endif
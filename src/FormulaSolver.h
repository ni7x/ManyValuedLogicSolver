#ifndef FORMULA_SOLVER_H
#define FORMULA_SOLVER_H

#include <istream>
#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "Scanner.h"
#include <list>
#include "TruthTable.h"

namespace formula_solver {

    class FormulaSolver {
    public:
        FormulaSolver(std::istream& input_stream, std::ostream& error_stream, int n, int k);

        int evaluate_formula(const std::map<char, int>& new_variable_evaluations, std::vector<TruthTable> operator_functions);

        int get_variable_value(char variable);
        void set_variables(const std::map<char, int>& new_variable_evaluations);

        void set_formula_evaluation_result(int result);
        bool is_formula_valid();
        void add_variable_name(char variable_name);
        int number_of_logical_values;
        std::vector<TruthTable> logical_operators;

        void find_all_tautological_logical_operators();
        std::vector<std::map<char, int>> generate_all_variables_evaluations();
    private:
        FormulaSolver(std::istream& input_stream, std::ostream& error_stream);
        std::istream& input_stream;
        std::ostream& error_stream;
        Scanner scanner;
        Parser parser;

        bool is_evaluation_mode = false;
        int formula_evaluation_result;
        std::unordered_map<char, int> variables_evaluation;

        int number_of_true_logical_values;
        void parse_and_reset();
        void reset_input();
        TruthTable generate_truth_table(int index) const;

        std::vector<char> get_variable_names();
    };
}

#endif

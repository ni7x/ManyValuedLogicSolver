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

        double evaluate_formula(const std::map<char, double>& new_variable_evaluations);

        double get_variable_value(char variable);
        void set_variables(const std::map<char, double>& new_variable_evaluations);

        void set_formula_evaluation_result(double result);
        bool is_formula_valid();
        void add_variable_name(char variable_name);
        int number_of_logical_values;

        std::set<std::map<char, double>> generate_all_true_evaluations(int number_of_logical_values, int number_of_true_logical_values);

    private:
        FormulaSolver(std::istream& input_stream, std::ostream& error_stream);
        std::istream& input_stream;
        std::ostream& error_stream;
        Scanner scanner;
        Parser parser;

        bool is_evaluation_mode = false;
        double formula_evaluation_result;
        std::unordered_map<char, double> variable_evaluations;

        double add_func(double a, double b);
        int number_of_true_logical_values;

        void parse_and_reset();
        void reset_input();
        void generate_combinations(int number_of_logical_values, std::vector<double>& current_values, std::set<std::map<char, double>>& result);
        std::set<std::map<char, double>> generate_variable_evaluations(int number_of_logical_values);
    };
}

#endif

#ifndef FORMULA_SOLVER_H
#define FORMULA_SOLVER_H

#include <istream>
#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "Scanner.h"

namespace formula_solver {

    class FormulaSolver {
    public:
        FormulaSolver(std::istream& input_stream, std::ostream& error_stream);
        FormulaSolver(std::istream& input_stream, std::ostream& error_stream, 
                      const std::unordered_map<char, double>& new_variable_evaluations);
        FormulaSolver(std::istream& input_stream, std::ostream& error_stream, int n, int k);

        double evaluate_formula();
        // double evaluate_formula(int n, int k);
        double evaluate_formula(const std::map<char, double>& new_variable_evaluations);

        double get_variable_value(char variable);
        void set_variables(const std::map<char, double>& new_variable_evaluations);

        void set_formula_evaluation_result(double result);
        bool is_formula_valid();

        std::set<std::map<char, double>> generate_all_true_evaluations(int num_variables, int true_variables);

    private:
        std::istream& input_stream;
        std::ostream& error_stream;
        Scanner scanner;
        Parser parser;
        double formula_evaluation_result;
        std::unordered_map<char, double> variable_evaluations;
        int number_of_variables;
        int number_of_true_variables;
        void reset_input();
    };
}

#endif

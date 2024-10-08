#include "FormulaSolver.h"
#include <algorithm>
#include <iostream>

namespace formula_solver {

    double FormulaSolver::add_func(double a, double b){

    }

    FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream)
        : input_stream(input_stream), error_stream(error_stream),
          scanner(&input_stream, &error_stream), parser(&scanner, this){
        if(!is_formula_valid()){
            throw std::runtime_error("Formula is invalid. Couldn't create Formula Solver.");
        }else{
            std::cout << "--------FORMULA IS CORRECT-------" << std::endl<< std::endl;;
        }
    }


    FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream, 
                                 int n, int k)
        : FormulaSolver(input_stream, error_stream) {
            number_of_logical_values = n;
            number_of_true_logical_values = k;
    }

    void FormulaSolver::parse_and_reset(){
        parser.parse();
        reset_input();
    }

    void FormulaSolver::reset_input() {
        input_stream.clear();
        input_stream.seekg(0, std::ios::beg);
    }

    bool FormulaSolver::is_formula_valid() {
        try {
            is_evaluation_mode = false;
            parse_and_reset();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid formula syntax. " << e.what() << std::endl;
            return false;
        }
    }

    double FormulaSolver::evaluate_formula(const std::map<char, double>& new_variable_evaluations) {
        is_evaluation_mode = true;
        set_variables(new_variable_evaluations);
        parse_and_reset();
        return formula_evaluation_result;
    }

    double FormulaSolver::get_variable_value(char variable) {
        if (is_evaluation_mode) {
            auto it = variable_evaluations.find(variable);
            if (it != variable_evaluations.end()) {
                return it->second;
            } else {
                throw std::runtime_error("Variable not found in evaluations.");
            }
        } else {
            add_variable_name(variable);
            return 0.0;
        }
    }

    void FormulaSolver::set_formula_evaluation_result(double result) {
        formula_evaluation_result = result;
    }

    void FormulaSolver::set_variables(const std::map<char, double>& new_variable_values) {
        variable_evaluations.clear();
        variable_evaluations.insert(new_variable_values.begin(), new_variable_values.end());
    }

    void FormulaSolver::add_variable_name(char variable_name){
            variable_evaluations.insert({variable_name, 0.0});
    }

    std::vector<double> generate_last_k_values(int n, int k) {
        std::vector<double> last_k_values;
        double step = 1.0 / (n - 1);
        for (int i = n - k; i <= n-1; i++) {
            last_k_values.push_back(i * step);
        }
        return last_k_values;
    }

    void FormulaSolver::generate_combinations(int number_of_logical_values, std::vector<double>& current_values, std::set<std::map<char, double>>& result) {
        if (current_values.size() == variable_evaluations.size()) {
            std::map<char, double> evaluation;
            int i = 0;
            for( auto &variable : variable_evaluations){
                evaluation[variable.first] = current_values[i];
                i++;
            }
       
            result.insert(evaluation);
            return;
        }

        double step = (1.0 / (number_of_logical_values - 1));
        for (double value = 0; value <= 1.0 + step/2; value += step) {
            current_values.push_back(value);
            generate_combinations(number_of_logical_values, current_values, result);
            current_values.pop_back();
        }
    }

    std::set<std::map<char, double>> FormulaSolver::generate_variable_evaluations(int number_of_logical_values) {
        std::set<std::map<char, double>> result;
        std::vector<double> current_values;
        generate_combinations(number_of_logical_values, current_values, result);
        return result;
    }

    std::set<std::map<char, double>> FormulaSolver::generate_all_true_evaluations(int number_of_logical_values, int number_of_true_logical_values) {
        std::vector<double> true_values = generate_last_k_values(number_of_logical_values, number_of_true_logical_values);
        auto evaluations = generate_variable_evaluations(number_of_logical_values);
        std::set<std::map<char, double>> true_evaluations;
        std::cout << evaluations.size() << " total evaluations" << std::endl;
        for (const auto& evaluation : evaluations) {
            auto it = std::find_if(true_values.begin(), true_values.end(), [&](double true_value) {
                double formula_value = evaluate_formula(evaluation);
                return std::abs(true_value - formula_value) < 1e-6;;
            });

            if (it != true_values.end()) {
                true_evaluations.insert(evaluation);
            }
        }

        return true_evaluations;
    }


}

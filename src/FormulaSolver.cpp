#include "FormulaSolver.h"
#include <algorithm>
#include <iostream>

namespace formula_solver {

    FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream)
        : input_stream(input_stream), error_stream(error_stream),
          scanner(&input_stream, &error_stream), parser(&scanner, this),
          formula_evaluation_result(0.0) {}

    FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream, 
                                 const std::unordered_map<char, double>& new_variable_evaluations)
        : FormulaSolver(input_stream, error_stream) {
        variable_evaluations = new_variable_evaluations;
    }

    FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream, 
                                 int n, int k)
        : FormulaSolver(input_stream, error_stream) {
            number_of_variables = n;
            number_of_true_variables = k;
            variable_evaluations.clear();
            for (int i = 0; i < 25; i++) {
                char variable_name = 'a' + i; 
                variable_evaluations[variable_name] = 0.0; 
        }
    }

    void FormulaSolver::reset_input() {
        input_stream.clear();
        input_stream.seekg(0, std::ios::beg);
    }

    bool FormulaSolver::is_formula_valid() {
        try {
            parser.parse();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid formula syntax. " << e.what() << std::endl;
            return false;
        }
    }

    double FormulaSolver::evaluate_formula() {
        reset_input();
        parser.parse();
        return formula_evaluation_result;
    }

    double FormulaSolver::evaluate_formula(const std::map<char, double>& new_variable_evaluations) {
        set_variables(new_variable_evaluations);
        return evaluate_formula();
    }

    double FormulaSolver::get_variable_value(char variable) {
        auto it = variable_evaluations.find(variable);
        if (it != variable_evaluations.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Error: Variable " + std::string(1, variable) + " not found.");
        }
    }

    void FormulaSolver::set_formula_evaluation_result(double result) {
        formula_evaluation_result = result;
    }

    void FormulaSolver::set_variables(const std::map<char, double>& new_variable_values) {
        variable_evaluations.clear();
        variable_evaluations.insert(new_variable_values.begin(), new_variable_values.end());
    }

    std::vector<double> generate_last_k_values(int n, int k) {
        std::vector<double> last_k_values;
        double step = 1.0 / (n - 1);
        for (int i = n - k; i <= n-1; ++i) {
            last_k_values.push_back(i * step);
            std::cout << i * step << std::endl;
        }
        return last_k_values;
    }

    void generate_combinations(int num_variables, std::vector<double>& current_values,  //todo lepszy algroytm
                               std::set<std::map<char, double>>& result) {
        if (current_values.size() == num_variables) {
            std::map<char, double> evaluation;
            char variable_name = 'a';
            for (double value : current_values) {
                evaluation[variable_name++] = value;
            }
            result.insert(evaluation);
            return;
        }

        for (double value = 0; value <= 1.0; value += 1.0 / (num_variables - 1)) {
            current_values.push_back(value);
            generate_combinations(num_variables, current_values, result);
            current_values.pop_back();
        }
    }

    std::set<std::map<char, double>> generate_variable_evaluations(int num_variables) {

        std::set<std::map<char, double>> result;
        std::vector<double> current_values;
        generate_combinations(num_variables, current_values, result);
        return result;
    }

    std::set<std::map<char, double>> FormulaSolver::generate_all_true_evaluations(int num_variables, int true_variables) {
        std::vector<double> true_values = generate_last_k_values(num_variables, true_variables);
        auto evaluations = generate_variable_evaluations(num_variables);
        std::set<std::map<char, double>> true_evaluations;

        for (const auto& evaluation : evaluations) {
        
            auto it = std::find(true_values.begin(), true_values.end(), evaluate_formula(evaluation));
            if (it != true_values.end()) {
                true_evaluations.insert(evaluation);
            }
        }

        return true_evaluations;
    }
}

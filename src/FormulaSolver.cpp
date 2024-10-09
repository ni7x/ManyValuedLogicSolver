#include "FormulaSolver.h"
#include <algorithm>
#include <iostream>
#include <fstream>

namespace formula_solver {

     FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream)
        : input_stream(input_stream), error_stream(error_stream),
          scanner(&input_stream, &error_stream), parser(&scanner, this){

    }


    FormulaSolver::FormulaSolver(std::istream& input_stream, std::ostream& error_stream, 
                                 int n, int k)
        : FormulaSolver(input_stream, error_stream) {
            number_of_logical_values = n;
            number_of_true_logical_values = k;
            logical_operators.resize(4, TruthTable(n));
            if(!is_formula_valid()){
                throw std::runtime_error("Formula is invalid. Couldn't create Formula Solver.");
            }else{
                std::cout << "--------FORMULA IS CORRECT-------" << std::endl<< std::endl;;
            }
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

    int FormulaSolver::evaluate_formula(const std::map<char, int>& new_variable_evaluations, const std::vector<TruthTable> logical_operators) {
        is_evaluation_mode = true;
        this->logical_operators = logical_operators;
        set_variables(new_variable_evaluations);
        parse_and_reset();
        return formula_evaluation_result;
    }

    int FormulaSolver::get_variable_value(char variable) {
        if (is_evaluation_mode) {
            auto it = variables_evaluation.find(variable);
            if (it != variables_evaluation.end()) {
                return it->second;
            } else {
                throw std::runtime_error("Variable not found in evaluations.");
            }
        } else {
            add_variable_name(variable);
            return 0.0;
        }
    }

    void FormulaSolver::set_formula_evaluation_result(int result) {
        formula_evaluation_result = result;
    }

    void FormulaSolver::set_variables(const std::map<char, int>& new_variable_values) {
        variables_evaluation.clear();
        variables_evaluation.insert(new_variable_values.begin(), new_variable_values.end());
    }

    void FormulaSolver::add_variable_name(char variable_name){
            variables_evaluation.insert({variable_name, 0});
    }


    std::vector<int> generate_last_k_values(int n, int k) {
        std::vector<int> last_k_values;
        for (int i = n - k; i < n; i++) {
            last_k_values.push_back(i);
        }
        return last_k_values;
    }


    std::vector<char> FormulaSolver::get_variable_names() {
        std::vector<char> keys;
        for (const auto& pair : variables_evaluation) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    std::vector<std::map<char, int>> FormulaSolver::generate_all_variables_evaluations() {
        std::vector<std::map<char, int>> result;
        std::vector<int> current(variables_evaluation.size(), 0);

        auto variable_names = get_variable_names();
        int num_of_variables = variable_names.size();

        while (true) {
            std::map<char, int> evaluation;
            for (int i = 0; i < num_of_variables; ++i) {
                evaluation[variable_names[i]] = current[i];
            }

            result.push_back(evaluation);

            int i = num_of_variables - 1;
            while (i >= 0 && current[i] == number_of_logical_values - 1) {
                current[i] = 0;  // Reset this position and move to the previous digit
                --i;
            }

            if (i < 0) break;  // If we've reached the end, stop

            current[i]++;  // Increment the current digit
        }

        return result;  // Return all evaluations
    }


    TruthTable FormulaSolver::generate_truth_table(int index) const {
        int num_elements = (number_of_logical_values * (number_of_logical_values + 1)) / 2;
        TruthTable table(number_of_logical_values);

        std::vector<double> values(num_elements);

        int combination = index;
        for (int i = 0; i < num_elements; ++i) {
            values[i] = combination % number_of_logical_values;
            combination /= number_of_logical_values;
        }

        table.set_cells(values);

        return table;
    }

    void FormulaSolver::find_all_tautological_logical_operators() {
        auto all_possible_evaluations = generate_all_variables_evaluations();
        int num_of_truth_table_combinations = std::pow(number_of_logical_values, (number_of_logical_values * (number_of_logical_values + 1)) / 2);
        std::vector<int> true_values_in_logic = generate_last_k_values(number_of_logical_values, number_of_true_logical_values);

        std::cout << all_possible_evaluations.size() << " all possible evaluations " << std::endl;
        std::cout << num_of_truth_table_combinations << " total two-argument truth tables for n-valued logic " << std::endl;
        std::cout << std::pow(num_of_truth_table_combinations, 4) << " total 4 operator combinations" << std::endl;
        for (int i = num_of_truth_table_combinations - 1; i >= 0; --i) {
            for (int j = num_of_truth_table_combinations - 1; j >= 0; --j) {
                for (int k = num_of_truth_table_combinations - 1; k >= 0; --k) {
                    for (int l = num_of_truth_table_combinations - 1; l >= 0; --l) {
                        TruthTable and_operator = generate_truth_table(i);
                        TruthTable or_operator = generate_truth_table(j);
                        TruthTable implication_operator = generate_truth_table(k);
                        TruthTable equivalence_operator = generate_truth_table(l);

                        std::vector<TruthTable> current_logical_operators = {and_operator, or_operator, implication_operator, equivalence_operator};

                        bool is_tautology = true;

                        for (const auto& evaluation : all_possible_evaluations) {
                            int formula_result = evaluate_formula(evaluation, current_logical_operators);

                            if (std::find(true_values_in_logic.begin(), true_values_in_logic.end(), formula_result) == true_values_in_logic.end()) {
                                is_tautology = false;
                                break;
                            }
                        }

                        if (is_tautology) {
                            std::cout << "Tautology found with combination: " << i << ", " << j << ", " << k << ", " << l << std::endl;
                            std::cout << "AND" << std::endl;
                            std::cout << and_operator << std::endl;
                            std::cout << "OR" << std::endl;
                            std::cout << or_operator << std::endl;
                            std::cout << "IMPLICATION" << std::endl;
                            std::cout << implication_operator << std::endl;
                            std::cout << "EQUIVALENCE" << std::endl;
                            std::cout << equivalence_operator << std::endl;
                        }
                    }
                }
            }
        }
    }

}

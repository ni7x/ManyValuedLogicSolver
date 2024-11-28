#include "include/FormulaSolver.h"
#include "include/FormulaEvaluator.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <future>
#include <vector>

namespace formula_solver {

    FormulaSolver::FormulaSolver(std::istream &input_stream, std::ostream &error_stream, int n, int k)
            : evaluator(input_stream, error_stream, n, k) {}


    std::vector<int> generate_last_k_values(int n, int k) {
        std::vector<int> last_k_values;
        for (int i = n - k; i < n; i++) {
            last_k_values.push_back(i);
        }
        return last_k_values;
    }

    std::vector<UnaryTruthTable> FormulaSolver::generate_all_unary_truth_tables() const {
        int num_elements = evaluator.number_of_logical_values;
        std::vector<UnaryTruthTable> truth_tables;

        auto total_combinations = static_cast<int>(std::pow(evaluator.number_of_logical_values, num_elements));

        for (int index = 0; index < total_combinations; index++) {
            std::vector<int> cell_values(num_elements, 0);

            int temp_index = index;
            for (int position = 0; position < num_elements; position++) {
                cell_values[position] = temp_index % evaluator.number_of_logical_values;
                temp_index /= evaluator.number_of_logical_values;
            }

            UnaryTruthTable table(evaluator.number_of_logical_values);
            table = cell_values;
            truth_tables.push_back(table);
        }

        return truth_tables;
    }



    std::vector<std::vector<int>> FormulaSolver::generate_all_variables_evaluations() const {
        std::vector<std::vector<int>> evaluations;
        std::vector<int> current_evaluation(evaluator.variable_names.size(), 0);

        auto total_combinations = std::pow(evaluator.number_of_logical_values, evaluator.variable_names.size());

        for (int i = 0; i < total_combinations; i++) {
            evaluations.push_back(current_evaluation);

            for (int position = 0; position < current_evaluation.size(); position++) {
                current_evaluation[position]++;

                if (current_evaluation[position] < evaluator.number_of_logical_values) {
                    break;
                }

                current_evaluation[position] = 0;
            }
        }

        return evaluations;
    }


    std::vector<BinaryTruthTable> FormulaSolver::generate_all_truth_tables() const {
        int num_elements = evaluator.number_of_logical_values * evaluator.number_of_logical_values;
        std::vector<BinaryTruthTable> truth_tables;

        auto total_combinations = static_cast<int>(std::pow(evaluator.number_of_logical_values, num_elements));

        for (int index = 0; index < total_combinations; index++) {
            std::vector<int> cell_values(num_elements, 0);

            int temp_index = index;
            for (int position = 0; position < num_elements; position++) {
                cell_values[position] = temp_index % evaluator.number_of_logical_values;
                temp_index /= evaluator.number_of_logical_values;
            }

            BinaryTruthTable table(evaluator.number_of_logical_values);
            table  = cell_values;
            truth_tables.push_back(table);
        }

        return truth_tables;
    }


    void FormulaSolver::find_all_tautological_logical_operators() {
        auto all_possible_evaluations = generate_all_variables_evaluations();
        auto binary_truth_tables = generate_all_truth_tables();
        auto unary_truth_tables = generate_all_unary_truth_tables();
        auto true_values_in_logic = generate_last_k_values(
                evaluator.number_of_logical_values, evaluator.number_of_true_logical_values);

        int total_binary_combinations = std::pow(evaluator.number_of_logical_values,
                                                 evaluator.number_of_logical_values * evaluator.number_of_logical_values);
        int total_unary_combinations = std::pow(evaluator.number_of_logical_values,
                                                evaluator.number_of_logical_values);


        std::vector<LogicalOperator> operators(evaluator.used_operators.begin(), evaluator.used_operators.end());
        std::vector<int> operator_indices(operators.size(), 0);

        do {
            std::unordered_map<LogicalOperator, BinaryTruthTable> binary_logical_operators;
            std::unordered_map<LogicalOperator, UnaryTruthTable> unary_logical_operators;
            for (size_t i = 0; i < operators.size(); i++) {
                LogicalOperator current_operator = operators[i];
                if (get_operator_type(current_operator) == OperatorType::UNARY) {
                    unary_logical_operators[current_operator] = unary_truth_tables[operator_indices[i] % total_unary_combinations];
                } else {
                    binary_logical_operators[current_operator] = binary_truth_tables[operator_indices[i] % total_binary_combinations];
                }
            }

            if (check_tautology(all_possible_evaluations, true_values_in_logic,
                                binary_logical_operators, unary_logical_operators)) {
                display_tautology(binary_logical_operators, unary_logical_operators);
            }

        } while (update_operator_indices(operator_indices, operators,
                                         total_binary_combinations, total_unary_combinations));
    }



    bool FormulaSolver::check_tautology(
            const std::vector<std::vector<int>>& all_evaluations,
            const std::vector<int>& true_values,
            const std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
            const std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators) {

        for (const auto& evaluation : all_evaluations) {
            int result = evaluator.evaluate_formula(evaluation, binary_logical_operators, unary_logical_operators);
            if (std::find(true_values.begin(), true_values.end(), result) == true_values.end()) {
                return false;
            }
        }
        return true;
    }


    void FormulaSolver::display_tautology(
             std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
             std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators) {

        for (auto& [op, table] : binary_logical_operators) {
            std::cout << op  << " operator" <<  std::endl  << table << std::endl;
        }

        for (auto& [op, table] : unary_logical_operators) {
            std::cout << op  << " operator"  << std::endl << table << std::endl;
        }

        std::cout << std::endl << "==================" << std::endl;
    }

    bool FormulaSolver::update_operator_indices(
            std::vector<int>& indices,
            const std::vector<LogicalOperator>& operators,
            int total_binary_combinations,
            int total_unary_combinations) {

        int position = operators.size() - 1;

        while (position >= 0) {
            int max_combinations = (get_operator_type(operators[position]) == OperatorType::UNARY)
                                   ? total_unary_combinations
                                   : total_binary_combinations;

            if (indices[position] < max_combinations - 1) {
                ++indices[position];
                return true;
            } else {
                indices[position] = 0;
                --position;
            }
        }

        return false;
    }



}

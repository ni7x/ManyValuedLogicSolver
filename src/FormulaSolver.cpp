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
        std::vector<UnaryTruthTable> truth_tables;

        int num_rows = evaluator.number_of_logical_values;

        int total_combinations = std::pow(evaluator.number_of_logical_values, num_rows);

        std::vector<int> current_evaluation(num_rows, 0);

        for (int i = 0; i < total_combinations; i++) {
            UnaryTruthTable table(evaluator.number_of_logical_values);

            for (int row = 0; row < num_rows; ++row) {
                table[row] = current_evaluation[row];
            }

            truth_tables.push_back(table);

            for (int position = 0; position < current_evaluation.size(); position++) {
                current_evaluation[position]++;

                if (current_evaluation[position] < evaluator.number_of_logical_values) {
                    break;
                }

                current_evaluation[position] = 0;
            }
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
        std::vector<BinaryTruthTable> truth_tables;

        int num_rows = evaluator.number_of_logical_values * evaluator.number_of_logical_values;

        int total_combinations = std::pow(evaluator.number_of_logical_values, num_rows);

        std::vector<int> current_evaluation(num_rows, 0);

        for (int i = 0; i < total_combinations; i++) {
            BinaryTruthTable table(evaluator.number_of_logical_values);

            int index = 0;
            for (int row = 0; row < evaluator.number_of_logical_values; ++row) {
                for (int col = 0; col < evaluator.number_of_logical_values; ++col) {
                    table[row][col] = current_evaluation[index++];
                }
            }

            truth_tables.push_back(table);

            for (int position = 0; position < current_evaluation.size(); position++) {
                current_evaluation[position]++;

                if (current_evaluation[position] < evaluator.number_of_logical_values) {
                    break;
                }

                current_evaluation[position] = 0;
            }
        }

        return truth_tables;
    }

    void FormulaSolver::find_all_tautological_logical_operators() {

        auto all_possible_evaluations = generate_all_variables_evaluations();
        auto binary_truth_tables = generate_all_truth_tables();
        auto unary_truth_tables = generate_all_unary_truth_tables();
        auto true_values_in_logic = generate_last_k_values(
                evaluator.number_of_logical_values, evaluator.number_of_true_logical_values);


        int total_binary_combinations = static_cast<int>(std::pow(evaluator.number_of_logical_values,
                                                                  evaluator.number_of_logical_values * evaluator.number_of_logical_values));

        int total_unary_combinations = static_cast<int>(std::pow(evaluator.number_of_logical_values,
                                                                 evaluator.number_of_logical_values));


        std::cout << total_binary_combinations << std::endl;

        std::vector<LogicalOperator> operators(evaluator.used_operators.begin(), evaluator.used_operators.end());
        std::vector<int> operator_indices(operators.size(), 0);
        int n = 0;
        do {
            std::unordered_map<LogicalOperator, BinaryTruthTable> binary_logical_operators;
            std::unordered_map<LogicalOperator, UnaryTruthTable> unary_logical_operators;
            for (int i = 0; i < operators.size(); i++) {
                LogicalOperator current_operator = operators[i];
                for(auto indices: operator_indices){
                    std::cout << indices << " ";
                }
                std::cout << std::endl;
                if (get_operator_type(current_operator) == OperatorType::UNARY) {
                    unary_logical_operators[current_operator] = unary_truth_tables[operator_indices[i] % total_unary_combinations];
                } else {
                    binary_logical_operators[current_operator] = binary_truth_tables[operator_indices[i] % total_binary_combinations];
                }
            }

            if (check_tautology(all_possible_evaluations, true_values_in_logic,
                                binary_logical_operators, unary_logical_operators)) {
                n++;
                display_tautology(binary_logical_operators, unary_logical_operators);
            }

        } while (update_operator_indices(operator_indices, operators,
                                         total_binary_combinations, total_unary_combinations));

        std::cout << "Count " << n;
    }



    bool FormulaSolver::check_tautology(
            const std::vector<std::vector<int>>& all_evaluations,
            const std::vector<int>& true_values,
            const std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
            const std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators) {

        auto isTautology = true;

        for (const auto& evaluation : all_evaluations) {

            std::list<int> result = evaluator.evaluate_formula(evaluation, binary_logical_operators, unary_logical_operators);

            for (const auto& f : result) {

                if (std::find(true_values.begin(), true_values.end(), f) == true_values.end()) {
                    isTautology = false;
                    break;
                }
            }
        }
        return isTautology;
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
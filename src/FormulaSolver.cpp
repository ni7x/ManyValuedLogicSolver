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
        int num_of_truth_table_combinations = std::pow(evaluator.number_of_logical_values,
                                                       (evaluator.number_of_logical_values * evaluator.number_of_logical_values));
        std::vector<int> true_values_in_logic = generate_last_k_values(evaluator.number_of_logical_values, evaluator.number_of_true_logical_values);

        std::cout << num_of_truth_table_combinations << " total two-argument truth tables for n-valued logic" << std::endl;
        std::cout << std::pow(num_of_truth_table_combinations, evaluator.used_operators.size())
                  << " total operator combinations for " << evaluator.used_operators.size() << " operators" << std::endl;

        std::vector<BinaryTruthTable> truth_tables = generate_all_truth_tables();

        int num_used_operators = evaluator.used_operators.size();
        std::vector<int> indices(num_used_operators, 0);

        while (true) {
            std::map<LogicalOperator, BinaryTruthTable> current_logical_operators;

            int i = 0;
            for (auto logical_operator : evaluator.used_operators) {
                current_logical_operators[logical_operator] = truth_tables[indices[i]];
                i++;
            }

            bool is_tautology = true;
            for (const auto& evaluation : all_possible_evaluations) {
                int formula_result = evaluator.evaluate_formula(evaluation, current_logical_operators);

                if (std::find(true_values_in_logic.begin(), true_values_in_logic.end(), formula_result) ==
                    true_values_in_logic.end()) {
                    is_tautology = false;
                    break;
                }
            }

            if (is_tautology) {
                std::cout << "==================" << std::endl;
                for (auto& [op, table] : current_logical_operators) {
                    std::cout << "Operator " << op << std::endl;
                    std::cout << table;
                    std::cout << std::endl;
                }
            }

            int position = num_used_operators - 1;
            while (position >= 0) {
                if (indices[position] < num_of_truth_table_combinations - 1) {
                    ++indices[position];
                    break;
                } else {
                    indices[position] = 0;
                    --position;
                }
            }

            if (position < 0) {
                break;
            }
        }

    }

}

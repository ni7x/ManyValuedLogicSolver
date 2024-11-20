#include "FormulaSolver.h"
#include "FormulaEvaluator.h"
#include "FormulaParserParams.h"
#include "ParserContext.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <future>
#include <vector>

namespace formula_solver {

    FormulaSolver::FormulaSolver(std::istream &input_stream, std::ostream &error_stream, int n, int k)
            : evaluator(input_stream, error_stream, n, k) {}

    int FormulaSolver::parse_with_params(FormulaParserParams params){

        return 0
        ;
    }


    std::vector<int> generate_last_k_values(int n, int k) {
        std::vector<int> last_k_values;
        for (int i = n - k; i < n; i++) {
            last_k_values.push_back(i);
        }
        return last_k_values;
    }


    std::vector<std::unordered_map<char, int>> FormulaSolver::generate_all_variables_evaluations() const {
        std::vector<std::unordered_map<char, int>> evaluations;

        std::unordered_map<char, int> current_evaluation;

        for (const auto& variable : evaluator.variable_evaluations) {
            current_evaluation[variable.first] = 0;
        }

        auto total_combinations = std::pow(evaluator.number_of_logical_values, evaluator.variable_evaluations.size());

        for (int i = 0; i < total_combinations; i++) {
            evaluations.push_back(current_evaluation);

            for (auto it = current_evaluation.begin(); it != current_evaluation.end(); ++it) {
                it->second++;

                if (it->second < evaluator.number_of_logical_values) {
                    break;
                }

                it->second = 0;
            }
        }

        return evaluations;
    }


    std::vector<BinaryTruthTable> FormulaSolver::generate_all_truth_tables() const {
        int num_elements = (evaluator.number_of_logical_values * (evaluator.number_of_logical_values + 1)) / 2;
        std::vector<BinaryTruthTable> truth_tables;

        auto total_combinations = std::pow(evaluator.number_of_logical_values, num_elements);

        for (int index = 0; index < total_combinations; index++) {
            BinaryTruthTable table(evaluator.number_of_logical_values);
            std::vector<double> cell_values(num_elements, 0);

            for (int i = 0; i < index; ++i) {
                for (int position = 0; position < num_elements; position++) {
                    cell_values[position]++;

                    if (cell_values[position] < evaluator.number_of_logical_values) {
                        break;
                    }

                    cell_values[position] = 0;
                }
            }

            table.set_cells(cell_values);
            truth_tables.push_back(table);
        }

        return truth_tables;
    }

    void FormulaSolver::find_all_tautological_logical_operators() {
        auto all_possible_evaluations = generate_all_variables_evaluations();
        int num_of_truth_table_combinations = std::pow(evaluator.number_of_logical_values, (evaluator.number_of_logical_values * (evaluator.number_of_logical_values + 1)) / 2);
        std::vector<int> true_values_in_logic = generate_last_k_values(evaluator.number_of_logical_values, evaluator.number_of_true_logical_values);

        std::cout << all_possible_evaluations.size() << " all possible evaluations " << std::endl;
        std::cout << num_of_truth_table_combinations << " total two-argument truth tables for n-valued logic " << std::endl;
        std::cout << std::pow(num_of_truth_table_combinations, 4) << " total 4 operator combinations" << std::endl;

        std::vector<BinaryTruthTable> truth_tables = generate_all_truth_tables();

        std::mutex output_mutex;
        std::atomic<int> found_tautologies(0);

        auto check_combination = [&](int i, int j, int k, int l) {
            BinaryTruthTable and_operator = truth_tables[i];
            BinaryTruthTable or_operator = truth_tables[j];
            BinaryTruthTable implication_operator = truth_tables[k];
            BinaryTruthTable equivalence_operator = truth_tables[l];

            std::vector<BinaryTruthTable> current_logical_operators = {and_operator,
                                                                       or_operator,
                                                                       implication_operator,
                                                                       equivalence_operator};


            FormulaParserParams params;
            params.logical_operators = current_logical_operators;
            bool is_tautology = true;

            for (const auto& evaluation : all_possible_evaluations) {
                ParserContext context("a|b", params);

                context.update_evaluations(evaluation);
                int formula_result = context.evaluate();

                if (std::find(true_values_in_logic.begin(), true_values_in_logic.end(), formula_result) ==
                    true_values_in_logic.end()) {
                    is_tautology = false;
                    break;
                }
            }

            if (is_tautology) {
                std::lock_guard<std::mutex> lock(output_mutex);
                std::cout << "Tautology found with combination: " << i << ", " << j << ", " << k << ", " << l << std::endl;
                std::cout << "AND" << std::endl;
                std::cout << and_operator << std::endl;
                std::cout << "OR" << std::endl;
                std::cout << or_operator << std::endl;
                std::cout << "IMPLICATION" << std::endl;
                std::cout << implication_operator << std::endl;
                std::cout << "EQUIVALENCE" << std::endl;
                std::cout << equivalence_operator << std::endl;
                found_tautologies++;
            }
        };


        std::vector<std::thread> threads;


        for (int i = num_of_truth_table_combinations - 1; i >= 0; --i) {
            for (int j = num_of_truth_table_combinations - 1; j >= 0; --j) {
                for (int k = num_of_truth_table_combinations - 1; k >= 0; --k) {
                    for (int l = num_of_truth_table_combinations - 1; l >= 0; --l) {

                        threads.emplace_back(check_combination, i, j, k, l);
                    }
                }
            }
        }


        for (auto& t : threads) {
            t.join();
        }


        std::cout << "Total tautologies found: " << found_tautologies.load() << std::endl;
    }

}

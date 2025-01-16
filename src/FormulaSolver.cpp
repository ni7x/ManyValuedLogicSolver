#include "include/FormulaSolver.h"
#include "include/FormulaEvaluator.h"
#include <algorithm>
#include <iostream>
#include <future>
#include <vector>
#include <functional>
#include <unordered_set>

namespace formula_solver {

    FormulaSolver::FormulaSolver(std::shared_ptr<std::istream> input_stream, std::ostream& error_stream, int n, int k)
            : evaluator(std::move(input_stream), error_stream, n, k) {
        initialize_data();
    }

    std::shared_ptr<FormulaEvaluator> create_new_formula_evaluator(const FormulaEvaluator& original_evaluator) {
        if (!original_evaluator.input_stream) {
            throw std::runtime_error("Input stream is invalid in create_new_formula_evaluator.");
        }

        std::ostringstream temp_input_stream;

        temp_input_stream << original_evaluator.input_stream->rdbuf();
        original_evaluator.input_stream->clear();
        original_evaluator.input_stream->seekg(0);

        std::string formula_string = temp_input_stream.str();

        std::shared_ptr<std::istream> input_stream = std::make_shared<std::istringstream>(formula_string);
        return std::make_shared<FormulaEvaluator>(input_stream, original_evaluator.error_stream,
                                                  original_evaluator.number_of_logical_values,
                                                  original_evaluator.number_of_true_logical_values);
    }

    std::vector<int> generate_last_k_values(int n, int k) {
        std::vector<int> last_k_values;
        for (int i = n - k; i < n; i++) {
            last_k_values.push_back(i);
        }
        return last_k_values;
    }

    void FormulaSolver::initialize_data() {
        all_possible_evaluations = generate_all_variables_evaluations();
        binary_truth_tables = generate_all_truth_tables();
        unary_truth_tables = generate_all_unary_truth_tables();
        true_values_in_logic = generate_last_k_values(
                evaluator.number_of_logical_values, evaluator.number_of_true_logical_values);

        total_binary_combinations = static_cast<int>(std::pow(evaluator.number_of_logical_values,
                                                              evaluator.number_of_logical_values * evaluator.number_of_logical_values));

        total_unary_combinations = static_cast<int>(std::pow(evaluator.number_of_logical_values,
                                                             evaluator.number_of_logical_values));

        int unary_operator_count = 0;
        int binary_operator_count = 0;
        for (const auto& op : evaluator.used_operators) {
            if (get_operator_type(op) == OperatorType::UNARY) {
                unary_operator_count++;
            } else {
                binary_operator_count++;
            }
        }
        if (unary_operator_count > 0 && binary_operator_count > 0) {
            total_unary_binary_table_combinations = total_binary_combinations * total_unary_combinations;
        } else if (binary_operator_count > 0) {
            total_unary_binary_table_combinations = total_binary_combinations;
        } else if (unary_operator_count > 0) {
            total_unary_binary_table_combinations = total_unary_combinations;
        }
    }

    std::vector<UnaryTruthTable> FormulaSolver::generate_all_unary_truth_tables() const {
        std::vector<UnaryTruthTable> truth_tables;

        int num_rows = evaluator.number_of_logical_values;

        auto total_combinations = std::pow(evaluator.number_of_logical_values, num_rows);

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

        auto total_binary_combinations_ = std::pow(evaluator.number_of_logical_values, num_rows);

        std::vector<int> current_evaluation(num_rows, 0);

        for (int i = 0; i < total_binary_combinations_; i++) {
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

    void FormulaSolver::find_tautologies_in_range(
            FormulaEvaluator* local_evaluator,
            int indexStart,
            int indexEnd,
            std::mutex& output_mutex) {
        if (!local_evaluator) {
            std::cerr << "Error: local_formula_evaluator is null!" << std::endl;
            return;
        }
        std::vector<LogicalOperator> operators(evaluator.used_operators.begin(), evaluator.used_operators.end());
        std::vector<int> operator_indices(operators.size(), 0);

        try {
            for (int i = indexStart; i < indexEnd; ++i) {
                int temp = i;

                for (int j = 0; j < operators.size(); ++j) {
                    operator_indices[j] = temp % (get_operator_type(operators[j]) == OperatorType::UNARY
                                                  ? total_unary_combinations
                                                  : total_binary_combinations);
                    temp /= (get_operator_type(operators[j]) == OperatorType::UNARY
                             ? total_unary_combinations
                             : total_binary_combinations);
                }

                std::unordered_map<LogicalOperator, BinaryTruthTable> binary_logical_operators;
                std::unordered_map<LogicalOperator, UnaryTruthTable> unary_logical_operators;

                for (int j = 0; j < operators.size(); ++j) {
                    LogicalOperator current_operator = operators[j];
                    if (get_operator_type(current_operator) == OperatorType::UNARY) {
                        unary_logical_operators[current_operator] = unary_truth_tables[operator_indices[j]];
                    } else {
                        binary_logical_operators[current_operator] = binary_truth_tables[operator_indices[j]];
                    }
                }

                if (check_tautology(local_evaluator, all_possible_evaluations, true_values_in_logic,
                                    binary_logical_operators, unary_logical_operators)) {
                    std::lock_guard<std::mutex> lock(output_mutex);
                    display_tautology(binary_logical_operators, unary_logical_operators);
                }
            }
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(output_mutex);
            std::cerr << "Exception in thread: " << e.what() << std::endl;
        }
    }

    void FormulaSolver::find_all_tautological_logical_operators() {
        const unsigned int num_threads = std::thread::hardware_concurrency();
        const int total_combinations = total_unary_binary_table_combinations;
        const int chunk_size = (total_combinations + num_threads - 1) / num_threads;

        std::vector<std::thread> threads;

        std::mutex output_mutex;

        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end = std::min((i + 1) * chunk_size, total_combinations);

            threads.emplace_back([this, start, end, &output_mutex]() {
                try {
                    std::shared_ptr<FormulaEvaluator> local_formula_evaluator = create_new_formula_evaluator(evaluator);
                    find_tautologies_in_range(local_formula_evaluator.get(), start, end, output_mutex);
                } catch (const std::exception& e) {
                    std::lock_guard<std::mutex> lock(output_mutex);
                    std::cerr << "Error in thread: " << e.what() << std::endl;
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    bool FormulaSolver::check_tautology(
            FormulaEvaluator* local_formula_evaluator,
            const std::vector<std::vector<int>>& all_evaluations,
            const std::vector<int>& true_values,
            const std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
            const std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators) {

        std::unordered_set<int> true_values_set(true_values.begin(), true_values.end());

        for (const auto& evaluation : all_evaluations) {
            std::list<int> formulas_results = local_formula_evaluator->evaluate_formula(evaluation, binary_logical_operators, unary_logical_operators);

            for (int formula_result : formulas_results) {
                if (true_values_set.find(formula_result) == true_values_set.end()) {
                    return false;
                }
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
}

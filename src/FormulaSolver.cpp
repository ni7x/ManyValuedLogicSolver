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

    BinaryTruthTable generate_binary_truth_table_from_index(int n, uint64_t index) {
        int size = n * n;
        BinaryTruthTable table(n);
        for (int i = 0; i < size; ++i) {
            int value = index % n;
            table[i / n][i % n] = value;
            index /= n;
        }
        return table;
    }

    UnaryTruthTable generate_unary_truth_table_from_index(int n, uint64_t index) {
        UnaryTruthTable table(n);
        for (int i = 0; i < n; ++i) {
            table[i] = index % n;
            index /= n;
        }
        return table;
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

    uint64_t int_pow(uint64_t base, uint64_t exp) {
        uint64_t result = 1;
        while (exp > 0) {
            if (exp % 2 == 1)
                result *= base;
            base *= base;
            exp /= 2;
        }
        return result;
    }

    void FormulaSolver::initialize_data() {
        all_possible_evaluations = generate_all_variables_evaluations();

        true_values_in_logic = generate_last_k_values(
                evaluator.number_of_logical_values, evaluator.number_of_true_logical_values);
        for (const auto& var : evaluator.variable_names) {
            std::cout << var << " ";
        }
        std::cout << std::endl;
        total_binary_combinations = int_pow(evaluator.number_of_logical_values,
                                            evaluator.number_of_logical_values * evaluator.number_of_logical_values);

        total_unary_combinations = int_pow(evaluator.number_of_logical_values,
                                           evaluator.number_of_logical_values);

        int unary_operator_count = 0;
        int binary_operator_count = 0;

        for (const auto& op : evaluator.used_operators) {
            if (get_operator_type(op) == OperatorType::UNARY)
                unary_operator_count++;
            else
                binary_operator_count++;
        }

        if (unary_operator_count > 0 && binary_operator_count > 0) {
            total_unary_binary_table_combinations = int_pow(total_binary_combinations, binary_operator_count)
                                                    * int_pow(total_unary_combinations, unary_operator_count);
        } else if (binary_operator_count > 0) {
            total_unary_binary_table_combinations = int_pow(total_binary_combinations, binary_operator_count);
        } else if (unary_operator_count > 0) {
            total_unary_binary_table_combinations = int_pow(total_unary_combinations, unary_operator_count);
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

        std::uint64_t total_binary_combinations_ = 1;
        for (int i = 0; i < num_rows; ++i) {
            total_binary_combinations_ *= evaluator.number_of_logical_values;
        }

        std::vector<int> current_evaluation(num_rows, 0);

        for (std::uint64_t i = 0; i < total_binary_combinations_; ++i) {
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

    uint64_t FormulaSolver::find_tautologies_in_range(
            FormulaEvaluator* local_evaluator,
            uint64_t indexStart,
            uint64_t indexEnd,
            std::mutex& output_mutex) {
        if (!local_evaluator) {
            std::cerr << "Error: local_formula_evaluator is null!" << std::endl;
            return 0;
        }
        uint64_t count = 0;
        std::vector<LogicalOperator> operators(evaluator.used_operators.begin(), evaluator.used_operators.end());
        std::vector<int> operator_indices(operators.size(), 0);

        try {
            for (uint64_t i = indexStart; i < indexEnd; ++i) {
                auto temp = i;

                for (uint64_t j = 0; j < operators.size(); ++j) {
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
                        unary_logical_operators[current_operator] =
                                generate_unary_truth_table_from_index(evaluator.number_of_logical_values, operator_indices[j]);
                    } else {
                        binary_logical_operators[current_operator] =
                                generate_binary_truth_table_from_index(evaluator.number_of_logical_values, operator_indices[j]);
                    }
                }

//                std::cout << binary_logical_operators[LogicalOperator::IMPLICATION] << std::endl;
//                std::cout << unary_logical_operators[LogicalOperator::NOT] << std::endl;

                if (check_tautology(local_evaluator, all_possible_evaluations, true_values_in_logic,
                                    binary_logical_operators, unary_logical_operators)) {

                    std::lock_guard<std::mutex> lock(output_mutex);
                    display_tautology(binary_logical_operators, unary_logical_operators);
                    count++;
                }
            }
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(output_mutex);
            std::cerr << "Exception in thread: " << e.what() << std::endl;
        }
        return count;
    }

    void FormulaSolver::find_all_tautological_logical_operators() {
        const unsigned int num_threads = std::thread::hardware_concurrency();
        const uint64_t total_combinations = total_unary_binary_table_combinations;

        const uint64_t chunk_size = (total_combinations + num_threads - 1) / num_threads;

        std::vector<std::thread> threads;

        std::mutex output_mutex;
        uint64_t c = 0;

        for (int i = 0; i < num_threads; ++i) {
            uint64_t start = i * chunk_size;
            uint64_t end = std::min((i + 1) * chunk_size, total_combinations);
            threads.emplace_back([this, start, end, &output_mutex, &c]() {
                try {
                    std::shared_ptr<FormulaEvaluator> local_formula_evaluator = create_new_formula_evaluator(evaluator);
                    uint64_t result = find_tautologies_in_range(local_formula_evaluator.get(), start, end, output_mutex);

                    std::lock_guard<std::mutex> lock(output_mutex);
                    c += result;

                } catch (const std::exception& e) {
                    std::lock_guard<std::mutex> lock(output_mutex);
                    std::cerr << "Error in thread: " << e.what() << std::endl;
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Tautologies found: " << c << std::endl;
    }



    bool FormulaSolver::check_tautology(
            FormulaEvaluator* local_formula_evaluator,
            const std::vector<std::vector<int>>& all_evaluations,
            const std::vector<int>& true_values,
            const std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
            const std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators) {

        std::unordered_set<int> true_values_set(true_values.begin(), true_values.end());
//        std::cout << true_values.size() << std::endl;
        for (const auto& evaluation : all_evaluations) {
            std::list<int> formulas_results = local_formula_evaluator->evaluate_formula(evaluation, binary_logical_operators, unary_logical_operators);
            for (int formula_result : formulas_results) {
//                std::cout << formula_result << " ";
                if (true_values_set.find(formula_result) == true_values_set.end()) {
                    return false;
                }
            }
//            std::cout << std::endl;
        }

        return true;
    }

    void FormulaSolver::display_tautology(
            std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
            std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators) {

        for (auto& [op, table] : binary_logical_operators) {
            auto pair = std::make_pair(op, table);
            std::cout << const_cast<std::pair<LogicalOperator, BinaryTruthTable>&>(pair);
        }

        for (auto& [op, table] : unary_logical_operators) {
            std::cout <<  table << std::endl;
        }

        std::cout << std::endl << "==================" << std::endl;
    }
}

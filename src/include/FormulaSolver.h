#ifndef FORMULA_SOLVER_H
#define FORMULA_SOLVER_H

#include <istream>
#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "flex_bison/Scanner.h"
#include <list>
#include <mutex>
#include <memory>
#include "BinaryTruthTable.h"
#include "include/FormulaEvaluator.h"

namespace formula_solver {

    std::shared_ptr<FormulaEvaluator> create_new_formula_evaluator(const FormulaEvaluator& original_evaluator);

    class FormulaSolver {
    public:
        FormulaSolver(std::shared_ptr<std::istream> input_stream, std::ostream& error_stream, int n, int k);

        void find_all_tautological_logical_operators();

        static bool check_tautology(
                FormulaEvaluator* local_formula_evaluator,
                const std::vector<std::vector<int>>& all_evaluations,
                const std::vector<int>& true_values,
                const std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
                const std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators
        );

        std::vector<std::vector<int>> generate_all_variables_evaluations() const;

        void initialize_data();

        void find_tautologies_in_range(
                FormulaEvaluator* local_evaluator,
                int indexStart,
                int indexEnd,
                std::mutex& output_mutex
        );

        FormulaEvaluator evaluator;

    private:
        std::vector<std::vector<int>> all_possible_evaluations;
        std::vector<BinaryTruthTable> binary_truth_tables;
        std::vector<UnaryTruthTable> unary_truth_tables;
        std::vector<int> true_values_in_logic;
        int total_binary_combinations;
        int total_unary_combinations;
        int total_unary_binary_table_combinations;

        std::vector<BinaryTruthTable> generate_all_truth_tables() const;
        std::vector<UnaryTruthTable> generate_all_unary_truth_tables() const;

        void display_tautology(
                std::unordered_map<LogicalOperator, BinaryTruthTable>& binary_logical_operators,
                std::unordered_map<LogicalOperator, UnaryTruthTable>& unary_logical_operators
        );
    };
}

#endif

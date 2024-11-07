#ifndef FORMULA_SOLVER_H
#define FORMULA_SOLVER_H

#include <istream>
#include <ostream>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include "Scanner.h"
#include <list>
#include "BinaryTruthTable.h"
#include "FormulaEvaluator.h"
#include "FormulaParserParams.h"
namespace formula_solver {

    class FormulaSolver {
    public:
        FormulaSolver(std::istream &input_stream, std::ostream &error_stream, int n, int k);
        void find_all_tautological_logical_operators();
        FormulaEvaluator evaluator;
        int parse_with_params(FormulaParserParams params);
    private:
        std::vector<std::unordered_map<char, int>> generate_all_variables_evaluations() const;
        std::vector<BinaryTruthTable> generate_all_truth_tables() const;
    };
}

#endif

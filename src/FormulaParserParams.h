
#ifndef FORMULAPARSERPARAMS_H
#define FORMULAPARSERPARAMS_H

#include <list>
#include <unordered_map>
#include "BinaryTruthTable.h"

namespace formula_solver {
    class FormulaParserParams {
    public:
        std::vector<BinaryTruthTable> logical_operators;
        std::unordered_map<char, int> evaluations;
        int evaluation_result;
        std::string formula;
        FormulaParserParams(const std::string& formula)
                :  evaluation_result(0), formula(formula) {}
    };
}
#endif //FORMULAPARSERPARAMS_H

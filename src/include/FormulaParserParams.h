
#ifndef FORMULAPARSERPARAMS_H
#define FORMULAPARSERPARAMS_H

#include <list>
#include <unordered_map>
#include "BinaryTruthTable.h"

namespace formula_solver {
    class FormulaParserParams {
        std::vector<BinaryTruthTable> logical_operators;
        std::unordered_map<char, int> variable_evaluations;
        std::string formula;
        int formula_result;
    };
}
#endif //FORMULAPARSERPARAMS_H

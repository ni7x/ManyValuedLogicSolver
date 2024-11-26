//
// Created by kpiot on 20.11.2024.
//

#ifndef PARSERCONTEXT_H
#define PARSERCONTEXT_H

#include "Scanner.h"
#include "FormulaParserParams.h"

namespace formula_solver {
    class ParserContext {
    public:
        Parser parser;
        Scanner scanner;
        std::istream& input_stream;
        std::vector<BinaryTruthTable> logical_operators;
        std::unordered_map<char, int> evaluations;
        int evaluation_result;

        ParserContext(std::istream& input)
                : input_stream(input),
                  scanner(input, std::cerr),
                  parser(&scanner, this),
                  evaluation_result(0){
        }

        int evaluate() {
            parser.parse();
            input_stream.clear();
            input_stream.seekg(0, std::ios::beg);
            return evaluation_result;
        }
    };
}
#endif //PARSERCONTEXT_H

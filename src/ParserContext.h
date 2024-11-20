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
        const std::string& input_stream;
        FormulaParserParams &params;

        ParserContext(const std::string& input, FormulaParserParams &params)
                : input_stream(input),
                  scanner(input, std::cerr),
                  parser(&scanner, &params),
                  params(params) {
            std::cout << "XD";
        }

        void update_evaluations(const std::unordered_map<char, int> &new_evaluations) {
            params.evaluations = new_evaluations;
        }

        int evaluate() {
            parser.parse();
            return params.evaluation_result;

        }
    };
}
#endif //PARSERCONTEXT_H

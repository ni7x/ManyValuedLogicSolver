#ifndef SCANNER_H
#define SCANNER_H

#include <sstream>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#if !defined(yyFlexLexerOnce)
#include "flex_bison/FlexLexer.h"
#endif

#include "generated_files/FormulaParser.tab.h"
#include "LogicalOperator.h"
#include <set>

namespace formula_solver {

class Scanner : public yyFlexLexer {
public:
    Scanner(std::istream& arg_yyin, std::ostream& arg_yyout)
        : yyFlexLexer(arg_yyin, arg_yyout) {}

    Scanner(std::istream* arg_yyin = nullptr, std::ostream* arg_yyout = nullptr)
        : yyFlexLexer(arg_yyin, arg_yyout) {}

    Scanner(const std::string& input, std::ostream& arg_yyout)
        : yyFlexLexer(input_stream, arg_yyout), input_stream(input) {}

    int lex(Parser::semantic_type *yylval);
    std::set<LogicalOperator> used_operators;
    int number_of_formulas = 1;

private:
    std::istringstream input_stream;
};

} // namespace formula_solver

#endif // SCANNER_H

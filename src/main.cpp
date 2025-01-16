#include<iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <chrono> 
#include <iomanip> 
#include <ctime>   
#include <fstream>

#include "flex_bison/Scanner.h"
#include "include/FormulaSolver.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <k>\n";
        return 1;
    }
    int n, k;
    try {
        n = std::stoi(argv[1]);
        k = std::stoi(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing arguments: " << e.what() << '\n';
        return 1;
    }

    std::cout << "----------ENTER FORMULA----------" << std::endl;

    std::istringstream input_stream("(p=>q) => ((q => r) =>(p=>r)), (!p => p) => p, p => (!p => q)");

    std::shared_ptr<std::istream> shared_input_stream = std::make_shared<std::istringstream>(input_stream.str());

    formula_solver::FormulaSolver solver(shared_input_stream, std::cerr, n, k);

    try {
        solver.find_all_tautological_logical_operators();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

void formula_solver::Parser::error(const std::string& msg) {
    throw std::runtime_error(msg);
}

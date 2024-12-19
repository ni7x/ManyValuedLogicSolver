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

    std::string input_line;
    std::getline(std::cin, input_line);
    std::istringstream input_formula(input_line);
    std::vector<int> variable_evaluations = {0, 1};

    formula_solver::FormulaSolver solver(input_formula, std::cerr, n, k);

    //solver.evaluator.evaluate_formula(variable_evaluations);
    auto ev =  formula_solver::create_new_formula_evaluator(solver.evaluator);
    ev->input_stream.clear();
    std::cout << "D";
    ev->input_stream.seekg(0, std::ios::beg);
    std::cout << "X";
    ev->evaluate_formula(variable_evaluations);
    std::cout << "1" << std::endl;

   // solver.find_tautologies_in_range(solver.evaluator, 0, 1000);
    //solver.find_all_tautological_logical_operators();
    std::cout << "2" << std::endl;
    std::cout << std::endl;

}

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

void formula_solver::Parser::error(const std::string& msg) {
    throw std::runtime_error(msg);
}

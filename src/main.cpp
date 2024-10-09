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

#include "Scanner.h"
#include "FormulaSolver.h"

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
    formula_solver::FormulaSolver solver(input_formula, std::cerr, n, k);
    std::ofstream output_file("output.txt");

    solver.find_all_tautological_logical_operators();

    output_file.close();
}

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

void formula_solver::Parser::error(const std::string& msg) {
    throw std::runtime_error(msg);
}

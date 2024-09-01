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

    auto start_time = std::chrono::steady_clock::now();

    if (!output_file.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return 1; 
    }

    for (const auto& evaluation :  solver.generate_all_true_evaluations(n, k)) { 
        for (const auto& pair : evaluation) {
            output_file << pair.first << ": " << pair.second << " ";
        }
        output_file << std::endl;
    }

    std::cout << "--WRITING TO output.txt COMPLETED--" << std::endl;


    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Time elapsed: " << std::fixed << std::setprecision(2) << elapsed_seconds.count() << " seconds." << std::endl;

    output_file.close();
    return 0;
}

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

void formula_solver::Parser::error(const std::string& msg) {
    throw std::runtime_error(msg);
}

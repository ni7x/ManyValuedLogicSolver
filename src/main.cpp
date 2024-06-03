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

// TODO evaluate all narazie dziala tylko gdy liczba zmiennych zdaniowych <= liczba wartosci logicznych

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <k>\n";
        return 1;
    }
    
    {
        std::istringstream input_formula("a | b | c & d");
        formula_solver::FormulaSolver solver(input_formula, std::cerr, {{'a', 1.0}, {'b', 1.0}, {'c', 0}, {'d', 0}});
        std::cout << solver.evaluate_formula() << std::endl;
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

    if(!solver.is_formula_valid()){
        return 0;
    }

    auto start_time = std::chrono::steady_clock::now();


    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return 1; 
    }

    std::cout << "----------ALL TRUE EVALUATIONS FOR FORMULA(" << input_line << ") ARE IN OUTPUT.txt----------" << std::endl;

    for (const auto& evaluation :  solver.generate_all_true_evaluations(n, k)) { 
        for (const auto& pair : evaluation) {
            outputFile << pair.first << ": " << pair.second << " ";  
        }
        outputFile << std::endl;
    }

    std::cout << "DONE" << std::endl;
    auto end_time = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end_time - start_time;

    std::cout << "Time elapsed: " << std::fixed << std::setprecision(2) << elapsed_seconds.count() << " seconds." << std::endl;

    outputFile.close();

    return 0;
    
}

int yyFlexLexer::yylex() {
    throw std::runtime_error("Invalid call to yyFlexLexer::yylex()");
}

void formula_solver::Parser::error(const std::string& msg) {
    throw std::runtime_error(msg);
}

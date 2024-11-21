#include "FormulaEvaluator.h"
#include "FormulaParserParams.h"

namespace formula_solver {



    FormulaEvaluator::FormulaEvaluator(std::istream& input_stream, std::ostream& error_stream)
            : input_stream(input_stream), error_stream(error_stream)
              {

    }

    FormulaEvaluator::FormulaEvaluator(std::istream& input_stream, std::ostream& error_stream,
                                       int n, int k)
            : FormulaEvaluator(input_stream, error_stream) {
        number_of_logical_values = n;
        number_of_true_logical_values = k;
        logical_operators.resize(4, BinaryTruthTable(n));
        if(!is_formula_valid()){
            throw std::runtime_error("Formula is invalid. Couldn't create Formula Solver.");
        }else{
            std::cout << "--------FORMULA IS CORRECT-------" << std::endl<< std::endl;;
        }
    }


    void FormulaEvaluator::reset_input() {
        input_stream.clear();
        input_stream.seekg(0, std::ios::beg);
    }

    void FormulaEvaluator::parse_and_reset(){

        reset_input();
    }

    bool FormulaEvaluator::is_formula_valid() {
        try {
            is_evaluation_mode = false;
            parse_and_reset();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid formula syntax. " << e.what() << std::endl;
            return false;
        }
    }


    void FormulaEvaluator::add_variable_name(char variable_name) {
        variable_evaluations[variable_name] = 0;
    }



    int FormulaEvaluator::get_variable_value(char variable) {
        if (is_evaluation_mode) {
            auto it = variable_evaluations.find(variable);
            if (it != variable_evaluations.end()) {
                return it->second;
            } else {
                throw std::runtime_error("Variable not found in evaluations.");
            }
        } else {
            add_variable_name(variable);
            return 0;
        }
    }

    void FormulaEvaluator::set_formula_evaluation_result(int result) {
        formula_evaluation_result = result;
    }

    void FormulaEvaluator::set_variables(const std::map<char, int>& new_variable_evaluations) {
        variable_evaluations = new_variable_evaluations;
    }

}

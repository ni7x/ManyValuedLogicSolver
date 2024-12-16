#include "include/FormulaEvaluator.h"

namespace formula_solver {



    FormulaEvaluator::FormulaEvaluator(std::istream& input_stream, std::ostream& error_stream)
            : input_stream(input_stream), error_stream(error_stream),
              scanner(&input_stream, &error_stream), parser(&scanner, this){

    }

    FormulaEvaluator::FormulaEvaluator(std::istream& input_stream, std::ostream& error_stream,
                                       int n, int k)
            : FormulaEvaluator(input_stream, error_stream) {
        number_of_logical_values = n;
        number_of_true_logical_values = k;

        binary_logical_operators[LogicalOperator::AND] = BinaryTruthTable(n);
        binary_logical_operators[LogicalOperator::OR] = BinaryTruthTable(n);
        binary_logical_operators[LogicalOperator::IMPLICATION] = BinaryTruthTable(n);
        binary_logical_operators[LogicalOperator::EQUIVALENCE] = BinaryTruthTable(n);

        unary_logical_operators[LogicalOperator::NOT] = UnaryTruthTable(n);
        if(!is_formula_valid()){
            throw std::runtime_error("Formula is invalid. Couldn't create Formula Solver.");
        }else{
            this->used_operators = scanner.used_operators;
            this->number_of_formulas = scanner.number_of_formulas;
            variable_names.sort();
            variable_evaluations.resize(variable_names.size());
            int index = 0;
            for (const auto& variable : variable_names) {
                variable_index_map[variable] = index++;
            }
            std::cout << "--------FORMULA IS CORRECT-------" << std::endl<< std::endl;;
        }
    }

    void FormulaEvaluator::reset_input() {
        input_stream.clear();
        input_stream.seekg(0, std::ios::beg);
    }

    void FormulaEvaluator::parse_and_reset(){
        formulas_evaluations.clear();
        parser.parse();
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
        variable_names.push_back(variable_name);
    }

    std::list<int> FormulaEvaluator::evaluate_formula(const std::vector<int>& new_variable_evaluations, const std::unordered_map<LogicalOperator, BinaryTruthTable> binary_logical_operators, const std::unordered_map<LogicalOperator, UnaryTruthTable> unary_logical_operators){
        is_evaluation_mode = true;
        this->binary_logical_operators = binary_logical_operators;
        this->unary_logical_operators = unary_logical_operators;
        this->variable_evaluations = new_variable_evaluations;
        parse_and_reset();
        return formulas_evaluations;
    }

    int FormulaEvaluator::get_variable_value(char variable) {
        if (is_evaluation_mode) {
            auto it = std::find(variable_names.begin(), variable_names.end(), variable);
            if (it != variable_names.end()) {
                return variable_evaluations[variable_index_map[variable]];
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

    void FormulaEvaluator::set_variables(const std::vector<int>& new_variable_evaluations) {
        variable_evaluations = new_variable_evaluations;
    }

}

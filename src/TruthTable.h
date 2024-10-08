#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "LogicalOperator.h"
class TruthTable {
public:
    TruthTable(LogicalOperator logical_operator, int num_of_variables)
            : logical_operator(logical_operator), num_of_variables(num_of_variables) {
        if (num_of_variables <= 0) {
            throw std::invalid_argument("Number of variables must be positive.");
        }
        int num_elements = (num_of_variables * (num_of_variables + 1)) / 2; // 1 + 2 + ... + n -> suma ciagu artmetyczngo
        cells.resize(num_elements, 0);
    }

    class RowProxy {
    public:
        RowProxy(TruthTable& parent, int row) : parent(parent), row(row) {}

        double& operator[](int col) {
            if (row < col) {
                return parent.cells[parent.index(col, row)];
            }
            return parent.cells[parent.index(row, col)];
        }

    private:
        TruthTable& parent;
        int row;
    };

    RowProxy operator[](int row) {
        if (row < 0 || row >= num_of_variables) {
            throw std::out_of_range("Row index out of range.");
        }
        return RowProxy(*this, row);
    }

    void set_cells(const std::vector<double>& values) {
        if (values.size() != cells.size()) {
            throw std::invalid_argument("Values size must match the number of lower triangle elements.");
        }
        cells = values;
    }

    void print() {

        std::cout << logical_operator << "\t";
        for (int i = 0; i < num_of_variables; i++) {
            std::cout << i << "\t";
        }
        std::cout << std::endl;

        for (int i = 0; i < num_of_variables; i++) {
            std::cout << i << "\t";

            for (int j = 0; j < num_of_variables; j++) {
                std::cout << (*this)[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

private:
    int num_of_variables;
    LogicalOperator logical_operator;
    std::vector<double> cells;


    int index(int row, int col) const {
        return (row * (row + 1)) / 2 + col;
    }
};

#endif // TRUTHTABLE_H

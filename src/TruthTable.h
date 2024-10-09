#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "LogicalOperator.h"
class TruthTable {
public:
    TruthTable(int num_of_logical_values)
            : num_of_logical_values(num_of_logical_values) {
        if (num_of_logical_values <= 0) {
            throw std::invalid_argument("");
        }
        int num_elements = (num_of_logical_values * (num_of_logical_values + 1)) / 2; // 1 + 2 + ... + n -> suma ciagu artmetyczngo
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
        if (row < 0 || row >= num_of_logical_values) {
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



    friend std::ostream& operator<<(std::ostream& os,  TruthTable& table) {
        os << "" << "\t";
        for (int i = 0; i < table.num_of_logical_values; i++) {
            os << i << "\t";
        }
        os << std::endl;

        for (int i = 0; i < table.num_of_logical_values; i++) {
            os << i << "\t";
            for (int j = 0; j < table.num_of_logical_values; j++) {
                os << table[i][j] << "\t";
            }
            os << std::endl;
        }
        return os;
    }

private:
    int num_of_logical_values;
   // LogicalOperator logical_operator;
    std::vector<double> cells;


    int index(int row, int col) const {
        return (row * (row + 1)) / 2 + col;
    }
};

#endif // TRUTHTABLE_H

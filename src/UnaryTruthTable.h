#ifndef UNARYTRUTHTABLE_H
#define UNARYTRUTHTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "LogicalOperator.h"
class UnaryTruthTable {
public:
    UnaryTruthTable(int num_of_logical_values)
            : num_of_logical_values(num_of_logical_values) {
        if (num_of_logical_values <= 0) {
            throw std::invalid_argument("");
        }
        cells.resize(num_of_logical_values, 0);
    }

    void set_cells(const std::vector<int>& values) {
        if (values.size() != cells.size()) {
            throw std::invalid_argument("Invalid size");
        }
        cells = values;
    }

private:
    int num_of_logical_values;
    std::vector<int> cells;

};

#endif // UNARYTRUTHTABLE_H

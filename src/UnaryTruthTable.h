#ifndef UNARYTRUTHTABLE_H
#define UNARYTRUTHTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "LogicalOperator.h"
#include "TruthTable.h"

class UnaryTruthTable : TruthTable {
public:
    UnaryTruthTable(int num_of_logical_values)
            : num_of_logical_values(num_of_logical_values) {
        if (num_of_logical_values <= 0) {
            throw std::invalid_argument("");
        }
        cells.resize(num_of_logical_values, 0);
    }

    UnaryTruthTable& operator=(const std::vector<int>& other){
        int expected_size = num_of_logical_values;
        if (other.empty() || other.size() != expected_size) {
            throw std::invalid_argument("Input vector must be square and non-empty.");
        }

        cells = other;
        return *this;
    }


    int& operator[](int index) {
        if (index < 0 || index >= num_of_logical_values) {
            throw std::out_of_range("Index out of bounds.");
        }
        return cells[index];
    }

    friend std::ostream& operator<<(std::ostream& os, UnaryTruthTable& table) {
        os << "" << "\t";
        for (int i = 0; i < table.num_of_logical_values; i++) {
            os << i << "\t";
        }
        os << std::endl;

        for (int i = 0; i < table.num_of_logical_values; i++) {
                os << table[i] << "\t";
        }
        return os;
    }

private:
    int num_of_logical_values;
    std::vector<int> cells;

};

#endif // UNARYTRUTHTABLE_H

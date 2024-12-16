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

        cells.resize(num_of_logical_values, 0);
    }

    UnaryTruthTable() : num_of_logical_values(0) {
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
            std::cout << " wrong unary  " << index << std::endl;
            throw std::out_of_range("Index outofbounds");
        }
        return cells[index];
    }

    friend std::ostream& operator<<(std::ostream& os, UnaryTruthTable& table) {

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

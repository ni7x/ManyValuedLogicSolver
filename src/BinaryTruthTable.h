#ifndef BINARYTRUTHTABLE_H
#define BINARYTRUTHTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "LogicalOperator.h"

class BinaryTruthTable {
public:
    BinaryTruthTable(int num_of_logical_values)
            : num_of_logical_values(num_of_logical_values) {
        if (num_of_logical_values <= 0) {
            throw std::invalid_argument("Number of logical values must be greater than zero.");
        }
        cells.resize(num_of_logical_values, std::vector<int>(num_of_logical_values, 0));
    }

    BinaryTruthTable() : num_of_logical_values(0) {}

    std::vector<int>& operator[](int index) {
        if (index < 0 || index >= num_of_logical_values) {
            std::cout << " wrpmg " << index << std::endl;
            throw std::out_of_range("Index outofbounds");
        }
        return cells[index];
    }

    BinaryTruthTable& operator=(const std::vector<std::vector<int>>& other) {
        if (other.empty() || other.size() != other[0].size()) {
            throw std::invalid_argument("Input vector must be square and non-empty.");
        }

        for (int i = 0; i < num_of_logical_values; ++i) {
            for (int j = 0; j < num_of_logical_values; ++j) {
                cells[i][j] = other[i][j];
            }
        }

        return *this;
    }

    BinaryTruthTable& operator=(const std::vector<int>& other){
        int expected_size = num_of_logical_values * num_of_logical_values;
        if (other.empty() || other.size() != expected_size) {
            throw std::invalid_argument("Input vector must be square and non-empty.");
        }

        for (int row = 0; row < num_of_logical_values; ++row) {
            for (int col = 0; col < num_of_logical_values; ++col) {
                cells[row][col] = other[row * num_of_logical_values + col];
            }
        }
        return *this;
    }


    friend std::ostream& operator<<(std::ostream& os, BinaryTruthTable& table) {
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




public:
    int num_of_logical_values;
    std::vector<std::vector<int>> cells;
};

#endif // BINARYTRUTHTABLE_H

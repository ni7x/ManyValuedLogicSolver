#ifndef LOGICAL_OPERATOR_H
#define LOGICAL_OPERATOR_H

#include <string>
#include <iostream>

enum class LogicalOperator {
    AND,
    OR,
    IMPLICATION,
    EQUIVALENCE,
    NOT
};

enum class OperatorType {
    UNARY,
    BINARY,
    UNKNOWN
};

inline OperatorType get_operator_type(LogicalOperator op) {
    switch (op) {
        case LogicalOperator::NOT:
            return OperatorType::UNARY;
        case LogicalOperator::AND:
        case LogicalOperator::OR:
        case LogicalOperator::IMPLICATION:
        case LogicalOperator::EQUIVALENCE:
            return OperatorType::BINARY;
        default:
            return OperatorType::UNKNOWN;
    }
}

 inline std::ostream& operator<<(std::ostream& os, const LogicalOperator op) {
    switch (op) {
        case LogicalOperator::AND:
            os << "&";
            break;
        case LogicalOperator::OR:
            os << "|";
            break;
        case LogicalOperator::IMPLICATION:
            os << "=>";
            break;
        case LogicalOperator::EQUIVALENCE:
            os << "<=>";
            break;
        case LogicalOperator::NOT:
            os << "!";
            break;
        default:
            os << "Unknown";
            break;
    }
    return os;
}
#endif
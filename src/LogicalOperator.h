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
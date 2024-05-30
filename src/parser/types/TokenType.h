#include <iostream>

#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

enum class TokenType {
    VARIABLE,
    NUMBER,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    INVALID_TOKEN,
    END_OF_FORMULA,
    NOT,
    AND,
    OR,
    IMPLICATION,
    EQUIVALENCE

};


std::ostream& operator<<(std::ostream& os, const TokenType tokenType) {
    switch (tokenType) {
        case TokenType::VARIABLE:
            os << "Variable";
            break;
        case TokenType::NUMBER:
            os << "Number";
            break;
        case TokenType::LEFT_PARENTHESIS:
            os << "Left Parenthesis";
            break;
        case TokenType::RIGHT_PARENTHESIS:
            os << "Right Parenthesis";
            break;
        case TokenType::INVALID_TOKEN:
            os << "Invalid Token";
            break;
        case TokenType::END_OF_FORMULA:
            os << "End of Formula";
            break;
        default:
            os << "Unknown Token";
            break;
    }
    return os;
}

#endif // TOKEN_TYPE_H
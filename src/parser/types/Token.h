#include "TokenType.h"
#include <iostream>

#ifndef TOKEN_H
#define TOKEN_H

class Token {
public:
    TokenType type;

    Token(TokenType type = TokenType::INVALID_TOKEN) : type(type) {}

    friend std::ostream& operator<<(std::ostream &os, const Token& token);

    virtual ~Token() = default;

    virtual char getValue() const {
        return ' ';
    }
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
    return os << token.type;
}


#endif //TOKEN_H

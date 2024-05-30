#ifndef VALUETOKEN_H
#define VALUETOKEN_H

#include "Token.h"

class ValueToken : public Token {
public:
    char value;

    ValueToken(TokenType type, const char& value) : Token(type), value(value) {}

    char getValue() const override {
        return value;
    }
};


#endif //VALUETOKEN_H

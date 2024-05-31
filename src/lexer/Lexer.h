#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "../parser/types/Token.h"
#include "../parser/types/ValueToken.h"

class Lexer {
private:
    std::string input;
    size_t current_position;

    char currentChar() const {
        return input[current_position];
    }

    void advance() {
        current_position++;
    }

public:
    Lexer(const std::string& input) : input(input), current_position(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        while (current_position < input.size()) {
            char current_char = currentChar();

            switch (current_char) {
                case ' ':
                    break;
                case '(':
                    tokens.push_back(Token(TokenType::LEFT_PARENTHESIS));
                    break;
                case ')':
                    tokens.push_back(Token(TokenType::RIGHT_PARENTHESIS));
                    break;
                case '&':
                    tokens.push_back(Token(TokenType::AND));
                    break;
                case '|':
                    tokens.push_back(Token(TokenType::OR));
                    break;
                case '~':
                    tokens.push_back(Token(TokenType::NOT));
                    break;
                case '<':
                    if (current_position + 2 < input.size() &&
                        input[current_position + 1] == '=' && input[current_position + 2] == '>') {
                        tokens.push_back(Token(TokenType::EQUIVALENCE));
                        current_position += 2;
                    }
                    break;
                case '=':
                    if (current_position + 1 < input.size() && input[current_position + 1] == '>') {
                        tokens.push_back(Token(TokenType::IMPLICATION));
                        advance();
                    }
                    break;
                default:
                    if (isalpha(current_char)) {
                        tokens.push_back(ValueToken(TokenType::VARIABLE, current_char));
                    } else if (isdigit(current_char)) {
                        tokens.push_back(ValueToken(TokenType::NUMBER, current_char));
                    } else {
                        tokens.push_back(Token(TokenType::INVALID_TOKEN));
                    }
                    break;
            }
            advance();
        }
        return tokens;
    }
};

#endif // LEXER_H

#include <QApplication>
#include <QPushButton>
#include "src/parser/lexer/Lexer.h"

int main(int argc, char *argv[]) {
    Lexer lexer = Lexer("(a|b) => c <=> d");
    for(auto token: lexer.tokenize()){
        std::cout << token;
    }
}

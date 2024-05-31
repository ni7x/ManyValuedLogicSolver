#include <cstdio>
#include <cstdlib>

extern "C" {
int yyparse(void);
void yyerror(const char *s);
}

int main() {
    if (yyparse() == 0) {
        printf("Parsing completed successfully.\n");
    } else {
        printf("Parsing failed.\n");
    }
    return 0;
}
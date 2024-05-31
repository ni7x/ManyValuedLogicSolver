#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
void yy_scan_string(const char* s);

char* read_input_line(char* buffer, size_t size);
void parse_and_test(const char* input_filename, const char* output_filename);
void test_formula(const char* formula, FILE* output_file);

int main(int argc, char *argv[]) {
    if (argc == 3) {
        printf("\n\n-----RUNNING TESTS------\n\n");
        parse_and_test(argv[1], argv[2]);
    }else{
        printf("\nENTER LOGICAL FORMULA: \n");
        char input[256];
        yyin = stdin;
        read_input_line(input, sizeof(input));
        yy_scan_string(input);
        yyparse();
    }

    return 0;
}

void parse_and_test(const char* input_filename, const char* output_filename) {
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    char input[256];
    while (fgets(input, sizeof(input), input_file) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        test_formula(input, output_file);
    }

    fclose(input_file);
    fclose(output_file);
}

void test_formula(const char* formula, FILE* output_file) {
    yy_scan_string(formula);
    int parse_result = yyparse();
    if (parse_result == 0) {
        fprintf(output_file, "Parsed: %s\n", formula);
    } else {
        fprintf(output_file, "Not Parsed: %s\n", formula);
    }
}

char* read_input_line(char* buffer, size_t size) {
    int c;
    size_t i = 0;
    while ((c = getchar()) != '\n' && c != EOF && i < size - 1) {
        buffer[i++] = c;
    }
    buffer[i] = '\0'; 
    return buffer;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
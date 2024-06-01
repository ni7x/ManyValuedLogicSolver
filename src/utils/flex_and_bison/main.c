#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_params.h"
#include <math.h>

extern int yyparse();
extern FILE* yyin;

int  yyparse (ParserParams *parse_params);
void yy_scan_string(const char* s);

char* read_input_line(char* buffer, size_t size);
void parse_and_test(const char* input_filename, const char* output_filename);
void test_formula(const char* formula, FILE* output_file);
void parse_and_compare_test(const char* input_filename, const char* output_filename);

float map_letter_to_value(char letter) {
    if (letter >= 'a' && letter <= 'z') {
        return ((float)(letter - 'a')) / 25.0f;
    } else {
        fprintf(stderr, "Error: Invalid variable name '%c'\n", letter);
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char *argv[]) {
    if (argc == 3) {
        printf("\n\n-----RUNNING TESTS------\n\n");
        parse_and_compare_test(argv[1], argv[2]);
    }else{
        printf("\nENTER LOGICAL FORMULA: \n");
        char input[256];
        yyin = stdin;
        read_input_line(input, sizeof(input));
        yy_scan_string(input);
        ParserParams params;
        params.map_function = &map_letter_to_value;
        float parsed_result;
        params.result = &parsed_result;
        yyparse(&params);
        printf("Parsed result: %.2f\n", parsed_result);
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
    ParserParams params;
    params.map_function = &map_letter_to_value;
    float parsed_result;
    params.result = &parsed_result;
    int parse_result = yyparse(&params);
    if (parse_result == 0) {
        fprintf(output_file, "Parsed: %s = %.2f\n", formula, parsed_result);
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

void yyerror (ParserParams *parse_params, const char *msg){
    fprintf(stderr, "Parse error: %s\n", msg);
}

void parse_and_compare_test(const char* input_filename, const char* output_filename) {
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

        // Split the input into formula and expected result
        char *token = strtok(input, ";");
        if (token != NULL) {
            char formula[128];
            strcpy(formula, token);
            token = strtok(NULL, ";");
            if (token != NULL) {
                float expected_result = atof(token);
                yy_scan_string(formula);
                ParserParams params;
                float parsed_result;
                params.result = &parsed_result;
                params.map_function = &map_letter_to_value;
                int parse_result = yyparse(&params);
                if (parse_result == 0) {
                    if (fabs(parsed_result - expected_result) < 1e-6) {
                        fprintf(output_file, "Test Passed: %s = %.2f (Expected: %.2f)\n", formula, parsed_result, expected_result);
                    } else {
                        fprintf(output_file, "Test Failed: %s = %.2f (Expected: %.2f)\n", formula, parsed_result, expected_result);
                    }
                } else {
                    fprintf(output_file, "Not Parsed: %s\n", formula);
                }
            }
        }
    }

    fclose(input_file);
    fclose(output_file);
}
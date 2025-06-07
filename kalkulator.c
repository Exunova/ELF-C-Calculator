#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Deklarasi Fungsi
double evaluate_expression(const char **expr);
double evaluate_term(const char **expr);
double evaluate_factor(const char **expr);
double evaluate_scientific_function(const char **expr);
void skip_spaces(const char **expr);


int main() {
    char expression[256];
    bool running = true;

    printf("=== KALKULATOR ARITMATIKA & SCIENTIFIC ===\n\n");
    printf("Petunjuk penggunaan:\n");
    printf("- Gunakan operator +, -, *, / untuk operasi aritmatika.\n");
    printf("- Gunakan tanda kurung untuk operasi dengan prioritas.\n");
    printf("- Fungsi scientific yang tersedia:\n");
    printf("  * sin(x), cos(x), tan(x) -> dalam derajat\n");
    printf("  * akar(x, y ) -> x akar y\n");
    printf("  * exponen(x,y) -> perpangkatan (x^y)\n\n");
    printf("Contoh input: (sin(30) + exponen(2,3)) * akar(16,2) - 3\n");
    
    while (running) {
        printf("\nMasukkan ekspresi (atau ketik 'exit' untuk keluar): ");
        fgets(expression, sizeof(expression), stdin);
        expression[strcspn(expression, "\n")] = 0;
        if (strcmp(expression, "exit") == 0) {
            printf("Keluar dari program.\n");
            running = false;
            break;
        }

        const char *expr_ptr = expression;
        double result = evaluate_expression(&expr_ptr);
        printf("Hasil: %.2f\n", result);
    }
    return 0;
}

// Fungsi Mengevaluasi Ekspresi
double evaluate_expression(const char **expr) {
    double result = evaluate_term(expr);
    while (**expr) {
        skip_spaces(expr);
        if (**expr == '+') {
            (*expr)++;
            result += evaluate_term(expr);
        } else if (**expr == '-') {
            (*expr)++;
            result -= evaluate_term(expr);
        } else {
            break;
        }
    }
    return result;
}

// Fungsi Perkalian & Pembagian
double evaluate_term(const char **expr) {
    double result = evaluate_factor(expr);
    while (**expr) {
        skip_spaces(expr);
        if (**expr == '*') {
            (*expr)++;
            result *= evaluate_factor(expr);
        } else if (**expr == '/') {
            (*expr)++;
            double divisor = evaluate_factor(expr);
            if (divisor == 0.0) {
                printf("Tidak dapat membagi dengan angka 0\n");
                exit(1);
            }
            result /= divisor;
        } else {
            break;
        }
    }
    return result;
}

// Fungsi Mengevaluasi Angka, Tanda Kurung, atau Fungsi Scientific
double evaluate_factor(const char **expr) {
    skip_spaces(expr);
    double result = 0.0;
    if (**expr == '(') {
        (*expr)++;
        result = evaluate_expression(expr);
        skip_spaces(expr);
        if (**expr == ')') {
            (*expr)++;
        }
    } else if (isalpha(**expr)) {
        result = evaluate_scientific_function(expr);
    } else {
        result = strtod(*expr, (char **)expr);
    }
    return result;
}

// Fungsi Mengevaluasi Scientific
double evaluate_scientific_function(const char **expr) {
    char func[10];
    int i = 0;
    while (isalpha(**expr) && i < 9) {
        func[i++] = **expr;
        (*expr)++;
    }
    func[i] = '\0';
    skip_spaces(expr);

    if (**expr == '(') {
        (*expr)++;
        double value1 = evaluate_expression(expr);
        skip_spaces(expr);

        double value2 = 0;
        if (**expr == ',') {
            (*expr)++;
            skip_spaces(expr);
            value2 = evaluate_expression(expr);
        }

        skip_spaces(expr);
        if (**expr == ')') {
            (*expr)++;
        }

        if (strcmp(func, "sin") == 0) return sin(value1 * M_PI / 180.0);
        if (strcmp(func, "cos") == 0) return cos(value1 * M_PI / 180.0);
        if (strcmp(func, "tan") == 0) return tan(value1 * M_PI / 180.0);
        if (strcmp(func, "akar") == 0) return pow(value1, 1.0 / value2);
        if (strcmp(func, "exponen") == 0) return pow(value1, value2);
    }
    printf("Fungsi tidak valid: %s\n", func);
    exit(1);
}

// Fungsi Skip Spasi Kosong
void skip_spaces(const char **expr) {
    while (isspace(**expr)) (*expr)++;
}

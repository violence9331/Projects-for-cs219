#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "variables.h"
#include "operations.h"

// 检查字符串是否为数字
int is_number(const char* str) {
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

// 处理表达式
double evaluate_expression(char* expr) {
    // 这里可以扩展为更复杂的表达式解析器
    // 目前只支持简单的二元运算
    char* op = strpbrk(expr, "+-*/^");
    if (!op) {
        // 如果没有操作符，可能是单个数字或变量
        if (is_number(expr)) {
            return atof(expr);
        } else {
            Variable* var = find_variable(expr);
            if (var) {
                return var->value;
            } else {
                printf("Error: Variable '%s' not found.\n", expr);
                exit(1);
            }
        }
    }

    char left[100], right[100];
    strncpy(left, expr, op - expr);
    left[op - expr] = '\0';
    strcpy(right, op + 1);

    double a = evaluate_expression(left);
    double b = evaluate_expression(right);

    switch (*op) {
        case '+': return add(a, b);
        case '-': return subtract(a, b);
        case '*': return multiply(a, b);
        case '/': return divide(a, b);
        case '^': return power(a, b);
        default:
            printf("Error: Unknown operator '%c'.\n", *op);
            exit(1);
    }
}
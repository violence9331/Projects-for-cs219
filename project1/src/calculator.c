#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"
#include "parser.h"
#include "variables.h"
#include "operations.h"
#include <string.h>
#include "operations.h"

// 处理命令行参数
void handle_command_line(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <number> <operator> <number>\n", argv[0]);
        exit(1);
    }

    double a = atof(argv[1]);
    double b = atof(argv[3]);
    char* op = argv[2];

    double result;
    if (strcmp(op, "+") == 0) {
        result = add(a, b);
    } else if (strcmp(op, "-") == 0) {
        result = subtract(a, b); 
    } else if (strcmp(op, "*") == 0) {
        result = multiply(a, b);
    } else if (strcmp(op, "/") == 0) {
        result = divide(a, b);
    } else {
        printf("Error: Invalid operator '%s'.\n", op);
        exit(1);
    }

    printf("%g %s %g = %g\n", a, op, b, result);
}

// 交互模式
void interactive_mode() {
    char input[100];
    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // 去掉换行符
        input[strcspn(input, "\n")] = '\0';

        // 处理退出命令
        if (strcmp(input, "quit") == 0) {
            break;
        }

        // 处理变量赋值
        char* eq = strchr(input, '=');
        if (eq) {
            *eq = '\0';
            char* var_name = input;
            char* expr = eq + 1;
            double value = evaluate_expression(expr);
            set_variable(var_name, value);
            printf("%s = %g\n", var_name, value);
        } else {
            // 处理表达式
            double result = evaluate_expression(input);
            printf("%g\n", result);
        }
    }
}
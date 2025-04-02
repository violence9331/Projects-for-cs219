#include <math.h>
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

// 处理加法
double add(double a, double b) {
    return a + b;
}

// 处理减法
double subtract(double a, double b) {
    return a - b;
}

// 处理乘法
double multiply(double a, double b) {
    return a * b;
}

// 处理除法
double divide(double a, double b) {
    if (b == 0) {
        printf("Error: Division by zero.\n");
    }
    return a / b;
}

// 处理幂运算
double power(double a, double b) {
    return pow(a, b);
}
#ifndef VARIABLES_H
#define VARIABLES_H

#define MAX_VARIABLES 100
#define MAX_VAR_NAME_LENGTH 50

// 变量结构体
typedef struct {
    char name[MAX_VAR_NAME_LENGTH];
    double value;
} Variable;

// 变量相关函数声明
Variable* find_variable(const char* name);
void set_variable(const char* name, double value);

#endif // VARIABLES_H
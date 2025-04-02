#include <string.h>
#include "variables.h"
#include <stdio.h>
#include "variables.h"
// 全局变量数组
Variable variables[MAX_VARIABLES];
int variable_count = 0;

// 查找变量
Variable* find_variable(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

// 添加或更新变量
void set_variable(const char* name, double value) {
    Variable* var = find_variable(name);
    if (var) {
        var->value = value;
    } else {
        if (variable_count < MAX_VARIABLES) {
            strncpy(variables[variable_count].name, name, MAX_VAR_NAME_LENGTH);
            variables[variable_count].value = value;
            variable_count++;
        } else {
            printf("Error: Too many variables defined.\n");
        }
    }
}
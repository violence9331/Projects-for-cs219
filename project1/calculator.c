#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

#define MAX_STACK_SIZE 100
#define PI 3.14159265358979323846264338327950288
#define MAX_VARIABLES 100
#define MAX_VAR_NAME_LENGTH 50

// 变量结构体
typedef struct {
    char name[MAX_VAR_NAME_LENGTH];
    double value;
} Variable;
Variable variables[MAX_VARIABLES];
int variable_count = 0;
int mode = 0;  // 0: 弧度制，1: 角度制
int display_precision = 6;  // 显示结果的精度



//基本计算
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double power(double a, double b);
double sine(double a);
double cosine(double a);
double tangent(double a);
double natural_log(double a);
double log_base(double a, double b);
double square_root(double a);
double bin_to_dec(const char* bin);
double oct_to_dec(const char* oct);
double hex_to_dec(const char* hex);
void dec_to_bin(double dec, char* bin);
void dec_to_oct(double dec, char* oct);
void dec_to_hex(double dec, char* hex);

// 辅助函数
void showhelp();
void delete_variable(const char* name);
void clear_all_variables();
void save_variables(const char* filename);
void load_variables(const char* filename);
int precedence(char op);
void clear_console();
void print_startup_info();
int check_parentheses(char* expr);
int is_legal_char(char c);
int is_valid_variable_name(const char* name);
void remove_leading_trailing_spaces(char* str);
int check_function_parentheses(char* expr, int start);
int is_valid_function(const char* func_name);
int check_expression_validity(char* expr);
Variable* find_variable(const char* name);
void set_variable(const char* name, double value);
int is_number(const char* str);

// 中缀转后缀计算
void infix_to_postfix(char* infix, char* postfix);
double evaluate_postfix(char* postfix);
double evaluate_expression(char* expr);
int extract_inner_expression(char* expr, int start, char* inner_expr);

//交互逻辑
void handle_command_line(int argc, char* argv[]);
void interactive_mode();


// 定义运算符优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}


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
        return NAN;
    }
    return a / b;
}

// 处理幂运算
double power(double a, double b) {
    return pow(a, b);
}

// 处理正弦函数
double sine(double a) {
    if (mode == 1) {
        a = a * PI / 180.0; // 将角度转换为弧度
    }
    double n = a / PI;
    double remainder = fmod(n, 1.0);

    if (fabs(remainder) < DBL_EPSILON || fabs(remainder - 1.0) < DBL_EPSILON) {
        a = 0;
    }

    return sin(a);
}

// 处理余弦函数
double cosine(double a) {
    if (mode == 1) {
        a = a * PI / 180.0; // 将角度转换为弧度
    }
    double n = a / PI;
    double remainder = fmod(n, 1.0);

    if (fabs(remainder + 0.5) < DBL_EPSILON || fabs(remainder - 0.5) < DBL_EPSILON) {
        return 0;
    }

    return cos(a);
}

// 处理正切函数
double tangent(double a) {
    if (mode == 1) {
        a = a * PI / 180.0; // 将角度转换为弧度
    }
    double n = a / PI;
    double remainder = fmod(n, 1.0);

    if (fabs(remainder + 0.5) < DBL_EPSILON || fabs(remainder - 0.5) < DBL_EPSILON) {
        printf("Error: Tangent of odd multiple of PI/2 is undefined.\n");
        return NAN;
    }

    return tan(a);
}

// 处理自然对数函数
double natural_log(double a) {
    if (a <= 0) {
        printf("Error: Logarithm of non - positive number.\n");
        return NAN;
    }
    return log(a);
}

// 处理以 a 为底 b 的对数
double log_base(double a, double b) {
    if (a <= 0 || a == 1 || b <= 0) {
        printf("Error: Invalid arguments for logarithm. Base must be positive and not equal to 1, and argument must be positive.\n");
        return NAN;
    }
    return log(b) / log(a);
}

double square_root(double a) {
    if (a < 0) {
        printf("Error: Square root of negative number.\n");
        return NAN;
    }
    return sqrt(a);
}

// 二进制转十进制
double bin_to_dec(const char* bin) {
    // 检查输入是否只包含 0、1 和小数点
    for (int i = 0; bin[i] != '\0'; i++) {
        if (bin[i] != '0' && bin[i] != '1' && bin[i] != '.') {
            printf("Error: Invalid binary number format. Binary numbers should only contain 0, 1, and optionally a decimal point.\n");
            return NAN;
        }
    }
    double dec = 0;
    int len = strlen(bin);
    int dot_pos = -1;
    // 查找小数点位置
    for (int i = 0; i < len; i++) {
        if (bin[i] == '.') {
            dot_pos = i;
            break;
        }
    }

    if (dot_pos == -1) {
        for (int i = 0; i < len; i++) {
            if (bin[i] == '1') {
                dec += pow(2, len - i - 1);
            }
        }
    } else {
        // 处理整数部分
        for (int i = 0; i < dot_pos; i++) {
            if (bin[i] == '1') {
                dec += pow(2, dot_pos - i - 1);
            }
        }
        // 处理小数部分
        for (int i = dot_pos + 1; i < len; i++) {
            if (bin[i] == '1') {
                dec += pow(2, -(i - dot_pos));
            }
        }
    }
    return dec;
}

// 八进制转十进制
double oct_to_dec(const char* oct) {
    for (int i = 0; oct[i] != '\0'; i++) {
        if (!isdigit(oct[i]) || (oct[i] < '0' || oct[i] > '7') || (i > 0 && oct[i] == '.' && oct[i - 1] == '.')) {
            printf("Error: Invalid octal number format. Octal numbers should only contain 0-7 and optionally a decimal point.\n");
            return NAN;
        }
    }
    double dec = 0;
    int len = strlen(oct);
    int dot_pos = -1;
    for (int i = 0; i < len; i++) {
        if (oct[i] == '.') {
            dot_pos = i;
            break;
        }
    }

    if (dot_pos == -1) {
        for (int i = 0; i < len; i++) {
            dec += (oct[i] - '0') * pow(8, len - i - 1);
        }
    } else {
        for (int i = 0; i < dot_pos; i++) {
            dec += (oct[i] - '0') * pow(8, dot_pos - i - 1);
        }
        for (int i = dot_pos + 1; i < len; i++) {
            dec += (oct[i] - '0') * pow(8, -(i - dot_pos));
        }
    }
    return dec;
}

// 十六进制转十进制
double hex_to_dec(const char* hex) {
    for (int i = 0; hex[i] != '\0'; i++) {
        if (!isdigit(hex[i]) && (!isalpha(hex[i]) || (toupper(hex[i]) < 'A' || toupper(hex[i]) > 'F')) && hex[i] != '.') {
            printf("Error: Invalid hexadecimal number format. Hexadecimal numbers should only contain 0-9, a-f, A-F and optionally a decimal point.\n");
            return NAN;
        }
    }
    double dec = 0;
    int len = strlen(hex);
    int dot_pos = -1;
    for (int i = 0; i < len; i++) {
        if (hex[i] == '.') {
            dot_pos = i;
            break;
        }
    }

    if (dot_pos == -1) {
        for (int i = 0; i < len; i++) {
            if (isdigit(hex[i])) {
                dec += (hex[i] - '0') * pow(16, len - i - 1);
            } else if (isalpha(hex[i])) {
                dec += (toupper(hex[i]) - 'A' + 10) * pow(16, len - i - 1);
            }
        }
    } else {
        for (int i = 0; i < dot_pos; i++) {
            if (isdigit(hex[i])) {
                dec += (hex[i] - '0') * pow(16, dot_pos - i - 1);
            } else if (isalpha(hex[i])) {
                dec += (toupper(hex[i]) - 'A' + 10) * pow(16, dot_pos - i - 1);
            }
        }
        for (int i = dot_pos + 1; i < len; i++) {
            if (isdigit(hex[i])) {
                dec += (hex[i] - '0') * pow(16, -(i - dot_pos));
            } else if (isalpha(hex[i])) {
                dec += (toupper(hex[i]) - 'A' + 10) * pow(16, -(i - dot_pos));
            }
        }
    }
    return dec;
}

// 十进制转二进制
void dec_to_bin(double dec, char* bin) {
    if (dec < 0) {
        printf("Error: Input decimal number for conversion to binary should be non-negative.\n");
        return;
    }
    int integer_part = (int)dec;
    double fractional_part = dec - integer_part;

    int index = 0;
    if (integer_part == 0) {
        bin[index++] = '0';
    } else {
        while (integer_part > 0) {
            bin[index++] = (integer_part % 2) + '0';
            integer_part /= 2;
        }
        for (int i = 0; i < index / 2; i++) {
            char temp = bin[i];
            bin[i] = bin[index - i - 1];
            bin[index - i - 1] = temp;
        }
    }

    if (fractional_part > 0) {
        bin[index++] = '.';
        for (int i = 0; i < 10; i++) {
            fractional_part *= 2;
            if (fractional_part >= 1) {
                bin[index++] = '1';
                fractional_part -= 1;
            } else {
                bin[index++] = '0';
            }
        }
    }
    bin[index] = '\0';
}


void dec_to_oct(double dec, char* oct) {
    if (dec < 0) {
        printf("Error: Input decimal number for conversion to octal should be non - negative.\n");
        return;
    }
    int integer_part = (int)dec;
    double fractional_part = dec - integer_part;

    int index = 0;
    if (integer_part == 0) {
        oct[index++] = '0';
    } else {
        int stack[100];
        int top = -1;
        while (integer_part > 0) {
            stack[++top] = integer_part % 8;
            integer_part /= 8;
        }
        while (top >= 0) {
            oct[index++] = stack[top--] + '0';
        }
    }

    if (fractional_part > 0) {
        oct[index++] = '.';
        for (int i = 0; i < 10; i++) {
            fractional_part *= 8;
            int digit = (int)fractional_part;
            oct[index++] = digit + '0';
            fractional_part -= digit;
        }
    }
    oct[index] = '\0';
}

// 十进制转十六进制
void dec_to_hex(double dec, char* hex) {
    if (dec < 0) {
        printf("Error: Input decimal number for conversion to hexadecimal should be non - negative.\n");
        return;
    }
    int integer_part = (int)dec;
    double fractional_part = dec - integer_part;

    int index = 0;
    if (integer_part == 0) {
        hex[index++] = '0';
    } else {
        int stack[100];
        int top = -1;
        while (integer_part > 0) {
            stack[++top] = integer_part % 16;
            integer_part /= 16;
        }
        while (top >= 0) {
            int remainder = stack[top--];
            if (remainder < 10) {
                hex[index++] = remainder + '0';
            } else {
                hex[index++] = remainder - 10 + 'A';
            }
        }
    }

    if (fractional_part > 0) {
        hex[index++] = '.';
        for (int i = 0; i < 10; i++) {
            fractional_part *= 16;
            int digit = (int)fractional_part;
            if (digit < 10) {
                hex[index++] = digit + '0';
            } else {
                hex[index++] = digit - 10 + 'A';
            }
            fractional_part -= digit;
        }
    }
    hex[index] = '\0';
}

//检查变量名是否合法
int is_valid_variable_name(const char* name) {
    if (!isalpha(name[0])) {
        return 0;
    }
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalnum(name[i])) {
            return 0;
        }
    }
    return 1;
}

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
    char clean_name[MAX_VAR_NAME_LENGTH];
    strcpy(clean_name, name);
    remove_leading_trailing_spaces(clean_name);

    if (strcmp(clean_name, "pi") == 0) {
        printf("Error: 'pi' is a reserved constant and cannot be used as a variable name.\n");
        return;
    }

    if (!is_valid_variable_name(clean_name)) {
        printf("Error: Invalid variable name '%s'. Variable names must start with a letter and can only contain alphanumeric characters.\n", clean_name);
        return;
    }

    Variable* var = find_variable(clean_name);
    if (var) {
        var->value = value;
    } else {
        if (variable_count < MAX_VARIABLES) {
            strncpy(variables[variable_count].name, clean_name, MAX_VAR_NAME_LENGTH);
            variables[variable_count].value = value;
            variable_count++;
        } else {
            printf("Error: Too many variables defined.\n");
        }
    }
}

// 删除变量
void delete_variable(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            // 将后面的变量前移
            for (int j = i; j < variable_count - 1; j++) {
                strcpy(variables[j].name, variables[j + 1].name);
                variables[j].value = variables[j + 1].value;
            }
            variable_count--;
            printf("Variable '%s' deleted.\n", name);
            return;
        }
    }
    printf("Error: Variable '%s' not found.\n", name);
}
// 清空所有变量
void clear_all_variables() {
    variable_count = 0;
    printf("All variables cleared.\n");
}

// 保存变量到文件
void save_variables(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return;
    }
    for (int i = 0; i < variable_count; i++) {
        fprintf(file, "%s=%.*f\n", variables[i].name, display_precision, variables[i].value);
    }
    fclose(file);
    printf("Variables saved to '%s'.\n", filename);
}

// 从文件加载变量
void load_variables(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file '%s' for reading.\n", filename);
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char* eq = strchr(line, '=');
        if (eq) {
            *eq = '\0';
            char* name = line;
            char* value_str = eq + 1;
            double value = atof(value_str);
            set_variable(name, value);
        }
    }
    fclose(file);
    printf("Variables loaded from '%s'.\n", filename);
}
//清除变量后空格
void remove_leading_trailing_spaces(char* str) {
    int start = 0;
    while (isspace(str[start])) {
        start++;
    }
    int end = strlen(str) - 1;
    while (end >= start && isspace(str[end])) {
        end--;
    }
    int i;
    for (i = 0; start <= end; start++, i++) {
        str[i] = str[start];
    }
    str[i] = '\0';
}

// 中序表达式转后序表达式
void infix_to_postfix(char* infix, char* postfix) {
    char stack[MAX_STACK_SIZE];
    int top = -1;
    int j = 0;
    // 初始化 postfix 为空字符串
    postfix[0] = '\0';
    for (int i = 0; infix[i] != '\0'; i++) {
        if (isspace(infix[i])) continue;
        if (infix[i] == '-' && (i == 0 || infix[i - 1] == '(' || strchr("+-*/^", infix[i - 1]))) {
            // 将一元负号转换为 "0 -"
            postfix[j++] = '0';
            postfix[j++] = ' ';
            stack[++top] = '-'; // 将负号压栈
        }
        // 如果是数字或小数点，直接输出到后序表达式
        if (isdigit(infix[i]) || infix[i] == '.') {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' '; // 用空格分隔操作数
            i--; // 回退一个字符
        }
        // 如果是字母（可能是函数名或变量名）
        else if (isalpha(infix[i])) {
            char token[MAX_VAR_NAME_LENGTH];
            int k = 0;
            while (isalpha(infix[i])) {
                token[k++] = infix[i++];
            }
            token[k] = '\0';
            // 判断是否是函数名
            if (is_valid_function(token)) {
                 // 使用 'F' 标记函数名
                stack[++top] = '\0';
                for (int l = 0; l < strlen(token); l++) {
                    stack[++top] = token[l]; // 将函数名压入栈中
                }stack[++top] = 'F';
                 // 结束函数名
            } else {
                // 如果不是函数名，尝试作为变量处理
                for(int l = 0; l < strlen(token); l++) {
                    postfix[j++] = token[l];
                }
                postfix[j++] = ' ';
            }
            i--; // 回退一个字符
        }
        // 如果是左括号，压栈
        else if (infix[i] == '(') {
            stack[++top] = infix[i];
        }
        // 如果是右括号，弹出栈中的运算符直到遇到左括号
        else if (infix[i] == ')') {
            while (top >= 0 && stack[top] != '(') {
                postfix[j++] = stack[top--];
                postfix[j++] = ' ';
            }
            if (top >= 0 && stack[top] == '(') {
                top--; // 弹出 '('
                if (top >= 0 && stack[top] == 'F') {
                    top--; // 弹出函数名标记 'F'
                    char func_name[MAX_VAR_NAME_LENGTH];
                    int k = 0;
                    // 弹出函数名字符，直到遇到 '\0'
                    while (top >= 0 && stack[top] != '\0') {
                        func_name[k++] = stack[top--];
                    }
                    if (top >= 0 && stack[top] == '\0') {
                        top--; // 弹出 '\0'
                    }
                    func_name[k] = '\0'; // 结束字符串
        
                    // 反转函数名字符
                    for (int l = 0, r = k - 1; l < r; l++, r--) {
                        char temp = func_name[l];
                        func_name[l] = func_name[r];
                        func_name[r] = temp;
                    }
        
                    // 将函数名拼接到后序表达式中
                    if (func_name[0] != 'l')
                    {
                        for (int l = 0; l < k; l++) {
                        postfix[j++] = func_name[l];
                    }
                    postfix[j++] = ' ';
                    }    
                    if (strcmp(func_name, "log") == 0) {
                        int state = 0; 
                        while (infix[i]!= '(')
                        {
                            i--;
                        }
                        while (infix[i] != ')' && infix[i] != '\0') {
                            if (infix[i] == ',') {
                                state = 1;
                                i++; // 跳过逗号
                            }  else {
                                i++; // 跳过空格
                            }
                        }
                        // 根据参数数量处理函数
                        if (state == 1) {
                            char new_func_name[MAX_VAR_NAME_LENGTH];
                            strcpy(new_func_name, "logb");
                            // 将函数名称拼接到后序表达式中
                            for (int l = 0; l < strlen(new_func_name); l++) {
                                postfix[j++] = new_func_name[l];
                            }
                            postfix[j++] = ' ';
                    
                    
                        } else {
                            // 单参数，使用原始函数名称 "log"
                            for (int l = 0; l < strlen(func_name); l++) {
                                postfix[j++] = func_name[l];
                            }
                            postfix[j++] = ' ';
                        }
                        // 确保栈中的 '(' 被正确处理
                        while (top >= 0 && stack[top] != '(') {
                            postfix[j++] = stack[top--];
                            break;
                        }
                    }
                }
            }
        }
        // 如果是运算符
        else if (strchr("+-*/^", infix[i])) {
            while (top >= 0 && precedence(stack[top]) >= precedence(infix[i])) {
                postfix[j++] = stack[top--];
                postfix[j++] = ' ';
            }
            stack[++top] = infix[i];
        }
    }

    // 弹出栈中剩余的运算符
    while (top >= 0) {
        postfix[j++] = stack[top--];
        postfix[j++] = ' ';
    }

    postfix[j] = '\0'; // 结束字符串

    // 打印后序表达式
    printf("Generated postfix expression: \"%s\"\n", postfix);
}
//
int is_legal_char(char c) {
    return isdigit(c) || strchr("+-*/^() ,.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", c) != NULL;
}
//清空控制台
void clear_console() {
    printf("\033[2J\033[1;1H");
}
//检查整个表达式的括号
int check_parentheses(char* expr) {
    int paren_count = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == '(') {
            paren_count++;
        } else if (expr[i] == ')') {
            paren_count--;
            if (paren_count < 0) {
                printf("Error: Mismatched parentheses in expression. Extra closing parenthesis at position %d.\n", i);
                return 0;
            }
        }
    }
    if (paren_count != 0) {
        printf("Error: Mismatched parentheses in expression. %s parenthesis missing.\n", paren_count > 0 ? "Closing" : "Opening");
        return 0;
    }
    return 1;
}

// 检查函数名是否合法
int is_valid_function(const char* func_name) {
    const char* supported_functions[] = {"sin", "cos", "tan", "log", "sqrt", "sine", "cosine", "tangent","logb"};
    const int num_supported_functions = sizeof(supported_functions) / sizeof(supported_functions[0]);

    for (int i = 0; i < num_supported_functions; i++) {
        if (strcmp(func_name, supported_functions[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}
// 检查字符串是否为数字
int is_number(const char* str) {
    char* endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}
// 计算后序表达式
double evaluate_postfix(char* postfix) {
    double stack[MAX_STACK_SIZE];
    int top = -1;
    char* token = strtok(postfix, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // 如果是数字，压栈
            stack[++top] = atof(token);
        } else if (isalpha(token[0])) {
            if (strcmp(token, "pi") == 0) {
                // 处理 pi 常量
                stack[++top] = PI;
            } else if (is_valid_function(token)) {
                // 如果是函数名
                if (strcmp(token, "logb") == 0) {
                    double num2 = stack[top--]; 
                    double num1 = stack[top--]; 
                    stack[++top] = log_base(num1, num2); // 调用 log_base 方法
                } else{
                    double arg = stack[top--]; // 弹出参数
                    if (strcmp(token,"log") == 0) {
                        
                        stack[++top] = natural_log(arg);
                    } else if (strcmp(token, "sin") == 0 || strcmp(token, "sine") == 0) {
                        stack[++top] = sine(arg);
                    } else if (strcmp(token, "cos") == 0 || strcmp(token, "cosine") == 0) {
                        stack[++top] = cosine(arg);
                    } else if (strcmp(token, "tan") == 0 || strcmp(token, "tangent") == 0) {
                        stack[++top] = tangent(arg);
                    } else if (strcmp(token, "sqrt") == 0) {
                        stack[++top] = square_root(arg);
                    } else {
                        printf("Error: Unknown function '%s'.\n", token);
                        return NAN;
                    }
                }
            } else {
                // 如果是变量，查找其值
                Variable* var = find_variable(token);
                if (var) {
                    stack[++top] = var->value;
                } else {
                    printf("Error: Variable '%s' not found.\n", token);
                    return NAN;
                }
            }
        } else {
            double b = stack[top--];
                double a = stack[top--];
                switch (token[0]) {
                    case '+': stack[++top] = add(a,b); break;
                    case '-': stack[++top] = subtract(a,b); break;
                    case '*': stack[++top] = multiply(a,b); break;
                    case '/': stack[++top] = divide(a,b); break;
                    case '^': stack[++top] = pow(a, b); break;
                    default:
                        printf("Error: Unknown operator '%c'.\n", token[0]);
                        return NAN;
                }
        }
        token = strtok(NULL, " ");
    }

    return stack[top];
}

int check_function_parentheses(char* expr, int start) {
    int paren_count = 1;
    int i = start + 1; // 跳过函数名后的左括号
    while (expr[i] != '\0') {
        if (expr[i] == '(') {
            paren_count++;
        } else if (expr[i] == ')') {
            paren_count--;
            if (paren_count == 0) {
                return 1; // 括号匹配完整
            }
        }
        i++;
    }
    printf("Error: Mismatched parentheses in function call starting at position %d.\n", start);
    return 0;
}

// 定义 extract_inner_expression 函数
int extract_inner_expression(char* expr, int start, char* inner_expr) {
    int paren_count = 1;
    int i = start + 1; // 跳过函数名后的左括号
    int j = 0;
    while (expr[i] != '\0') {
        if (expr[i] == '(') {
            paren_count++;
        } else if (expr[i] == ')') {
            paren_count--;
            if (paren_count == 0) {
                break;
            }
        }
        inner_expr[j++] = expr[i++];
    }
    inner_expr[j] = '\0';
    return j > 0;
}

// 检查表达式的合法性
int check_expression_validity(char* expr) {
    for (int i = 0; expr[i] != '\0'; i++) {
        if (!is_legal_char(expr[i])) {
            printf("Error: Illegal character '%c' at position %d.\n", expr[i], i);
            return 0;
        }
    }
    if (!check_parentheses(expr)) {
        return 0;
    }
    // 检查函数调用合法性
    for (int i = 0; expr[i] != '\0'; i++) {
        if (isalpha(expr[i])) {
            char token[MAX_VAR_NAME_LENGTH];
            int k = 0;
            while (isalpha(expr[i])) {
                token[k++] = expr[i++];
            }
            token[k] = '\0';

            if (is_valid_function(token)) {
                if (expr[i] != '(') {
                    printf("Error: Function '%s' must be followed by an opening parenthesis at position %d.\n", token, i);
                    return 0;
                }
                if (!check_function_parentheses(expr, i)) {
                    return 0;
                }
            }
            i--;
        }
    }

    // 检查操作数和运算符的数量以及运算符位置
    int operand_count = 0;
    int operator_count = 0;
    int in_number = 0;
    for (int i = 0; expr[i] != '\0'; i++) {
        if (isspace(expr[i])) {
            in_number = 0;
            continue;
        }

        if (isdigit(expr[i]) || expr[i] == '.') {
            if (!in_number) {
                operand_count++;
                in_number = 1;
            }
        } else if (isalpha(expr[i])) {
            char token[MAX_VAR_NAME_LENGTH];
            int k = 0;
            while (isalpha(expr[i])) {
                token[k++] = expr[i++];
            }
            token[k] = '\0';

            if (is_valid_function(token)) {
                // 函数调用视为一个操作数
                operand_count++;
                // 跳过函数参数
                while (expr[i] != ')' && expr[i] != '\0') {
                    i++;
                }
            } else {
                // 变量视为一个操作数
                operand_count++;
            }
            i--;
        } else if (strchr("+-*/^", expr[i])) {
            // 检查是否是负号
            if (expr[i] == '-' && (i == 0 || expr[i - 1] == '(' || strchr("+-*/^", expr[i - 1]))) {
                // 一元负号，不增加运算符数量
                in_number = 0;
            } else {
                operator_count++;
                in_number = 0;
                if (i == 0) {
                    printf("Error: Operator '%c' at position %d is not in a valid position.\n", expr[i], i);
                    return 0;
                }
                int prev = i - 1;
                // 跳过前导空格
                while (prev >= 0 && isspace(expr[prev])) {
                    prev--;
                }
                if (prev < 0) {
                    printf("Error: Operator '%c' at position %d is not in a valid position.\n", expr[i], i);
                    return 0;
                }
                if (expr[prev] != ')' && !isdigit(expr[prev]) && !isalpha(expr[prev])) {
                    printf("Error: Operator '%c' at position %d is not in a valid position.\n", expr[i], i);
                    return 0;
                }
            }
        } else if (expr[i] == '(') {
            in_number = 0;
        } else if (expr[i] == ')') {
            in_number = 0;
        }
    }
    // 检查操作数之间是否缺少运算符
    if (operand_count > 1 && operator_count == 0) {
        printf("Error: Missing operator between operands.\n");
        return 0;
    }

    if (operand_count - operator_count != 1) {
        printf("Error: Mismatched number of operands and operators. Expected %d operands for %d operators, but found %d operands.\n", operator_count + 1, operator_count, operand_count);
        return 0;
    }

    return 1;
}
double evaluate_expression(char* expr) {
    if (!check_expression_validity(expr)) {
        return NAN;
    }
    char postfix[1000];
    infix_to_postfix(expr, postfix); // 中序转后序
    return evaluate_postfix(postfix); // 计算后序表达式
}

void handle_command_line(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <number> <operator> <number>\n", argv[0]);
        return;
    }

    if (!is_number(argv[1]) ||!is_number(argv[3])) {
        printf("Error: The input cannot be interpret as numbers.\n");
        return;
    }

    double a = atof(argv[1]);
    double b = atof(argv[3]);
    char* op = argv[2];

    double result;
    if (strcmp(op, "+") == 0) {
        result = add(a, b);
    } else if (strcmp(op, "-") == 0) {
        result = subtract(a, b);
    } else if (strcmp(op, "x") == 0 || strcmp(op, "*") == 0) {
        result = multiply(a, b);
    } else if (strcmp(op, "/") == 0) {
       result = divide(a, b);
    } else {
        printf("Error: Invalid operator '%s'.\n", op);
        return;
    }
    if (!isnan(result))
    {
        printf("%g %s %g = %g\n", a, op, b, result);
    }    
}


// 交互模式
void interactive_mode() {
    char input[100];
    while (1) {
        printf("> ");//识别命令
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0'; // 移除换行符
        if (strcmp(input, "quit") == 0) {
            break;
        }
        if (strcmp(input, "clear") == 0) {
            clear_console();
            continue;
        }
        if (strcmp(input, "-h") == 0)
        {
            showhelp();
            continue;
        }
        if (strncmp(input, "delete ", 7) == 0) {
            char* name = input + 7;
            if (name[0] == '\0') {
                printf("Error: No variable name provided for deletion.\n");
                continue;
            }
            delete_variable(name);
            continue;
        }
        if (strcmp(input, "clearvars") == 0) {
            clear_all_variables();
            continue;
        }
        if (strncmp(input, "savevars ", 9) == 0) {
            char* filename = input + 9;
            if (filename[0] == '\0') {
                printf("Error: No filename provided for saving variables.\n");
                continue;
            }
            save_variables(filename);
            continue;
        }
        if (strncmp(input, "loadvars ", 9) == 0) {
            char* filename = input + 9;
            if (filename[0] == '\0') {
                printf("Error: No filename provided for loading variables.\n");
                continue;
            }
            load_variables(filename);
            continue;
        }
        if (strcmp(input, "radian") == 0) {
            mode = 0;
            printf("Switched to radian mode.\n");
            continue;
        } else if (strcmp(input, "degree") == 0) {
            mode = 1;
            printf("Switched to degree mode.\n");
            continue;
        }
        if (strncmp(input, "precision ", 10) == 0) {
            int new_precision;
            if (sscanf(input + 10, "%d", &new_precision) == 1 && new_precision >= 0) {
                display_precision = new_precision;
                printf("Display precision set to %d digits.\n", display_precision);
            } else {
                printf("Error: Invalid precision value. Please enter a non - negative integer.\n");
            }
            continue;
        }
        if (strncmp(input, "bin2dec ", 7) == 0) {
            char bin[100];
            if (sscanf(input + 7, "%s", bin) != 1) {
                printf("Error: No binary number provided for conversion.\n");
                continue;
            }
            double dec = bin_to_dec(bin);
            if (!isnan(dec))
            {
                printf("%s (binary) = %.*f (decimal)\n", bin, display_precision, dec);
            }
            continue;
        } else if (strncmp(input, "oct2dec ", 7) == 0) {
            char oct[100];
            if (sscanf(input + 7, "%s", oct) != 1) {
                printf("Error: No octal number provided for conversion.\n");
                continue;
            }
            double dec = oct_to_dec(oct);
            if (!isnan(dec))
            {
                printf("%s (octal) = %.*f (decimal)\n", oct, display_precision, dec);
            }
            continue;
        } else if (strncmp(input, "hex2dec ", 7) == 0) {
            char hex[100];
            if (sscanf(input + 7, "%s", hex) != 1) {
                printf("Error: No hexadecimal number provided for conversion.\n");
                continue;
            }
            double dec = hex_to_dec(hex);
            if(!isnan(dec))
            { 
                printf("%s (hexadecimal) = %.*f (decimal)\n", hex, display_precision, dec);
            }
            continue;
        } else if (strncmp(input, "dec2bin ", 7) == 0) {
            double dec;
            if (sscanf(input + 7, "%lf", &dec) != 1) {
                printf("Error: No decimal number provided for conversion.\n");
                continue;
            }
            char bin[100];
            dec_to_bin(dec, bin);
            if (!isnan(dec))
            {
                printf("%.*f (decimal) = %s (binary)\n", display_precision, dec, bin);
            }
            continue;
        } else if (strncmp(input, "dec2oct ", 7) == 0) {
            double dec;
            if (sscanf(input + 7, "%lf", &dec) != 1) {
                printf("Error: No decimal number provided for conversion.\n");
                continue;
            }
            char oct[100];
            dec_to_oct(dec, oct);
            if (!isnan(dec))
            {
                printf("%.*f (decimal) = %s (octal)\n", display_precision, dec, oct);
            }
            continue;
        } else if (strncmp(input, "dec2hex ", 7) == 0) {
            double dec;
            if (sscanf(input + 7, "%lf", &dec) != 1) {
                printf("Error: No decimal number provided for conversion.\n");
                continue;
            }
            char hex[100];
            dec_to_hex(dec, hex);
            if (!isnan(dec))
            {
                printf("%.*f (decimal) = %s (hexadecimal)\n", display_precision, dec, hex);
            }
            continue;
        }
        if (strcmp(input, "showvars") == 0) {
            if (variable_count == 0) {
                printf("No variables defined.\n");
            } else {
                for (int i = 0; i < variable_count; i++) {
                    printf("%s = %.*g\n", variables[i].name, display_precision, variables[i].value);
                }
            }
            continue;
        }

        // 处理变量赋值
        char* eq = strchr(input, '=');
        if (eq) {
            *eq = '\0';
            char* var_name = input;
            char clean_name[MAX_VAR_NAME_LENGTH];
            strcpy(clean_name, var_name);
            remove_leading_trailing_spaces(clean_name);
            if (!is_valid_variable_name(clean_name)) {
                printf("Error: Invalid variable name '%s'.\n", clean_name);
                continue;
            }
            char* expr = eq + 1;
            double value = evaluate_expression(expr); // 使用中序转后序计算表达式
            if (!isnan(value)) {
                set_variable(clean_name, value);
                printf("%s = %.*g\n", clean_name, display_precision, value);
            }
        } else {
            // 处理表达式
            double result = evaluate_expression(input); // 使用中序转后序计算表达式
            if (!isnan(result)) {
                printf("%s = %.*g\n", input, display_precision, result);
            }
        }
    }
}

void print_startup_info() {
    printf("本计算器支持两种输入模式：\n");
    printf("1. 命令行模式：./calculator <数字> <运算符> <数字>，例如：./calculator 3 + 4\n");
    printf("在命令行模式由于读入限制，请用空格将不同运算元素分开,该模式支持二元四则运算，高级功能请前往交互模式\n");
    printf("2. 交互模式：直接运行程序名进入，可输入各种命令和表达式\n");
    printf("若需要查看详细帮助信息，请输入 -h\n");
}

void showhelp() {
    printf("### 函数支持\n");
    printf("- sin(x)、sine(x)：正弦函数，x 为角度或弧度（取决于当前模式）\n");
    printf("- cos(x)、cosine(x)：余弦函数，x 为角度或弧度（取决于当前模式）\n");
    printf("- tan(x)、tangent(x)：正切函数，x 为角度或弧度（取决于当前模式）\n");
    printf("- log(x)：自然对数函数，x 必须大于 0\n");
    printf("- log(x, y)：以 x 为底 y 的对数，x 必须大于 0 且不等于 1，y 必须大于 0\n");
    printf("- sqrt(x)：平方根函数，x 必须大于等于 0\n");
    printf("### 变量操作\n");
    printf("- 变量赋值：变量名=表达式，例如：x = 5 + 3\n");
    printf("- 变量名必须以字母开头，只能包含字母和数字，不能使用 'pi' 作为变量名\n");
    printf("- 显示变量：输入 showvars 查看当前定义的所有变量及其值\n");
    printf("### 变量支持\n");
    printf("- clearvars：清空所有变量\n");
    printf("- savevars <文件名>：将所有变量保存到文件\n");
    printf("- loadvars <文件名>：从文件加载变量\n");
    printf("- delete <变量名>：删除指定变量\n");
    printf("### 模式切换\n");
    printf("- radian：切换到弧度制模式\n");
    printf("- degree：切换到角度制模式\n");
    printf("### 精度设置\n");
    printf("- precision <n>：设置显示结果的精度为 n 位有效数字，n 为非负整数\n");
    printf("### 进制转换\n");
    printf("- bin2dec <二进制数>：将二进制数转换为十进制数\n");
    printf("- oct2dec <八进制数>：将八进制数转换为十进制数\n");
    printf("- hex2dec <十六进制数>：将十六进制数转换为十进制数\n");
    printf("- dec2bin <十进制数>：将十进制数转换为二进制数\n");
    printf("- dec2oct <十进制数>：将十进制数转换为八进制数\n");
    printf("- dec2hex <十进制数>：将十进制数转换为十六进制数\n");
    printf("### 退出程序\n");
    printf("- quit：退出交互模式\n");
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // 交互模式
        print_startup_info();
        interactive_mode();
    } else {
        // 命令行模式
        handle_command_line(argc, argv);
    }
    return 0;
}
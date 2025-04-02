#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 计算向量点积的函数
double dot_product(int *a, int *b, int n) {
    double result = 0;
    for (int i = 0; i < n; i++) {
        result += (double)a[i] * b[i];
    }
    return result;
}

int main() {
    // 向量长度
    int n = 1000000;
    // 为向量分配内存
    int *a = (int *)malloc(n * sizeof(int));
    int *b = (int *)malloc(n * sizeof(int));
    if (a == NULL || b == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return 1;
    }

    // 初始化随机数种子
    srand(time(NULL));
    // 生成随机向量
    for (int i = 0; i < n; i++) {
        a[i] = rand();
        b[i] = rand();
    }

    // 记录开始时间
    clock_t start = clock();
    // 计算点积
    double result = dot_product(a, b, n);
    // 记录结束时间
    clock_t end = clock();

    // 计算花费的时间
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 输出结果
    printf("向量点积: %lf\n", result);
    printf("计算耗时: %lf 秒\n", time_taken);

    // 释放分配的内存
    free(a);
    free(b);

    return 0;
}    
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


// 平凡算法：链式累加
double naive_sum(double *a, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

// 优化算法：两路链式累加
double optimized_sum(double *a, int n) {
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < n; i += 2) {
        sum1 += a[i];
        sum2 += a[i + 1];
    }
    return sum1 + sum2;
}

int main() {
    // 测试规模（2的幂次，覆盖 L1/L2/L3 及远超 L3）
    int sizes[] = {1024, 4096, 16384, 65536, 262144, 1048576, 4194304, 16777216};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // 重复次数：小规模多重复，大规模少重复，保证总运行时间稳定
    int repeats[] = {20000, 10000, 5000, 2000, 1000, 500, 200, 100};

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    printf("n\t\t数组大小(MB)\tNaive时间(s)\tOptimized时间(s)\n");

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = sizes[idx];
        int repeat = repeats[idx];
        double size_mb = (double)n * sizeof(double) / (1024 * 1024); // 数组占用内存

        double *a = (double*)malloc(n * sizeof(double));
        // 初始化：a[i] = i，便于验证正确性
        for (int i = 0; i < n; i++) {
            a[i] = i;
        }

        //验证一次正确性（打印一次和）
        if (idx == 0) {
            double sum_check = naive_sum(a, n);
            double expected = (double)n * (n - 1) / 2.0;
            printf("验证 n=%d：计算结果 %.0f，期望值 %.0f\n", n, sum_check, expected);
        }

        // 测试平凡算法
        QueryPerformanceCounter(&start);
        for (int r = 0; r < repeat; r++) {
            naive_sum(a, n);
        }
        QueryPerformanceCounter(&end);
        double time_naive = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart / repeat;

        // 测试优化算法
        QueryPerformanceCounter(&start);
        for (int r = 0; r < repeat; r++) {
            optimized_sum(a, n);
        }
        QueryPerformanceCounter(&end);
        double time_opt = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart / repeat;

        printf("%d\t\t%.2f\t\t%.6f\t\t%.6f\n", n, size_mb, time_naive, time_opt);

        free(a);
    }

    return 0;
}

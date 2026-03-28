#include <stdio.h>
#include <stdlib.h>
#include <windows.h>   // 使用 Windows 高精度计时

// 平凡算法：逐列访问
void naive(double *A, double *x, double *y, int n) {
    for (int j = 0; j < n; j++) {
        y[j] = 0.0;
        for (int i = 0; i < n; i++) {
            y[j] += A[i * n + j] * x[i];
        }
    }
}

// 优化算法：逐行访问
void optimized(double *A, double *x, double *y, int n) {
    for (int i = 0; i < n; i++) y[i] = 0.0;
    for (int i = 0; i < n; i++) {
        double xi = x[i];
        for (int j = 0; j < n; j++) {
            y[j] += A[i * n + j] * xi;
        }
    }
}

int main() {
  
    int sizes[] = {64, 128, 256, 512, 1024, 2048, 4096};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // 重复次数：n 小时重复多次，n 大时减少次数
    int repeats[] = {10000, 5000, 2000, 1000, 500, 200, 100};

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);   // 获取计时器频率

    printf("Matrix size (n)\tNaive time (s)\tOptimized time (s)\n");

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = sizes[idx];
        int repeat = repeats[idx];

        // 分配内存
        double *A = (double*)malloc(n * n * sizeof(double));
        double *x = (double*)malloc(n * sizeof(double));
        double *y1 = (double*)malloc(n * sizeof(double));
        double *y2 = (double*)malloc(n * sizeof(double));

        // 初始化：A[i][j] = i + j, x[i] = i
        for (int i = 0; i < n; i++) {
            x[i] = i;
            for (int j = 0; j < n; j++) {
                A[i * n + j] = i + j;
            }
        }

        // 测试平凡算法（重复运行）
        QueryPerformanceCounter(&start);
        for (int r = 0; r < repeat; r++) {
            naive(A, x, y1, n);
        }
        QueryPerformanceCounter(&end);
        double time_naive = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart / repeat;

        // 测试优化算法（重复运行）
        QueryPerformanceCounter(&start);
        for (int r = 0; r < repeat; r++) {
            optimized(A, x, y2, n);
        }
        QueryPerformanceCounter(&end);
        double time_opt = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart / repeat;

        // 输出结果
        printf("%d\t\t%.6f\t\t%.6f\n", n, time_naive, time_opt);

       

        free(A); free(x); free(y1); free(y2);
    }

    return 0;
}

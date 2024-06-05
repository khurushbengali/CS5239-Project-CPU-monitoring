#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define rdpmc(ecx, eax, edx)    \
    asm volatile (              \
        "rdpmc"                 \
        : "=a"(eax),            \
          "=d"(edx)             \
        : "c"(ecx))

int32_t usage(void)
{
    printf("\t./mat_mul <N>\n");
    return -1;
}

void print_matrix(uint32_t N, long *m)
{
    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j)
            printf("%3ld ", m[i * N + j]);
        printf("\n");
    }
}

void transpose_matrix(uint32_t N, long *m)
{
    long *temp = malloc(N * N * sizeof(long));

    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j) {
            temp[i * N + j] = m[j * N + i];
        }
    }

    memcpy(m, temp, N * N * sizeof(long));
    free(temp);
}

int32_t main(int32_t argc, char *argv[])
{
    if (argc != 2)
        return usage();

    clock_t t;
    uint32_t N = atoi(argv[1]);
    int64_t *m1 = malloc(N * N * sizeof(int64_t));
    int64_t *m2 = malloc(N * N * sizeof(int64_t));
    int64_t *r = malloc(N * N * sizeof(int64_t));

    for (uint32_t i = 0; i < N * N; ++i) {
        m1[i] = i;
        m2[i] = i;
    }
    // Transpose one matrix
    transpose_matrix(N, m2);

    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    // Perform slow multiplication
    for (uint32_t i = 0; i < N; ++i)
        for (uint32_t j = 0; j < N; ++j)
            for (uint32_t k = 0; k < N; ++k)
                r[i * N + j] += m1[i * N + k] * m2[j * N + k];

    t = clock() - t;

    printf("Multiplication 1 finished in %6.2f s\n",
           ((float)t) / CLOCKS_PER_SEC);

    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    // Perform fast(er) multiplication
    for (uint32_t i = 0; i < N; ++i)
        for (uint32_t j = 0; j < N; ++j)
            for (uint32_t k = 0; k < N; ++k)
                r[i * N + j] += m1[i * N + k] * m2[j * N + k];

    t = clock() - t;

    printf("Multiplication 2 finished in %6.2f s\n",
           ((float)t) / CLOCKS_PER_SEC);

    return 0;
}



#include <stdio.h>      /* printf                         */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <stdlib.h>     /* malloc, calloc, free, atoi     */
#include <string.h>     /* memset                         */
#include <stdint.h>     /* uint32_t, uint64_t             */
#include <immintrin.h>  /* AVX512 intrinsics               */

#define rdpmc(ecx, eax, edx)    \
    asm volatile (              \
        "rdpmc"                 \
        : "=a"(eax),            \
          "=d"(edx)             \
        : "c"(ecx))

/*
 *  usage - how to run the program
 *      @return: -1
 */
int32_t
usage(void)
{
    printf("\t./mat_mul <N>\n");
    return -1;
}

/*
 *  print_matrix - if you need convincing that it works just fine
 *      @N: square matrix size
 *      @m: pointer to matrix
 */
void
print_matrix(uint32_t N, long *m)
{
    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j)
            printf("%3ld ", m[i * N + j]);
        printf("\n");
    }
}

/*
 *  AVX512 matrix multiplication - slow method
 */
void
avx512_slow_matrix_multiply(uint32_t N, int64_t *m1, int64_t *m2, int64_t *r)
{
    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j) {
            __m512i result = _mm512_setzero_si512();
            for (uint32_t k = 0; k < N; k += 8) {
                __m512i a = _mm512_loadu_si512((__m512i *)&m1[i * N + k]);
                __m512i b = _mm512_loadu_si512((__m512i *)&m2[k * N + j]);
                result = _mm512_add_epi64(result, _mm512_mullo_epi64(a, b));
            }
            int64_t sum[8];
            _mm512_storeu_si512((__m512i *)sum, result);
            r[i * N + j] = sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7];
        }
    }
}

/*
 *  AVX512 matrix multiplication - fast(er) method
 */
void
avx512_faster_matrix_multiply(uint32_t N, int64_t *m1, int64_t *m2, int64_t *r)
{
    for (uint32_t k = 0; k < N; ++k) {
        for (uint32_t i = 0; i < N; ++i) {
            __m512i a = _mm512_set1_epi64(m1[i * N + k]);
            for (uint32_t j = 0; j < N; j += 8) {
                __m512i b = _mm512_loadu_si512((__m512i *)&m2[k * N + j]);
                __m512i result = _mm512_loadu_si512((__m512i *)&r[i * N + j]);
                result = _mm512_add_epi64(result, _mm512_mullo_epi64(a, b));
                _mm512_storeu_si512((__m512i *)&r[i * N + j], result);
            }
        }
    }
}

int32_t
main(int32_t argc, char *argv[])
{
    if (argc != 2)
        return usage();

    /* allocate space for matrices */
    clock_t t;
    uint32_t N = atoi(argv[1]);
    int64_t *m1 = malloc(N * N * sizeof(int64_t));
    int64_t *m2 = malloc(N * N * sizeof(int64_t));
    int64_t *r = malloc(N * N * sizeof(int64_t));

    /* initialize matrices */
    for (uint32_t i = 0; i < N * N; ++i) {
        m1[i] = i;
        m2[i] = i;
    }

    /* Result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    avx512_slow_matrix_multiply(N, m1, m2, r);

    /* Clock delta */
    t = clock() - t;

    printf("AVX512 Slow Multiplication finished in %6.2f s\n", ((float)t) / CLOCKS_PER_SEC);

    /* Result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    avx512_faster_matrix_multiply(N, m1, m2, r);
    
    /* Clock delta */
    t = clock() - t;

    printf("AVX512 Faster Multiplication finished in %6.2f s\n", ((float)t) / CLOCKS_PER_SEC);

    return 0;
}


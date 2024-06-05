#include <stdio.h>      /* printf                         */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <stdlib.h>     /* malloc, calloc, free, atoi     */
#include <string.h>     /* memset                         */
#include <stdint.h>     /* uint32_t, uint64_t             */
#include <immintrin.h>  /* AVX2 intrinsics                */

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
 *  AVX2 matrix multiplication - slow method
 */
void
avx2_slow_matrix_multiply(uint32_t N, int64_t *m1, int64_t *m2, int64_t *r)
{
    for (uint32_t i = 0; i < N; ++i) {
        for (uint32_t j = 0; j < N; ++j) {
            __m256i result = _mm256_setzero_si256();
            for (uint32_t k = 0; k < N; k += 4) {
                __m256i a = _mm256_loadu_si256((__m256i *)&m1[i * N + k]);
                __m256i b = _mm256_loadu_si256((__m256i *)&m2[k * N + j]);
                result = _mm256_add_epi64(result, _mm256_mul_epu32(a, b));
            }
            int64_t sum[4];
            _mm256_storeu_si256((__m256i *)sum, result);
            r[i * N + j] = sum[0] + sum[1] + sum[2] + sum[3];
        }
    }
}

/*
 *  AVX2 matrix multiplication - fast(er) method
 */
void
avx2_faster_matrix_multiply(uint32_t N, int64_t *m1, int64_t *m2, int64_t *r)
{
    for (uint32_t k = 0; k < N; ++k) {
        for (uint32_t i = 0; i < N; ++i) {
            __m256i a = _mm256_set1_epi64x(m1[i * N + k]);
            for (uint32_t j = 0; j < N; j += 4) {
                __m256i b = _mm256_loadu_si256((__m256i *)&m2[k * N + j]);
                __m256i result = _mm256_loadu_si256((__m256i *)&r[i * N + j]);
                result = _mm256_add_epi64(result, _mm256_mul_epu32(a, b));
                _mm256_storeu_si256((__m256i *)&r[i * N + j], result);
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

    avx2_slow_matrix_multiply(N, m1, m2, r);

    /* Clock delta */
    t = clock() - t;

    printf("AVX2 Slow Multiplication finished in %6.2f s\n", ((float)t) / CLOCKS_PER_SEC);

    /* Result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    avx2_faster_matrix_multiply(N, m1, m2, r);

    /* Clock delta */
    t = clock() - t;

    printf("AVX2 Faster Multiplication finished in %6.2f s\n", ((float)t) / CLOCKS_PER_SEC);

    return 0;
}

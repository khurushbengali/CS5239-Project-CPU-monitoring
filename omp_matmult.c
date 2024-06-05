#include <stdio.h>      /* printf                         */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <stdlib.h>     /* malloc, calloc, free, atoi     */
#include <string.h>     /* memset                         */
#include <stdint.h>     /* uint32_t, uint64_t             */
#include <omp.h>        /* #pragma openmp parrallel       */

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
    for (uint32_t i=0; i<N; ++i) {
        for (uint32_t j=0; j<N; ++j)
            printf("%3ld ", m[i*N + j]);
        printf("\n");
    }
}

/*
 *  main - program entry point
 *      @argc: number of arguments & program name
 *      @argv: arguments
 */
int32_t
main(int32_t argc, char *argv[])
{
    if (argc != 2)
        return usage();

    /* allocate space for matrices */
    clock_t t;
    uint32_t N   = atoi(argv[1]);
    int64_t  *m1 = malloc(N * N * sizeof(int64_t));
    int64_t  *m2 = malloc(N * N * sizeof(int64_t));
    int64_t  *r  = malloc(N * N * sizeof(int64_t));

    /* initialize matrices */
    for (uint32_t i=0; i<N*N; ++i) {
        m1[i] = i;
        m2[i] = i;
    }

    // STANDARD MULTIPLICATION

    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */

    /* perform slow multiplication */
    for (uint32_t i=0; i<N; ++i)             /* line   */
        for (uint32_t j=0; j<N; ++j)         /* column */
            for (uint32_t k=0; k<N; ++k)
                r[i*N + j] += m1[i*N + k] * m2[k*N + j];

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 1 finished in %6.2f s\n",
           ((float)t)/CLOCKS_PER_SEC);


    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */

    /* perform fast(er) multiplication */
    for (uint32_t k=0; k<N; ++k)
        for (uint32_t i=0; i<N; ++i)         /* line   */
            for (uint32_t j=0; j<N; ++j)     /* column */
                r[i*N + j] += m1[i*N + k] * m2[k*N + j];

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 2 finished in %6.2f s\n",
           ((float)t)/CLOCKS_PER_SEC); 


    #define TILE_SIZE 128       // Adjust as needed
    #define THREAD_COUNT 8      // CPU THREADS USED
    omp_set_num_threads(THREAD_COUNT);
    uint32_t i,j,k,ii,jj,kk;
    #define MIN(a,b) (((a)<(b))?(a):(b))

    
    // PARALLELISM USING OPENMP 
    

    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */

    
    /* perform slow multiplication */
    #pragma omp parallel for private(i, j, k) collapse(3)
    for (i=0; i<N; ++i)             /* line   */
        for (j=0; j<N; ++j)         /* column */
            for (k=0; k<N; ++k)
                r[i*N + j] += m1[i*N + k] * m2[k*N + j];

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 1 (Parallel) finished in %6.6f s\n",
           ((float)t)/CLOCKS_PER_SEC);


    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */

    /* perform fast(er) multiplication */
    #pragma omp parallel for private(i, j, k) collapse(3)
    for (k=0; k<N; ++k)
        for (i=0; i<N; ++i)         /* line   */
            for (j=0; j<N; ++j)     /* column */
                r[i*N + j] += m1[i*N + k] * m2[k*N + j];

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 2 (Parallel) finished in %6.6f s\n",
           ((float)t)/CLOCKS_PER_SEC); 


    
    // PARALLELISM USING OPENMP (Tiling)


    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */

    
    /* perform slow(er) multiplication */
    omp_set_num_threads(THREAD_COUNT);
    #pragma omp parallel for private(ii,jj,kk)
    for (ii = 0; ii < N; ii += TILE_SIZE)
        for (jj = 0; jj < N; jj += TILE_SIZE)
            for (kk = 0; kk < N; kk += TILE_SIZE) {
                #pragma omp parallel for private(i,j,k)
                for (i=ii; i<MIN(ii+TILE_SIZE,N); ++i)             /* line   */
                    for (j=jj; j<MIN(jj+TILE_SIZE,N); ++j)         /* column */
                        for (k=kk; k<MIN(kk+TILE_SIZE,N); ++k)
                            r[i*N + j] += m1[i*N + k] * m2[k*N + j];
            }

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 1 (Parallel with tiling) finished in %6.6f s\n",
           ((float)t)/CLOCKS_PER_SEC);


    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */

    /* perform fast(er) multiplication */
    omp_set_num_threads(THREAD_COUNT);
    #pragma omp parallel for private(ii,jj,kk)
    for (kk = 0; kk < N; kk += TILE_SIZE)
        for (ii = 0; ii < N; ii += TILE_SIZE)
            for (jj = 0; jj < N; jj += TILE_SIZE) {
                #pragma omp parallel for private(i,j,k) collapse(3)
                for (k=kk; k<MIN(kk+TILE_SIZE,N); ++k)
                    for (i=ii; i<MIN(ii+TILE_SIZE,N); ++i)         /* line   */
                        for (j=jj; j<MIN(jj+TILE_SIZE,N); ++j)     /* column */
                            r[i*N + j] += m1[i*N + k] * m2[k*N + j];
            }

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 2 (Parallel with tiling) finished in %6.6f s\n",
           ((float)t)/CLOCKS_PER_SEC); 

    return 0;
}
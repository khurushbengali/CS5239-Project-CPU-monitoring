/*
    CPUID -r --> 0x0000000a 0x00: 
                    eax=0x07300404 
                    ebx=0x00000000 
                    ecx=0x00000000 
                    edx=0x00000603


*/

#include <stdio.h>      /* printf                         */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <stdlib.h>     /* malloc, calloc, free, atoi     */
#include <string.h>     /* memset                         */
#include <stdint.h>     /* uint32_t, uint64_t             */
#include <inttypes.h>

/*
 *  usage - how to run the program
 *      @return: -1
 */
int32_t
usage()
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

void read_l2_cache_misses() {
    // Define the command to be executed
    const char *command = "rdmsr 0xc1";
    system(command);
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

    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();

    /* TODO: count L2 cache misses for the next block using RDPMC */
    printf("Reading L2 cache misses before first multiplication.\n");
    read_l2_cache_misses();

    /* perform slow multiplication */
    for (uint32_t i=0; i<N; ++i)             /* line   */
        for (uint32_t j=0; j<N; ++j)         /* column */
            for (uint32_t k=0; k<N; ++k)
                r[i*N + j] += m1[i*N + k] * m2[k*N + j];
    
    printf("Reading L2 cache misses after first multiplication.\n");
    read_l2_cache_misses();

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 1 finished in %6.2f s\n",
           ((float)t)/CLOCKS_PER_SEC);


    /* result matrix clear; clock init */
    memset(r, 0, N * N * sizeof(int64_t));
    t = clock();


    /* TODO: count L2 cache misses for the next block using RDPMC */
    printf("Reading L2 cache misses before second multiplication.\n");
    read_l2_cache_misses();

    /* perform fast(er) multiplication */
    for (uint32_t k=0; k<N; ++k)
        for (uint32_t i=0; i<N; ++i)         /* line   */
            for (uint32_t j=0; j<N; ++j)     /* column */
                r[i*N + j] += m1[i*N + k] * m2[k*N + j];

    printf("Reading L2 cache misses after second multiplication.\n");
    read_l2_cache_misses();

    /* clock delta */
    t = clock() - t;

    printf("Multiplication 2 finished in %6.2f s\n",
           ((float)t)/CLOCKS_PER_SEC); 

    return 0;
}
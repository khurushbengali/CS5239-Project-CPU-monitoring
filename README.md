# Team 10 project on Topic 1: CPU Monitoring



## mpstat
### Task A
- **fact_rcrs_1.py:** This Python script calculates and prints the factorial of numbers up to a specified limit. It demonstrates recursion and adjusts the recursion limit as needed. To use the script, provide the desired limit as a command-line argument.
### Task B
- **fact_rcrs_2.py:** This Python script calculates and prints the factorial of numbers up to a specified limit, overcoming standard integer representation limits. It adjusts the representation size for large factorials and showcases iterative factorial computation.
- **task4_2_B.sh:** This Bash script optimizes CPU affinity for a Python script execution. It utilizes the stress tool to occupy all but one CPU core, leaving one core exclusively for running the specified Python script. This configuration aims to enhance script performance by minimizing CPU contention. 
### Task C
- **task4_2_C.sh:** This Bash script utilizes the stress tool to create stress workers on odd-numbered CPU cores. The user can adjust the number of workers and stress duration based on performance testing requirements. This script is beneficial for simulating a CPU-intensive load on specific cores, enabling performance testing and analysis on a subset of the available CPU resources.
## Zip with Compression levels
## Hardware Counters
### Task E - Ring3 cache performance evaluation
- **mat_mul_02_sys.c:** Modified C script for LLC misses reading. The script uses 'system(cmd)' alternatively to the 'asm' instructions.  
### Task F - Optimizing matrix multiplication
#### C implementation
- **naive_matmul.c:** This C script performs naive matrix multiplication for a square matrix of size N. It allocates memory for matrices, initializes them, and then performs both slow and optimized multiplication methods. 
- **transpose_matmul.c:** This C script performs matrix multiplication for a square matrix of size N, comparing both the standard and optimized methods. It transposes one matrix before multiplication, providing insights into the impact of matrix layout on performance. 
- **avx2_matmult.c:** This C script demonstrates AVX2-accelerated matrix multiplication, comparing a slower method with a faster variant. The AVX2 instructions enhance the performance of matrix operations, and the script measures and prints the execution time for each method, providing insights into the efficiency of AVX2 optimization in matrix multiplication.
- **avx512_matmul.c:** This C script showcases AVX512-accelerated matrix multiplication, featuring both a slower and a faster method.
- **omp_matmul.c:** This C script showcases OpenMP Thread Parallelism (with and without tiling) on matrix multiplacation for both slow and fast methods.
#### Python implementation
- **pythonmul_naive.py:** This Python script benchmarks both a slow and a fast matrix multiplication implementation, measuring the execution time for each method. It takes a matrix size (N) as a command-line argument and showcases the performance difference between the two multiplication techniques.
- **pythonmul_numpy.py:** This Python script demonstrates optimized matrix multiplication using NumPy. The result showcases the efficiency of NumPy in handling matrix operations.
- **pythonmul_parallel.py:** This Python script utilizes Numba to parallelize matrix multiplication. 
- **pythonmul_numpyparallel.py:** NumPy supports vectorized operations, and we leveraged this to further optimize the code. In this optimized version, the innermost loop has been replaced with a NumPy operation (np.sum) to take advantage of vectorization. 

import time
import sys
import numpy as np
from numba import njit, prange

def usage():
    print("\t./mat_mul <N>")
    return -1

@njit(parallel=True)
def multiply_matrices_parallel(N, m1, m2, r):
    for i in prange(N):
        for j in prange(N):
            for k in range(N):
                r[i, j] += m1[i, k] * m2[k, j]

def main():
    if len(sys.argv) != 2:
        return usage()

    N = int(sys.argv[1])

    # Allocate space for matrices
    m1 = np.arange(N * N, dtype=np.int64).reshape(N, N)
    m2 = np.arange(N * N, dtype=np.int64).reshape(N, N)
    r = np.zeros((N, N), dtype=np.int64)

    # Initialize matrices
    m1[:, :] = np.arange(N * N, dtype=np.int64).reshape(N, N)
    m2[:, :] = np.arange(N * N, dtype=np.int64).reshape(N, N)

    # Time measurement for optimized parallel multiplication
    t_start = time.time()

    # Perform optimized parallel matrix multiplication using numba
    multiply_matrices_parallel(N, m1, m2, r)

    # Time delta
    t_optimized_parallel = time.time() - t_start

    print(f"Optimized parallel multiplication finished in {t_optimized_parallel:.2f} s")

if __name__ == "__main__":
    main()


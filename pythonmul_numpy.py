import time
import sys
import numpy as np

def usage():
    print("\t./mat_mul <N>")
    return -1

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

    # Time measurement for optimized multiplication
    t_start = time.time()

    # Perform optimized matrix multiplication using NumPy
    r = np.dot(m1, m2)

    # Time delta
    t_optimized = time.time() - t_start

    print(f"Optimized multiplication finished in {t_optimized:.2f} s")

if __name__ == "__main__":
    main()


import sys
import time

def usage():
    print("\t./mat_mul.py <N>")
    return -1

def matrix_multiply_slow(N, m1, m2):
    result = [[0] * N for _ in range(N)]
    for i in range(N):
        for j in range(N):
            for k in range(N):
                result[i][j] += m1[i][k] * m2[k][j]
    return result

def matrix_multiply_fast(N, m1, m2):
    result = [[0] * N for _ in range(N)]
    for k in range(N):
        for i in range(N):
            for j in range(N):
                result[i][j] += m1[i][k] * m2[k][j]
    return result

def main():
    if len(sys.argv) != 2:
        return usage()

    N = int(sys.argv[1])

    # Allocate space for matrices and initialize them
    m1 = [[i for i in range(N)] for _ in range(N)]
    m2 = [[i for i in range(N)] for _ in range(N)]

    # Measure time for the slow multiplication
    start_time = time.time()

    # Perform slow matrix multiplication
    result_slow = matrix_multiply_slow(N, m1, m2)

    end_time = time.time()
    elapsed_time_slow = end_time - start_time

    print("Multiplication (slow) finished in {:.2f} s".format(elapsed_time_slow))

    # Measure time for the fast multiplication
    start_time = time.time()

    # Perform fast matrix multiplication
    result_fast = matrix_multiply_fast(N, m1, m2)

    end_time = time.time()
    elapsed_time_fast = end_time - start_time

    print("Multiplication (fast) finished in {:.2f} s".format(elapsed_time_fast))

if __name__ == "__main__":
    main()


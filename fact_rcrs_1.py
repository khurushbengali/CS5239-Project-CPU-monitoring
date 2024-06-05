#!/usr/bin/python3

import sys

# Get the current recursion limit
recursion_limit = sys.getrecursionlimit()

# Print the current recursion limit
print("Current recursion limit:", recursion_limit)

sys.setrecursionlimit(10000)

def factorial(n):
    return 1 if n == 0 \
           else n * factorial(n - 1)

if __name__ == '__main__':
    big_number = int(sys.argv[1])

    for i in range(1, big_number + 1):
        result = factorial(i)
        print('factorial(%d) = %d' % (i, result))
        factorial(i)



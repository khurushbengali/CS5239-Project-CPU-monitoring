#!/usr/bin/python3

import sys

# Get the current recursion limit
recursion_limit = sys.getrecursionlimit()

# Print the current recursion limit
print("Current recursion limit:", recursion_limit)

sys.set_int_max_str_digits(50000)

def factorial(n):
    result = 1
    for i in range(1, n + 1):
        result *= i
    return result

if __name__ == '__main__':
    big_number = int(sys.argv[1])

    for i in range(1, big_number + 1):
        result = factorial(i)
        print('factorial(%d) = %d' % (i, result))
        factorial(i)

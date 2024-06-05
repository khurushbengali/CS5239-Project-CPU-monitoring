#!/bin/bash

# Get the number of CPU cores on your system
NUM_CORES=$(nproc)

# Calculate N-1
NUM_WORKERS=$((NUM_CORES - 1))

# Set CPU affinity for N-1 workers to CPUs 1-(N-1)
for ((i = 1; i < NUM_CORES; i++)); do
          taskset -c $i stress -c 1 -q &
done

# Let the workers settle (adjust the delay as needed)
sleep 5

# Run your Python script on CPU 0
taskset -c 0 python3 try.py 10000

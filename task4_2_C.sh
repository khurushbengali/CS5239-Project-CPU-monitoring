num_cores=$(nproc)
odd_cores=()
num_workers=2  # You can set the desired number of stress workers

# Generate a list of odd-numbered cores
for ((core = 1; core <= num_cores; core += 2)); do
        odd_cores+=("$core")
done

# Start stress workers on the odd-numbered cores
for core in "${odd_cores[@]}"; do
        taskset -c "$core" stress -c 1 -t 10s &  # Adjust stress command as needed
done

# Wait for the workers to complete
wait

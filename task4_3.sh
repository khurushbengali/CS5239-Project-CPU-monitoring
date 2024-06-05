#!/bin/bash

# Define the input files
input_files=("big.txt" "MARBLES.BMP" "RAY.BMP")

# Define the output directory for compressed files
output_dir="compressed_files"

# Create the output directory if it doesn't exist
mkdir -p "$output_dir"
> compression_results.txt

# Loop through each input file
for input_file in "${input_files[@]}"
do
    # Loop through compression levels 0 to 9
    for level in {0..9}
    do
        # Define the output file for the current compression level and input file
        output_file="$output_dir/${input_file}_level${level}.zip"

        # Measure the time and compression rate using the time command
        { time zip -${level} "$output_file" "$input_file"; } 2>&1 | \
        grep real | \
        awk -v level=$level -v input_file="$input_file" '{print "\n Compression Level " level " for " input_file ": Time: " $2}' | \
        tee -a compression_results.txt
        unzip -v ${output_dir}/${input_file}_level${level}.zip 2>&1 | tee -a compression_results.txt
    done
done

# Loop through compression levels 0 to 9
for level in {0..9}
do
    # Define the output file for the current compression level and input file
    output_file="$output_dir/compressed_zip_level${level}.zip"
    
    # Measure the time and compression rate using the time command
    { time zip -${level} "$output_file" "${input_files[@]}"; } 2>&1 | \
    grep real | \
    awk -v level=$level -v input_file="$input_file" '{print "\n Compression Level " level " for all three files: Time: " $2}' | \
    tee -a compression_results.txt
    unzip -v ${output_dir}/compressed_zip_level${level}.zip 2>&1 | tee -a compression_results.txt
done

# Clean up temporary files
rm -rf "$output_dir"

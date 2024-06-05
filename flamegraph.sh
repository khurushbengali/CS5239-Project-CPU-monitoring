sudo perf record -F 99 -g ./matmulnaive 1024 
sudo perf script | ./FlameGraph/stackcollapse-perf.pl | ./FlameGraph/flamegraph.pl > flamegraph.svg 

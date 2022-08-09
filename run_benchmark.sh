GOVERNOR=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)
sudo cpupower frequency-set -g performance
./build/bin/best_bid_ask_benchmark
#sudo perf record --call-graph dwarf ./build/bin/best_bid_ask_benchmark
sudo cpupower frequency-set -g $GOVERNOR

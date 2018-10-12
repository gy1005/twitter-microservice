#!/usr/bin/env bash

mkdir -p ../logs

for run in {1..5}; do
  mkdir -p ../logs/run_$run
  for qps in 1800; do    
    ./setup.sh
    sleep 3
    taskset -c 0,2,4,6,8,10,12,14,16,18,20 ../build/Client -t 50 -d 60 -q $qps > ../logs/run_$run/qps_$qps.log
    ./clean_up.sh
  done
done
# High-Performance Cache (LRU & LFU) - C++17

## Overview
Implements two in-memory eviction caches:
- LRU (Least Recently Used)
- LFU (Least Frequently Used)

Includes a micro-benchmark harness to measure ops/sec under mixed read/write workload.

## Build
make

## Run
./cache_demo

## Notes
- Use -O3 -march=native for best performance.
- For concurrency: implement sharding and finer-grained locks.

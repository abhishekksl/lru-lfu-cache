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

## Micro-benchmark (measured on my machine)

- Mixed read/write workload (single-threaded)
- LRU ≈ **22.8M ops/sec** (measured)
- LFU ≈ **8.0M ops/sec** (measured)

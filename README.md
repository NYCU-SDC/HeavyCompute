# HeavyCompute

A simple benchmark program to compare CPU and GPU performance on intensive computation workloads using HIP.

This program performs a repeated arithmetic operation on a large array of integers.  
It provides a direct comparison between CPU execution and GPU kernel execution.

## File Structure

```
.
├── HeavyCompute.cpp  # Main program source code
├── Makefile          # Build configuration
└── README.md         # Documentation
```

## Build

Make sure you have ROCm and HIP environment properly installed.

```bash
make
```

This will generate the executable:

```
HeavyCompute
```

## Run

Execute the program:

```bash
./HeavyCompute
```

Output example:

```
GPU kernel execution time: 16.637 ms
CPU execution time: 2241.86 ms
Output comparison: PASSED
```

## Parameters

The following parameters are defined inside `HeavyCompute.cpp` and can be modified for different workloads:

| Variable   | Description                        | Default Value |
|------------|------------------------------------|---------------|
| `N`        | Number of elements                 | 10,000,000    |
| `iterations` | Number of computations per element | 100           |

## Purpose

This project is intended for:

- Performance benchmarking of CPU vs GPU
- HIP programming practice
- Testing hardware compute capability under heavy arithmetic workloads

---

## Environment

Tested on:

- ROCm version: 6.3.3
- HIP runtime
- Linux x86_64

---

## License

MIT License
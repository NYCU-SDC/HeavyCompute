#include <hip/hip_runtime.h>
#include <iostream>
#include <chrono>
#include <cstdlib>

#define SUCCESS 0
#define FAILURE 1

using namespace std;

// GPU Kernel: Perform multiple computations on each element to increase workload
__global__ void heavyKernel(const int* in, int* out, int iterations, size_t N) {
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id < N) {
        int value = in[id];
        // Simulate intensive computation: loop 100 times for each element
        for (int i = 0; i < iterations; i++){
            // Example computation: multiply by 2, add current loop index, then modulo to simulate heavy work
            value = (value * 2 + i) % 1000000;
        }
        out[id] = value;
    }
}

// CPU version: Same computation logic as GPU
void cpuHeavy(const int* in, int* out, int iterations, size_t N) {
    for (size_t id = 0; id < N; ++id) {
        int value = in[id];
        for (int i = 0; i < iterations; i++){
            value = (value * 2 + i) % 1000000;
        }
        out[id] = value;
    }
}

int main() {
    // Define the amount of data and the number of computation iterations
    size_t N = 10000000; // Default 10 million elements, you can change this number to test the difference of CPU and GPU computation.
    int iterations = 100; // Perform 100 computations per element

    // Allocate and initialize host arrays
    int* h_in = new int[N];
    int* h_out_gpu = new int[N];
    int* h_out_cpu = new int[N];
    for (size_t i = 0; i < N; i++){
        h_in[i] = i % 100; // Simple data initialization
    }

    // Allocate device memory
    int *d_in, *d_out;
    hipMalloc(&d_in, N * sizeof(int));
    hipMalloc(&d_out, N * sizeof(int));

    // Copy input data from host to device
    hipMemcpy(d_in, h_in, N * sizeof(int), hipMemcpyHostToDevice);

    // --- GPU Timing (using HIP Event) ---
    hipEvent_t start, stop;
    hipEventCreate(&start);
    hipEventCreate(&stop);

    int threadsPerBlock = 256;
    int blocks = (N + threadsPerBlock - 1) / threadsPerBlock;

    hipEventRecord(start, 0);
    hipLaunchKernelGGL(heavyKernel,
                       dim3(blocks),
                       dim3(threadsPerBlock),
                       0, 0,
                       d_in, d_out, iterations, N);
    hipEventRecord(stop, 0);
    hipEventSynchronize(stop);
    float gpuTimeMs;
    hipEventElapsedTime(&gpuTimeMs, start, stop);
    cout << "GPU kernel execution time: " << gpuTimeMs << " ms" << endl;

    // Copy GPU computation results back to host
    hipMemcpy(h_out_gpu, d_out, N * sizeof(int), hipMemcpyDeviceToHost);

    // Free GPU resources
    hipEventDestroy(start);
    hipEventDestroy(stop);
    hipFree(d_in);
    hipFree(d_out);

    // --- CPU Timing (using std::chrono) ---
    auto cpuStart = chrono::high_resolution_clock::now();
    cpuHeavy(h_in, h_out_cpu, iterations, N);
    auto cpuEnd = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> cpuTime = cpuEnd - cpuStart;
    cout << "CPU execution time: " << cpuTime.count() << " ms" << endl;

    // Verify that GPU and CPU results are consistent
    bool correct = true;
    for (size_t i = 0; i < N; i++){
        if (h_out_gpu[i] != h_out_cpu[i]){
            correct = false;
            break;
        }
    }
    cout << "Output comparison: " << (correct ? "PASSED" : "FAILED") << endl;

    // Free host resources
    delete[] h_in;
    delete[] h_out_gpu;
    delete[] h_out_cpu;

    return SUCCESS;
}

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <math.h>

#define N 1024 * 220


__device__ float f(float x) 
{
    return sinf(x)*x+cosf(x*x)*x*x;
}

__global__ void kernel(float* data)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    float stepbythread = 100.f / (1024.f * 220.f); 
    float begin = idx * stepbythread;
    float end = begin + stepbythread;

    float x = begin;
    float cur_min = f(x);

    x += 0.0001f;
    while (x <= end)
    {
        float cur_f = f(x);

        if (cur_f < cur_min)
            cur_min = cur_f;

        x += 0.0001f;
    }

    data[idx] = cur_min;

    if (idx % 512 == 0)
    {
        float this_min = data[idx];
        for (int i = idx; i < idx + 512; i++)
        {
            if (data[idx] > data[i])
                data[idx] = data[i];
        }
    }
}

int main()
{
    int deviceCount;
    cudaDeviceProp devProp;
    cudaGetDeviceCount(&deviceCount);
    printf("------------------------------------------\n");
    printf("Found %d devices\n", deviceCount);
    for (int device = 0; device < deviceCount; device++)
    {
        cudaGetDeviceProperties(&devProp, device);
        printf("Device                  : %d\n", device);
        printf("Compute copability      : %d.%d\n", devProp.major, devProp.minor);
        printf("Name                    : %s\n", devProp.name);
        printf("Total Global Memory     : %d\n", devProp.totalGlobalMem);
        printf("Shared memory per block : %d\n", devProp.sharedMemPerBlock);
        printf("Register per block      : %d\n", devProp.regsPerBlock);
        printf("Warp size               : %d\n", devProp.warpSize);
        printf("Shared memory per block : %d\n", devProp.sharedMemPerBlock);
        printf("Max pthreads per block  : %d\n", devProp.maxThreadsPerBlock);
        printf("Total constant memory   : %d\n", devProp.totalConstMem);
    }
    printf("------------------------------------------\n\n");

    float z[N];
    float* dev = NULL;

    if (cudaMalloc((void**)&dev, (N * sizeof(float))))
        printf("cudaMalloc error\n");

    kernel<<<dim3((N / 512), 1), dim3(512, 1)>>>(dev);

    if (cudaMemcpy(z, dev, (N * sizeof(float)), cudaMemcpyDeviceToHost))
        printf("cudaMemcpy error\n");

    if (cudaFree(dev))
        printf("cudaFree error");

    int count = 0;
    float min = z[0];
    for (int idx = 0; idx < N; idx += 512) 
    {
        //printf("a[%d] = %.5f\n", idx, z[idx]);

        if (z[idx] < z[0])
            min = z[idx];
        count++;
    }

    printf("count of blocks             : %d\n", N/512);
    printf("count of threads per block  : %d\n", 512);
    printf("%d out of %d comparisons were made on the processor\n%d out of %d comparisons were made on the graphics card\n", count, N, N-count, N);
    printf("f = sin(x) * x + cos(x * x) * x * x\n");
    printf("min(f) = %.5f\n", min);

    return 0;
}

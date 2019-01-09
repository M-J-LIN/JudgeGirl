#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include <inttypes.h>
#define MAXGPU 6
#define MAXN (67108864>>17)
int main(){
    cl_device_id GPU[MAXGPU];
    cl_context context;
    cl_command_queue commandQueue;
    cl_program program;
    cl_kernel kernel;
    cl_int status;
 
    cl_platform_id platforms;
    cl_uint num_platform;
    status = clGetPlatformIDs(1, &platforms, &num_platform);
    assert ( status == CL_SUCCESS );
 
    cl_uint num_GPU;
    status = clGetDeviceIDs(platforms, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &num_GPU);
    assert ( status == CL_SUCCESS );
 
    context = clCreateContext(NULL, 1, GPU, NULL, NULL, &status);
    assert ( status == CL_SUCCESS );
    commandQueue = clCreateCommandQueue(context, GPU[0], 0, &status);
    assert ( status == CL_SUCCESS );
    FILE *fp = fopen("vecdot.cl", "r");
    char kernelBuf[4096];
    const char *strings = kernelBuf;
    size_t length = fread(kernelBuf, 1, 4096, fp);
    program = clCreateProgramWithSource(context, 1, &strings, &length, &status);
    assert ( status == CL_SUCCESS );
 
    status = clBuildProgram(program, 1, GPU, NULL, NULL, NULL);
    assert ( status == CL_SUCCESS );
    if(status != CL_SUCCESS){
        char prog_log[4096];
        size_t logLength;
        clGetProgramBuildInfo(program, GPU[0], CL_PROGRAM_BUILD_LOG, 4096, prog_log, &logLength);
        printf("%s", prog_log);
        return 0;
    }
 
    kernel = clCreateKernel(program, "dotProduct", &status);
    assert ( status == CL_SUCCESS );
    int N;
    uint32_t key1, key2;
    uint32_t* C = (uint32_t*)malloc(MAXN * sizeof(uint32_t));
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, MAXN * sizeof(uint32_t), NULL, &status);
    assert ( status == CL_SUCCESS );
 
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
        memset(C, 0, MAXN*sizeof(uint32_t));
 
        status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufferC);
        status = clSetKernelArg(kernel, 1, sizeof(cl_int), (void*)&N);
        status = clSetKernelArg(kernel, 2, sizeof(cl_uint), (void*)&key1);
        status = clSetKernelArg(kernel, 3, sizeof(cl_uint), (void*)&key2);
        int chunk = 131072;
        int local = 256;
        N = ((N+chunk-1)/chunk)*local;
        size_t globalThreads [] = {( size_t ) N};
        size_t localThreads [] = {local};
 
        status = clEnqueueNDRangeKernel ( commandQueue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL , NULL );
        assert ( status == CL_SUCCESS );
        clEnqueueReadBuffer ( commandQueue , bufferC , CL_TRUE ,0 , MAXN*sizeof (uint32_t), C , 0 , NULL , NULL );
        uint32_t sum = 0;
        int r = N/local;
        for (int i = 0; i < r; i++) {
            sum += C[i];
        }
        printf("%" PRIu32 "\n", sum);
    }
    free(C);
    clReleaseMemObject(bufferC);
    clReleaseContext ( context );
    clReleaseCommandQueue ( commandQueue );
    clReleaseProgram ( program );
    clReleaseKernel ( kernel );
 
    return 0;
}
/*
16777216 1 2
16777216 3 5

2885681152
2147483648
*/

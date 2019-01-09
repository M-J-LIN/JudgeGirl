#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <string.h>
#include <assert.h>
#include <omp.h> 
#include <inttypes.h>
#define MAXGPU 6
#define MAXN (1073741824>>17)
void launch(int, int, uint32_t*);

int N;
uint32_t key1, key2;
uint32_t *C[3];
cl_mem bufferC[3];
uint32_t testcase[10000][3];
uint32_t result[10000];
cl_command_queue commandQueue[3];
cl_program program[3];
cl_kernel kernel[3];
cl_int status;
int main(){
    cl_device_id GPU[MAXGPU];
    cl_context context[3];
 
    cl_platform_id platforms;
    cl_uint num_platform;
    status = clGetPlatformIDs(1, &platforms, &num_platform);
	assert ( status == CL_SUCCESS );
 
    cl_uint num_GPU;
    status = clGetDeviceIDs(platforms, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &num_GPU);
	assert ( status == CL_SUCCESS );
 	for(int i = 0; i < num_GPU; i++)
    	context[i] = clCreateContext(NULL, 1, &GPU[i], NULL, NULL, &status);
	assert ( status == CL_SUCCESS );
 	for(int i = 0; i < num_GPU; i++)
    	commandQueue[i] = clCreateCommandQueue(context[i], GPU[i], 0, &status);
	assert ( status == CL_SUCCESS );
	FILE *fp = fopen("vecdot.cl", "r");
	char kernelBuf[4096];
	const char *strings = kernelBuf;
	size_t length = fread(kernelBuf, 1, 4096, fp);
 	for(int i = 0; i < num_GPU; i++)
		program[i] = clCreateProgramWithSource(context[i], 1, &strings, &length, &status);
	assert ( status == CL_SUCCESS );
 
 	for(int i = 0; i < num_GPU; i++)
		status = clBuildProgram(program[i], 1, &GPU[i], NULL, NULL, NULL);
	assert ( status == CL_SUCCESS );
	/*if(status != CL_SUCCESS){
		char prog_log[4096];
		size_t logLength;
		clGetProgramBuildInfo(program, GPU[0], CL_PROGRAM_BUILD_LOG, 4096, prog_log, &logLength);
		printf("%s", prog_log);
		return 0;
	}*/
	
 	for(int i = 0; i < num_GPU; i++)
		kernel[i] = clCreateKernel(program[i], "dotProduct", &status);
	assert ( status == CL_SUCCESS );
 	for(int i = 0; i < num_GPU; i++){
		C[i] = (uint32_t*)malloc(MAXN * sizeof(uint32_t));
		memset(C[i], 0, MAXN*sizeof(uint32_t));
		bufferC[i] = clCreateBuffer(context[i], CL_MEM_WRITE_ONLY, MAXN * sizeof(uint32_t), NULL, &status);
	}
	assert ( status == CL_SUCCESS );
	int cnt = 0;
 	while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
		testcase[cnt][0] = N, testcase[cnt][1] = key1, testcase[cnt][2] = key2;
		cnt++;	
    }
	int ind = 0;
	#pragma omp parallel sections
	{	
		#pragma omp section
		{	
			while(ind < cnt){
				int tmp;
				#pragma omp critical
				tmp = ind++;
				launch(0, tmp, testcase[tmp]);
			}
		}
		#pragma omp section
		{
			while(ind < cnt){
				int tmp;
				#pragma omp critical
				tmp = ind++;
				launch(1, tmp, testcase[tmp]);
			}
		}
		#pragma omp section
		{
			while(ind < cnt){
				int tmp;
				#pragma omp critical
				tmp = ind++;
				launch(2, tmp, testcase[tmp]);
			}
		}
	}
	for(int i = 0; i < cnt; i++){
        printf("%" PRIu32 "\n", result[i]);
	}
 	for(int i = 0; i < num_GPU; i++){
		free(C[i]);
		clReleaseMemObject(bufferC[i]);
		clReleaseContext ( context[i] ); 
		clReleaseCommandQueue ( commandQueue[i] );
		clReleaseProgram ( program[i] );
		clReleaseKernel ( kernel[i] );
	}
 
    return 0;
}
void launch(int kerNum, int index, uint32_t* testcase){
		status = clSetKernelArg(kernel[kerNum], 0, sizeof(cl_mem), (void*)&bufferC[kerNum]);
		status = clSetKernelArg(kernel[kerNum], 1, sizeof(cl_int), (void*)&testcase[0]);
		status = clSetKernelArg(kernel[kerNum], 2, sizeof(cl_uint), (void*)&testcase[1]);
		status = clSetKernelArg(kernel[kerNum], 3, sizeof(cl_uint), (void*)&testcase[2]);
		int chunk = 131072;	
		int local = 256;
		int gN = ((testcase[0]+chunk-1)/chunk)*local;
		size_t globalThreads [] = {( size_t ) gN};
		size_t localThreads [] = {local};	

		status = clEnqueueNDRangeKernel ( commandQueue[kerNum], kernel[kerNum], 1, NULL, globalThreads, localThreads, 0, NULL , NULL );
		assert ( status == CL_SUCCESS );
		clEnqueueReadBuffer ( commandQueue[kerNum] , bufferC[kerNum] , CL_TRUE ,0 , MAXN*sizeof (uint32_t), C[kerNum] , 0 , NULL , NULL );
    	uint32_t sum = 0;
		int r = gN/local;
    	#pragma omp parallel for schedule(static) reduction(+: sum)
		for (int i = 0; i < r; i++) {
			sum += C[kerNum][i];
        }
		result[index] = sum;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define MAX 1048576
#define MAX_THREAD 4
typedef struct CELL{
    int x, y, v;
}CELL;
CELL A[MAX], B[MAX];
int preA[MAX], preB[MAX];
int min(int a, int b){return (a < b) ? a : b;}
static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}
void prefixSum(int prefix_sum[]){
        #pragma omp parallel for num_threads(MAX_THREAD)
        for (int i = 0; i < MAX_THREAD; i++) {
            int start = MAX - (MAX_THREAD - i) * MAX/MAX_THREAD, end = MAX - (MAX_THREAD - i - 1) * MAX/MAX_THREAD;
            uint32_t sum = 0;
            for (int j = start; j < end; j++) {
                sum += prefix_sum[j];
                prefix_sum[j] = sum;
            }
        }
        int partial_sum[MAX_THREAD];
        partial_sum[0] = prefix_sum[MAX - (MAX_THREAD-1) * MAX/MAX_THREAD - 1];
        for (int i = 1; i < MAX_THREAD; i++) {
            partial_sum[i] = prefix_sum[MAX - (MAX_THREAD - i - 1) * MAX/MAX_THREAD - 1] + partial_sum[i - 1];
        }
        #pragma omp parallel for num_threads(MAX_THREAD)
        for (int i = 1; i < MAX_THREAD; i++) {
            int start = MAX - (MAX_THREAD - i) * MAX/MAX_THREAD, end = MAX - (MAX_THREAD - i - 1) * MAX/MAX_THREAD;
            for (int j = start; j < end; j++) {
                prefix_sum[j] += partial_sum[i - 1];
            }
        }
}
int main() {
    int N, M, R, nA, nB;
    scanf("%d %d %d", &N, &M, &R);
    scanf("%d %d", &nA, &nB);
    for (int i = 0; i < nA; i++)
        scanf("%d %d %d", &A[i].x, &A[i].y, &A[i].v);
    for (int i = 0; i < nB; i++)
        scanf("%d %d %d", &B[i].x, &B[i].y, &B[i].v);
    //int *CV = (int *)malloc(sizeof(int)*R);
    int CV[R];
    memset(CV, 0, R*4);
#pragma omp parallel sections
{
    #pragma omp section
    {
    for(int i = 0; i < nB; i++)
        preB[B[i].x + 1]++;
    prefixSum(preB);
    }
    #pragma omp section
    {
    for(int i = 0; i < nA; i++)
        preA[A[i].x + 1]++;
    prefixSum(preA);
    }
}
    uint32_t hash = 0;
    #pragma omp parallel for reduction(+:hash) firstprivate(CV)
    for(int t = 0; t <= A[nA - 1].x; t++){
        for(int i = preA[t]; i < preA[t+1]; i++){
            for(int j = preB[A[i].y]; j < preB[A[i].y + 1]; j++)
                CV[B[j].y] += A[i].v * B[j].v;
        }
        for(int j = 0; j < R; j++){
            if(CV[j])
                hash += encrypt((t+1)*(j+1), CV[j]);}
        memset(CV, 0, R*4);
    }
    printf("%u\n", hash);
    return 0;
}
/*
4 4 4
4 7
 
1 0 5
1 1 8
2 2 3
3 1 6
 
0 2 1
0 3 3
1 1 5
1 2 2
2 0 3
2 1 5
3 1 2

13093438
*/

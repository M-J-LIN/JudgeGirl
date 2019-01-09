#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "utils.h"
#include <pthread.h>
 
#define MAXN 10000005
#define MAX_THREAD 4
uint32_t prefix_sum[MAXN];
uint32_t key;
int n, partial_sum[MAX_THREAD];
void *final(void *id){
    int tid = *((int *)id);
    int start = MAXN - (MAX_THREAD - tid) * MAXN/4, end = MAXN - (MAX_THREAD - tid - 1) * MAXN/4;
    for (int i = start; i < end; i++) {
        prefix_sum[i] += partial_sum[tid - 1];
    }
    pthread_exit(NULL);
}
void *partial(void *id){
    uint32_t sum = 0;
    int tid = *((int *)id);
    int start = MAXN - (MAX_THREAD - tid) * MAXN/4, end = MAXN - (MAX_THREAD - tid - 1) * MAXN/4;
    for (int i = start; i < end; i++) {
        sum += encrypt(i, key);
        prefix_sum[i] = sum;
    }
    pthread_exit(NULL);
 
}
int main() {
    pthread_t threads[MAX_THREAD];
    int threadIndex[MAX_THREAD];
    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        prefix_sum[0] = encrypt(0, key);
        for (int i = 0; i < MAX_THREAD; i++) {
            threadIndex[i] = i;
            pthread_create(&threads[i], NULL, partial, (void *)(threadIndex + i));
        }
        for (int i = 0; i < MAX_THREAD; i++) {
            pthread_join(threads[i], NULL);
        }
        partial_sum[0] = prefix_sum[MAXN - (MAX_THREAD-1) * MAXN/4 - 1];
        for (int i = 1; i < MAX_THREAD; i++) {
            partial_sum[i] = prefix_sum[MAXN - (MAX_THREAD - i - 1) * MAXN/4 - 1] + partial_sum[i - 1];
        }
        for (int i = 1; i < MAX_THREAD; i++) {
            threadIndex[i] = i;
            pthread_create(&threads[i], NULL, final, (void *)(threadIndex + i));
        }
        for (int i = 1; i < MAX_THREAD; i++) {
            pthread_join(threads[i], NULL);
        }
        output(prefix_sum, n);
    }
    return 0;
}
/*
3 2
10 5

100
54560
*/

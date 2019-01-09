#include "matrix.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define UINT unsigned long
#define MAXT 16
typedef struct argu{
    int n;
    UINT *AL, *BL, *result;
}argu;
UINT BT[2048][2048];
argu temp[MAXT];
void *subtask(void *id){
    argu *argv = (argu *)id;
    for(int i = 0; i < argv->n; i++){
        UINT sum = 0;
        for (int j = 0; j < argv->n; j++) {
            sum += *(argv->AL + j) * *(argv->BL + (i * 2048 + j));
        }
        *(argv->result + i) = sum;
    }
    pthread_exit(NULL);
}
void multiply(int N, UINT A[][2048], UINT B[][2048], UINT C[][2048]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            BT[i][j] = B[j][i], C[i][j] = 0;
    pthread_t threads[MAXT];
    int tid[MAXT] = {}, count = 0;
    for (int i = 0; i < N; i++) {
        count = (count+1 >= MAXT) ? MAXT : count+1;
        int number = i % MAXT;
        tid[number] = number;
        temp[number].n = N;
        temp[number].AL = &A[i][0], temp[number].BL = &BT[0][0], temp[number].result = &C[i][0];
        pthread_create(&threads[number], NULL, subtask, (void *)&temp[number]);
        if(number == MAXT - 1){
            count = 0;
            for(int k = 0; k < MAXT; k++)
                pthread_join(threads[k], NULL);
        }
    }
    for(int k = 0; k < count; k++)
    {
        pthread_join(threads[k], NULL);
    }
}

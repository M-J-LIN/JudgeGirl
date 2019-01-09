#include <stdio.h>
#define MAXN 1000
 
char A[MAXN][MAXN], B[MAXN][MAXN];
void swap(char *a, char *b){char temp; temp = *a; *a = *b; *b = temp;}
int main(){
    int N, M;
    scanf("%d %d", &N, &M);
 
    for(int i = 0; i < N; i++){
        scanf(" ");
        for(int j = 0; j < N; j++){
            A[i][j] = B[i][j] = getchar();
        }
    }
 
    for(int i = 0; i < M; i++){
        #pragma omp parallel for
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N - 1; k++){
                if(A[j][k] == 'R' && A[j][k + 1] == 'W')
                    swap(&B[j][k], &B[j][k + 1]);
            }
            if(A[j][N - 1] == 'R' && A[j][0] == 'W')
                swap(&B[j][N - 1], &B[j][0]);
        }
        #pragma omp parallel for
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                A[i][j] = B[i][j];
            }
        }
        #pragma omp parallel for
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N - 1; k++){
                if(A[k][j] == 'B' && A[k + 1][j] == 'W')
                    swap(&B[k][j], &B[k + 1][j]);
            }
            if(A[N - 1][j] == 'B' && A[0][j] == 'W')
                swap(&B[N - 1][j], &B[0][j]);
        }
        #pragma omp parallel for
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                A[i][j] = B[i][j];
            }
        }
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            putchar(A[i][j]);
        }
        putchar('\n');
    }
    return 0;
}
/*
4 1
RRWR
WWBW
BWRW
WWWW

RWRR
WWWW
WWBR
BWWW
*/

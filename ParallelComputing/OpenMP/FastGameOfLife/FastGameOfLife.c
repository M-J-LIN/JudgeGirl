#include <stdio.h>
#define MAX 2000
 
char A[MAX + 2][MAX + 2], B[MAX + 2][MAX + 2];
 
int main(){
    int N, M;
    scanf("%d %d", &N, &M);
    for(int i = 1;i <= N; i++)
        for(int j = 1; j <= N; j++)
            scanf("\n%c", &A[i][j]);
    for(int i = 1;i <= N; i++)
        for(int j = 1; j <= N; j++)
            A[i][j] -= '0';
 
    for(int i = 0; i < M; i++){
        #pragma omp parallel for
        for(int j = 1; j <= N; j++){
            for(int k = 1; k <= N; k++){
                int sum = A[j - 1][k - 1] + A[j - 1][k] + A[j - 1][k + 1] +
                          A[j][k - 1] + A[j][k + 1] +
                          A[j + 1][k - 1] + A[j + 1][k] + A[j + 1][k + 1];
                B[j][k] = (sum == 3 || (A[j][k] && sum == 2)) ? 1 : 0;
            }
        }
        #pragma omp parallel for
        for(int j = 1; j <= N; j++)
            for(int k = 1; k <= N; k++)
                A[j][k] = B[j][k];
    }
 
    for(int i = 1;i <= N; i++){
        for(int j = 1; j <= N; j++){
            putchar(A[i][j] + '0');
        }
        putchar('\n');
    }
 
    return 0;
}
/*
5 1
10001
00100
01110
00100
01010

00000
00100
01010
00000
00100
*/

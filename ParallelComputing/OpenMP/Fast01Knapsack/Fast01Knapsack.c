#include <stdio.h>
#include <stdlib.h>
 
#define MAXM 1000005
#define MAXN 10005
int dp[2][MAXM];
int W[MAXN], V[MAXN];
#define max(a, b) (a > b ? a : b)
int main() {
    int N, M;
    while (scanf("%d %d", &N, &M) == 2) {
        for (int i = 0; i < N; i++)
            scanf("%d %d", &W[i], &V[i]);
        for (int i = 0; i < 2; i++)
            memset(dp[i], 0, sizeof(int) * (M+1));
 
        int p = 0, q;
        for (int i = 0; i < N; i++) {
            q = 1 - p;
            #pragma omp parallel
            {
                int pp = p, qq = q;
                int v = V[i], w = W[i];
                #pragma omp for
                for (int i = w; i <= M; i++)
                    dp[qq][i] = max(dp[pp][i-w]+v, dp[pp][i]);
                  #pragma omp for
                for (int i = 0; i < w; i++)
                    dp[qq][i] = dp[pp][i];
            }
            p = 1 - p;
        }
 
        printf("%d\n", dp[p][M]);
    }
    return 0;
}
/*
4 5
2 3
1 2 
3 4
2 2

7
*/

#include <stdio.h>                                                                                                                                                                   
#include <stdlib.h>
#define MAXN 20
int board[MAXN];
int N;
int goQueen(int l, int r, int n, int pre){
    if(n == N)
        return 1;
    int sum = 0;
    int err = pre | l | r | board[n];
    int test = ~err;
    test &= (1<<N)-1;
    while(test){
        int position = test & (-test);
        test ^= position;
        sum += goQueen((l|position)<<1, (r|position)>>1, n+1, position|pre);
    }   
    return sum;
}
int main(){
    int numCase = 0;
    while(scanf("%d", &N) != EOF){
        for(int i = 0; i < N; i++)
            board[i] = 0;
        numCase++;
        char barrier;
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                scanf("\n%c", &barrier);
                if(barrier == '*')
                    board[i] |= 1<<j;
            }   
        }   
        int sol = 0, pre;
#pragma omp parallel for reduction(+ : sol)
        for(int i = 0; i < N; i++){
            if(((board[0]>>i) & 1)) 
                continue;
            pre = 1 << i;
            sol = goQueen(pre<<1, pre>>1, 1, pre);
        }   
        printf("Case %d: %d\n", numCase, sol);
    }   
    return 0;
}
/*
8
........
........
........
........
........
........
........
........
4
.*..
....
....
....

Case 1: 92
Case 2: 1
*/

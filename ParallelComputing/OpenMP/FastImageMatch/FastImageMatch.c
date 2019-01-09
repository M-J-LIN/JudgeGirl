#include <stdio.h>
#include <stdlib.h>
#define MAX 512
 
int A[MAX][MAX], B[MAX][MAX];
int main(){
    int AH, AW, BH, BW;
    while(scanf("%d %d %d %d", &AH, &AW, &BH, &BW) != EOF){
        for(int i = 0; i < AH; i++)
            for(int j = 0; j < AW; j++)
                scanf("%d", &A[i][j]);
        for(int i = 0; i < BH; i++)
            for(int j = 0; j < BW; j++)
                scanf("%d", &B[i][j]);
 
        int min = 2147483647, x, y;
#pragma omp parallel for
        for(int i = 0; i <= AH - BH; i++){
            for(int j = 0; j <= AW - BW; j++){
                int temp = 0;
                for(int k = 0; k < BH; k++){
                    for(int l = 0; l < BW; l++){
                        temp += (A[k + i][l + j] - B[k][l]) * (A[k + i][l + j] - B[k][l]);
                    }
                }
#pragma omp critical
                if(temp < min || (temp == min && (i+1 < x || (i+1 == x && j < y))))
                    min = temp, x = i + 1, y = j + 1;
            }
        }
        printf("%d %d\n", x, y);
 
    }
    return 0;
}
/*
4 4 2 2
0 3 3 0
0 3 3 0
0 0 5 5
0 0 5 5
5 5
5 5
3 5 1 2
2 3 0 4 5
0 0 7 0 0
0 0 7 0 0
3 4

3 3
1 1
*/

#include <stdio.h>
#include <math.h>
#include <algorithm>
#define MAX 250000
#define square(x1, y1, x2, y2) (((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)))
 
using namespace std;
 
typedef struct{
    int x, y, num;
}Point;
 
Point points[MAX];
Point ans[MAX];
int  closest = 2147483647, numOfAns = 0;
int sqr(Point *a, Point *b){
    return square(a->x, a->y, b->x, b->y);
}
bool cmpX(Point a, Point b){
    return a.x < b.x;
}
bool cmpY(Point a, Point b){
    return a.y < b.y;
}
void DC(int left, int right){
    if(right <= left){
        return;
    }
    int mid = (left + right) / 2;
    DC(left, mid);
    DC(mid + 1, right);
 
    int tempright, templeft;
    templeft = tempright = mid;
    for(int i = mid; i >= left && (points[mid].x - points[i].x) * (points[mid].x - points[i].x) <= closest; i--) {templeft = i;}
    for(int i = mid + 1; i <= right && (points[mid].x - points[i].x) * (points[mid].x - points[i].x) <= closest; i++) {tempright = i;}
    sort(points + templeft, points + tempright, cmpY);
 
    for(int i = templeft; i <= tempright; i++){
        for(int j = i + 1; j <= i + 3 && j <= tempright ; j++){
            int temp = sqr(&points[i], &points[j]);
            if(temp > closest)
                break;
            else if(temp <= closest){
                if(temp < closest){
                    closest = temp;
                    numOfAns = 0;
                }
                if(points[i].num != ans[numOfAns - 1].x && points[i].num != ans[numOfAns - 1].y){
                    ans[numOfAns].x = min(points[i].num, points[j].num), ans[numOfAns].y = max(points[i].num, points[j].num);
                    numOfAns++;
                }
            }
        }
    }
    sort(points + templeft, points + tempright, cmpX);
}
 
 
int main(){
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; i++){
        scanf("%d %d", &points[i].x, &points[i].y);
        points[i].num = i + 1;
    }
 
    sort(points, points + N, cmpX);
    closest = sqr(&points[0], &points[1]);
    DC(0, N - 1);
 
    printf("%.2f %d\n", sqrt(float(closest)), numOfAns);
    for(int i = 0; i < numOfAns; i++) printf("%d %d\n", ans[i].x, ans[i].y);
    return 0;
}

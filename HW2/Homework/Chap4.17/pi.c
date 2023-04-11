#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    unsigned long long int total;
    unsigned long long int inCircle;
} points_t;

void *pointAtCircle(void *arg) {
    points_t* points = (points_t*)arg;
    int i;
    double point_x, point_y, distance;
    srand(time(0)); 
    for (i = 0; i < points->total; i++) {
        point_x = (double) rand() / RAND_MAX * 2 - 1;
        point_y = (double) rand() / RAND_MAX * 2 - 1;
        distance = point_x * point_x + point_y * point_y;
        if (distance <= 1) {
            points->inCircle++;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    points_t points;
    memset(&points, 0, sizeof(points));
    printf("輸入隨機點數量 : ");
    scanf("%llu", &points.total);
    pthread_create(&thread, NULL, pointAtCircle, (void*) &points);
    pthread_join(thread, NULL);
    printf("pi = %f\n", 4 * (double)points.inCircle / points.total);
    return 0;
}
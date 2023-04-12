#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define DIVISION 3

typedef struct {
    pthread_mutex_t mutex;
    int division_amount; 
    int remain_amount;
    int total_point;
    int in_circle_point;
} points_t;

void *divisionPoints(void *arg) {
    points_t* points = (points_t*)arg;
    int i;
    double point_x, point_y, distance;
    srand(time(0)); 
    for (i = 0; i < points->division_amount; i++) {
        point_x = (double) rand() / RAND_MAX * 2 - 1;
        point_y = (double) rand() / RAND_MAX * 2 - 1;
        distance = point_x * point_x + point_y * point_y;
        if (distance <= 1) {
            points->inCircle++;
        }
    }
    pthread_exit(NULL);
}

void *remainPoints(void *arg) {
    points_t* points = (points_t*)arg;
    int i;
    double point_x, point_y, distance;
    srand(time(0)); 
    for (i = 0; i < points->remain_amount; i++) {
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
    pthread_t thread[4];
    points_t points;
    int i;
    memset(&points, 0, sizeof(points));
    printf("輸入隨機點數量 : ");
    scanf("%d", &points.total);
    points.division_amount = points.total_point / 3;
    points.remain_amount = points.total_point % 3;
    pthread_create(&thread[0], NULL, pointAtCircle, (void*) &points);
    for (i = 1; i < 4; i++) {
        pthread_create(&thread[i], NULL, pointAtCircle, (void*) &points);
    }
    
    pthread_join(thread, NULL);
    printf("pi = %f\n", 4 * (double)points.inCircle / points.total);
    return 0;
}
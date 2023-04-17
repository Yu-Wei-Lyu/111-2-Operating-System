#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define DIVISION 3
#define THREADS 4

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
            pthread_mutex_lock(&points->mutex);
            points->in_circle_point++;
            pthread_mutex_unlock(&points->mutex);
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
            pthread_mutex_lock(&points->mutex);
            points->in_circle_point++;
            pthread_mutex_unlock(&points->mutex);
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread[THREADS];
    points_t points;
    int i;
    memset(&points, 0, sizeof(points));
    printf("輸入隨機點數量 : ");
    scanf("%d", &points.total_point);
    points.division_amount = points.total_point / (THREADS - 1);
    points.remain_amount = points.total_point % (THREADS - 1);
    pthread_mutex_init(&points.mutex, NULL);
    double start, end;
    start = clock();
    pthread_create(&thread[0], NULL, remainPoints, (void*) &points);
    for (i = 1; i < THREADS; i++) {
        pthread_create(&thread[i], NULL, divisionPoints, (void*) &points);
    }
    for (i = 0; i < THREADS; i++) {
        pthread_join(thread[i], NULL);
    }
    printf("pi = %f\n", 4 * (double)points.in_circle_point / points.total_point);
    end = clock();
    printf("總花費時間 : %f s\n", (end - start) / CLOCKS_PER_SEC );
    return 0;
}
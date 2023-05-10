#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define NORTH_FARMERS 2
#define SOUTH_FARMERS 2

pthread_mutex_t mutex;
pthread_cond_t cond_north;
pthread_cond_t cond_south;
int northbound_passing, southbound_passing;

void *northbound_farmer();
void *southbound_farmer();
void time_simulation();

int main() {
    pthread_t northbound_thread[NORTH_FARMERS];
    pthread_t southbound_thread[SOUTH_FARMERS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_north, NULL);
    pthread_cond_init(&cond_south, NULL);
    srand(time(NULL));
    int i;

    char farmer_id[NORTH_FARMERS * SOUTH_FARMERS];
    int id_index = 0;
    int ascii_value = 65;
    for (i = 0; i < NORTH_FARMERS * SOUTH_FARMERS; i++)
        farmer_id[i] = (char)ascii_value++;

    northbound_passing = 0;
    southbound_passing = 0;
    for (i = 0; i < NORTH_FARMERS; i++)
        pthread_create(&northbound_thread[i], NULL, northbound_farmer, (void *) &farmer_id[id_index++]);
    for (i = 0; i < SOUTH_FARMERS; i++)
        pthread_create(&southbound_thread[i], NULL, southbound_farmer, (void *) &farmer_id[id_index++]);
    for (i = 0; i < NORTH_FARMERS; i++)
        pthread_join(northbound_thread[i], NULL);
    for (i = 0; i < SOUTH_FARMERS; i++)
        pthread_join(southbound_thread[i], NULL);
    //pthread_cond_wait(&cond, &mutex);
    //pthread_cond_signal(&cond);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_north);
    pthread_cond_destroy(&cond_south);
    
    return 0;
}

void *northbound_farmer(void* arg) {
    char id = *(char*)arg;
    int waiting = 0;
    printf("Farmer %c is awake.\n", id);
    while(1) {
        time_simulation(5);
        pthread_mutex_lock(&mutex);
        while (southbound_passing > 0) {
            if(waiting == 0)
                printf("[Waiting from north] Farmer %c is waiting.\n", id);
            waiting = 1;
            pthread_cond_wait(&cond_south, &mutex);
        }
        waiting = 0;
        ++northbound_passing;
        printf("[Northbound] Farmer %c is passing.\n", id);
        pthread_mutex_unlock(&mutex);

        time_simulation(3);

        pthread_mutex_lock(&mutex);
        --northbound_passing;
        printf("[Northbound] Farmer %c has passed.\n", id);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond_north);
    }

    pthread_exit(NULL);
}

void *southbound_farmer(void* arg) {
    char id = *(char*)arg;
    int waiting = 0;
    printf("Farmer %c is awake.\n", id);
    while(1) {
        time_simulation(5);

        pthread_mutex_lock(&mutex);
        while (northbound_passing > 0) {
            if(waiting == 0)
                printf("[Waiting from south] Farmer %c is waiting.\n", id);
            waiting = 1;
            pthread_cond_wait(&cond_north, &mutex);
        }
        waiting = 0;
        ++southbound_passing;
        printf("[Southbound] Farmer %c is passing.\n", id);
        pthread_mutex_unlock(&mutex);

        time_simulation(3);

        pthread_mutex_lock(&mutex);
        printf("[Southbound] Farmer %c has passed.\n", id);
        --southbound_passing;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond_south);
    }
    pthread_exit(NULL);
}

// 模擬現實事件執行時間
void time_simulation(int max_second) {
    sleep(rand() % max_second + 1);
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MAX_CUSTOMERS 2
#define MAX_RESOURCES 4

bool isSafeState(int customer_id, int request[]);
void generate_request(int customer_id, int request[MAX_RESOURCES]);
void request_resources(int customer_id, int request[MAX_RESOURCES]);
void release_resources(int customer_id);

int available[MAX_RESOURCES];
int maximum[MAX_CUSTOMERS][MAX_RESOURCES];
int allocation[MAX_CUSTOMERS][MAX_RESOURCES];
int need[MAX_CUSTOMERS][MAX_RESOURCES];
void *customer(void *customer_id_ptr);

pthread_mutex_t mutex;

int main() {
    pthread_t threads[MAX_CUSTOMERS];
    int customerIds[MAX_CUSTOMERS];
    srand(time(NULL));
    memset(allocation, 0, sizeof(allocation));
    memset(need, 0, sizeof(need));

    printf("Enter available (%d numbers split with space): ", MAX_RESOURCES);
    for (int i = 0; i < MAX_RESOURCES; i++) {
        scanf("%d", &available[i]);
    }
    printf("Enter maximum:\n");
    for (int id = 0; id < MAX_CUSTOMERS; id++) {
        printf("\tProccess %d maximum (%d numbers split with space):", id, MAX_RESOURCES);
        for (int res = 0; res < MAX_RESOURCES; res++) {
            scanf("%d", &maximum[id][res]);
        }
    }
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        customerIds[i] = i;
        pthread_create(&threads[i], NULL, customer, (void *)&customerIds[i]);
    }
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}

bool isSafeState(int customer_id, int request[]) {
    int temp_need[MAX_CUSTOMERS][MAX_RESOURCES];
    for (int i = 0; i < MAX_RESOURCES; i++) {
        temp_need[customer_id][i] = maximum[customer_id][i] - allocation[customer_id][i];
    }
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > available[i] || temp_need[customer_id][i] > available[i]) {
            return false;
        }
    }
    return true;
}

void generate_request(int customer_id, int request[MAX_RESOURCES]) {
    printf("Customer %d's request: ", customer_id);
    for (int i = 0; i < MAX_RESOURCES; i++) {
        request[i] = rand() % (maximum[customer_id][i] + 1);
        printf("%d ", request[i]);
    }
    printf("\n");
}

void request_resources(int customer_id, int request[MAX_RESOURCES]) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_id][i] += request[i];
    }
}

void release_resources(int customer_id) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] += allocation[customer_id][i];
        allocation[customer_id][i] = 0;
    }
}

void *customer(void *customer_id_ptr) {
    int customer_id = *(int *)customer_id_ptr;
    int request[MAX_RESOURCES];
    int waiting = 0;

    generate_request(customer_id, request);
    while (true) {
        pthread_mutex_lock(&mutex);
        if (isSafeState(customer_id, request)) {
            request_resources(customer_id, request);
            printf("Customer %d's request granted.\n", customer_id);
            waiting = 0;
        } else {
            pthread_mutex_unlock(&mutex);
            if (waiting == 0) {
                printf("Customer %d's request cannot be granted. Waiting...\n", customer_id);
                waiting = 1;
            }
            continue;
        }
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3 + 1);
        pthread_mutex_lock(&mutex);
        release_resources(customer_id);
        printf("Customer %d's release resources.\n", customer_id);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 5 + 1);
        generate_request(customer_id, request);
    }
}
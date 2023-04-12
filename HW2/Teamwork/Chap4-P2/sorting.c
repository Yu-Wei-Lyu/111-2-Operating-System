#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 2048

typedef struct {
    int list[MAX_SIZE];
    int size;
} sublist_t;

int compare(const void *a, const void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    return (*x - *y);
}

void *sort(void *arg) {
    sublist_t *sublist = (sublist_t *)arg;
    qsort(sublist->list, sublist->size, sizeof(int), compare);
    pthread_exit(NULL);
}

void *merge(void *arg) {
    int mid = SIZE / 2;
    int i = 0, j = mid, k = 0;
    while (i < mid && j < SIZE) {
        if (list[i] < list[j]) {
            result[k++] = list[i++];
        } else {
            result[k++] = list[j++];
        }
    }
    while (i < mid) {
        result[k++] = list[i++];
    }
    while (j < SIZE) {
        result[k++] = list[j++];
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];
    sublist_t sublists[2];
    sublists[0].list = list;
    sublists[0].size = SIZE / 2;
    sublists[1].list = list + (SIZE / 2);
    sublists[1].size = SIZE / 2;
    pthread_create(&threads[0], NULL, sort, &sublists[0]);
    pthread_create(&threads[1], NULL, sort, &sublists[1]);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_create(&threads[2], NULL, merge, NULL);
    pthread_join(threads[2], NULL);
    printf("Sorted list:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}
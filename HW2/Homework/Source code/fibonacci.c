#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int* array;
    int size;
} fibonacci_t;

void *fibonacciFunc(void *arg) {
    fibonacci_t* data = (fibonacci_t*)arg;
    int i;
    data->array[0] = 0;
    if (data->size > 1) {
        data->array[1] = 1;
        for (i = 2; i < data->size; i++) {
            data->array[i] = data->array[i - 1] + data->array[i - 2];
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    fibonacci_t fibonacci;
    memset(&fibonacci, 0, sizeof(fibonacci));
    int fibonacci_value;
    while(1) {
        printf("輸入費式數列最大項 : ");
        scanf("%d", &fibonacci_value);
        if (fibonacci_value < 0) {
            printf("請輸入 >= 0 的項\n");
        } else {
            break;
        }
    }
    fibonacci.size = fibonacci_value + 1;
    fibonacci.array = (int *)malloc(fibonacci.size * sizeof(int));
    pthread_create(&thread, NULL, fibonacciFunc, (void*) &fibonacci);
    pthread_join(thread, NULL);
    int i;
    printf("費式數列 : %d", fibonacci.array[0]);
    for (i = 1; i < fibonacci.size; i++) {
        printf(", %d", fibonacci.array[i]);
    }
    printf("\n");
    free(fibonacci.array);
    return 0;
}
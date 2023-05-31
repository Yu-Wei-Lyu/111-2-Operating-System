#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void fcfs(int request[], int request_size);
void sstf(int request[], int request_size);
void scan(int request[], int request_size);
void c_scan(int request[], int request_size);
void copy_array(int source[], int dest[], int size);
int compare(const void *a, const void *b);
int sort_with_head(int arr[], int request_size);
void rebuild_array(int arr[], int request_size, int split_index);

int main(int argc, char* argv[]) {
    int request_size = argc -1;
    int request[request_size];
    for (int i = 0; i < request_size; ++i) {
        request[i] = atoi(argv[i + 1]);
    }

    fcfs(request, request_size);
    sstf(request, request_size);
    scan(request, request_size);
    c_scan(request, request_size);
    return 0;
}

void copy_array(int source[], int dest[], int size) {
    for (int i = 0; i < size; ++i) {
        dest[i] = source[i];
    }
}

void fcfs(int request[], int request_size) {
    int arr[request_size];
    copy_array(request, arr, request_size);
    int total = 0;
    printf("FCFS \t");
    printf("%d ", arr[0]);
    for (int i = 1; i < request_size; ++i) {
        total += abs(arr[i] - arr[i-1]);
        printf("%d ", arr[i]);
    }
    printf("\nFCFS total distance: %d\n", total);
}

int compare(const void *a, const void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    return (*x - *y);
}

int sort_with_head(int arr[], int request_size) {
    int head = arr[0];
    qsort(arr, request_size, sizeof(int), compare);
    for (int i = 0; i < request_size; ++i) {
        if (arr[i] == head)
            return i;
    }
}

void sstf(int request[], int request_size) {
    int arr[request_size];
    copy_array(request, arr, request_size);
    int head_index = sort_with_head(arr, request_size);
    int seak_foward = head_index - 1;
    int seak_backward = head_index + 1;
    int total = 0;
    printf("SSTF \t");
    printf("%d ", arr[head_index]);
    while(seak_foward >= 0 || seak_backward < request_size) {
        int foward_distance = (seak_foward >= 0) ? arr[head_index] - arr[seak_foward] : INFINITY;
        int backword_distance = (seak_backward < request_size) ? arr[seak_backward] - arr[head_index] : INFINITY;
        int next_index = (foward_distance > backword_distance) ? seak_backward++ : seak_foward--;
        total += abs(arr[head_index] - arr[next_index]);
        printf("%d ", arr[next_index]);
        head_index = next_index;
    }
    printf("\nSSTF total distance: %d\n", total);
}

void rebuild_array(int arr[], int request_size, int head_index) {
    int new_arr[request_size];
    int new_arr_index = 0;
    int i;
    for (i = head_index; i < request_size; ++i) {
        new_arr[new_arr_index++] = arr[i];
    }
    for (i = head_index - 1; i >= 0; --i) {
        new_arr[new_arr_index++] = arr[i];
    }
    copy_array(new_arr, arr, request_size);
}

void scan(int request[], int request_size) {
    int arr[request_size];
    copy_array(request, arr, request_size);
    int head_index = sort_with_head(arr, request_size);
    rebuild_array(arr, request_size, head_index);
    int total = 0;
    printf("SCAN \t");
    printf("%d ", arr[0]);
    for (int i = 1; i < request_size; ++i) {
        total += abs(arr[i] - arr[i-1]);
        printf("%d ", arr[i]);
    }
    printf("\nSCAN total distance: %d\n", total);
}

void rebuild_array_tail(int arr[], int request_size, int head_index) {
    int new_arr[request_size];
    int new_arr_index = 0;
    int i;
    for (i = head_index; i < request_size; ++i) {
        new_arr[new_arr_index++] = arr[i];
    }
    for (i = 0; i < head_index; ++i) {
        new_arr[new_arr_index++] = arr[i];
    }
    copy_array(new_arr, arr, request_size);
}

void c_scan(int request[], int request_size) {
    int arr[request_size];
    copy_array(request, arr, request_size);
    int head_index = sort_with_head(arr, request_size);
    rebuild_array_tail(arr, request_size, head_index);
    int total = 0;
    printf("C-SCAN \t");
    printf("%d ", arr[0]);
    for (int i = 1; i < request_size; ++i) {
        total += abs(arr[i] - arr[i-1]);
        printf("%d ", arr[i]);
    }
    printf("\nC-SCAN total distance: %d\n", total);
}
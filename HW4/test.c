#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int compare(const void *a, const void *b) {
    int *x = (int *)a;
    int *y = (int *)b;
    return (*x - *y);
}

int main() {
    int arr[] = {2150, 2069, 1212, 2296, 2800, 544, 1618, 356, 1523, 4965, 3681};
    int total = 0;
    int i, j;
    // printf("FCFS --");
    // for (i = 1; i < sizeof(arr)/sizeof(arr[0]); ++i) {
    //     int now = abs(arr[i] - arr[i-1]);
    //     total += now;
    //     printf("%d\n", now);
    // }
    // printf("Total : %d", total);
    int scanarr[] = {2150, 2296, 2800, 3681, 4965, 4999, 2069, 1618, 1523, 1212, 544, 356};
    // total = 0;
    // qsort(arr, 11, sizeof(int), compare);
    // printf("SCAN --");
    // for (i = 1; i < sizeof(scanarr)/sizeof(scanarr[0]); ++i) {
    //     int now = abs(scanarr[i] - scanarr[i-1]);
    //     total += now;
    //     printf("%d\n", now);
    // }
    // printf("Total : %d", total);

    // int cscanarr[] = {2150, 2296, 2800, 3681, 4965, 4999, 0, 356, 544, 1212, 1523, 1618, 2069};
    // total = 0;
    // printf("C-SCAN --");
    // for (i = 1; i < sizeof(cscanarr)/sizeof(cscanarr[0]); ++i) {
    //     int now = abs(cscanarr[i] - cscanarr[i-1]);
    //     total += now;
    //     printf("%d\n", now);
    // }

    int sortarr[] = {356, 544, 1212, 1523, 1618, 2069, 2150, 2296, 2800, 3681, 4965};
    int n = 6;
    i = n - 1;
    j = n + 1;
    total = 0;
    while(i >= 0 && j < sizeof(sortarr)/sizeof(sortarr[0])) {
        int num1 = sortarr[i];
        int num2 = sortarr[j];
        int abs1 = (i >= 0) ? sortarr[n] - sortarr[i] : 10000;
        int abs2 = (j < sizeof(sortarr)/sizeof(sortarr[0])) ? sortarr[j] - sortarr[n] : 10000;
        printf("now at %d, %d (%d) : %d (%d)", sortarr[n], sortarr[i], abs1, sortarr[j], abs2);
        int next = (abs1 > abs2) ? j++ : i--;
        printf("go to %d\n", sortarr[next]);
        total += abs(sortarr[n] - sortarr[next]);

        n = next;
    }
    printf("Total : %d", total);
    printf("Hello\n");
    return 0;
}
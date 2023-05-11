#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int ref_size = 10;
    int page_ref[ref_size];
    int i, j;
    if (argc == 1) {
        for (i = 0; i < ref_size; i++) {
            page_ref[i] = rand() % 10;
        }
    }

    int frame_size = 3;
    int frame[frame_size] = { -1 };
    int fifo[frame_size] = { -1 };
    int fifo_index = 0;
    int page_fault = 0;
    for (i = 0; i < ref_size; i++) {
        printf("now request : %d\n", page_ref[i]);
        for (j = 0; j < frame_size; j++) {
            if (frame[j] == page_ref[i]) {
                break;
            }
        }
        if (j == frame_size) {
            ++page_fault;
            fifo[fifo_index] = page_ref[i];
            fifo_index = 
        }
    }


    printf("end\n");
    return 0;
}
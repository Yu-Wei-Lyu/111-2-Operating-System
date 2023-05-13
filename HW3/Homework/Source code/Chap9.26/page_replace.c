#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define REFERECE_STRING_TYPE 10
#define REFERECE_SIZE 20

void init_refs(char page_refs[]);
void lru(const char page_refs[], const int frame_size);
void fifo(const char page_refs[], const int frame_size);
void optimal(const char page_refs[], const int frame_size);
int longest_called(const char page_refs[], const int current_index, const char frame[], const int frame_size);
int longest_called_again(const char page_refs[], const int current_index, const char frame[], const int frame_size);


int main(int argc, char *argv[]) {
    srand(time(NULL));
    char page_refs[REFERECE_SIZE];
    init_refs(page_refs);

    for (int frame_size = 1; frame_size <= 7; ++frame_size) {
        printf("\n");
        // printf("=== FIFO algorithm ===\n");
        fifo(page_refs, frame_size);
        // printf("=== Optimal algorithm ===\n");
        optimal(page_refs, frame_size);
        // printf("=== LRU algorithm ===\n");
        lru(page_refs, frame_size);
    }

    return 0;
}

// Initialize page-reference string
void init_refs(char page_refs[]) {
    printf("Page reference string : ");
    for (int refs_index = 0; refs_index < REFERECE_SIZE; refs_index++) {
        page_refs[refs_index] = (char)('0' + rand() % REFERECE_STRING_TYPE);
        printf("%c ", page_refs[refs_index]);
    }
    printf("\n");
}

// Least recenty used replacement algorithm
void lru(const char page_refs[], const int frame_size) {
    char frame[frame_size];
    memset(frame, ' ', sizeof(frame));
    int page_fault = 0;
    int refs_index, frame_index;
    for (refs_index = 0; refs_index < REFERECE_SIZE; refs_index++) {
        // printf("Request %c\n", page_refs[refs_index]);
        for (frame_index = 0; frame_index < frame_size; frame_index++) {
            if (frame[frame_index] == page_refs[refs_index]) {
                break;
            }
        }
        if (frame_index == frame_size) {
            // printf("page fault!\n");
            ++page_fault;
            int longest_unused_frame = longest_called(page_refs, refs_index, frame, frame_size);
            frame[longest_unused_frame] = page_refs[refs_index];
            // printf("current frame:");
            // for(frame_index = 0; frame_index < frame_size; frame_index++) {
            //     printf("%c ", frame[frame_index]);
            // }
            // printf("\n");
        } 
        // else {
        //     printf("Memory resident.\n");
        // }

    }
    printf("[%d frames] Page fault %d with LRU algorithm.\n", frame_size, page_fault);
}

// FIFO replacement algorithm
void fifo(const char page_refs[], const int frame_size) {
    char frame[frame_size];
    char fifo[frame_size];
    memset(frame, ' ', sizeof(frame));
    memset(fifo, ' ', sizeof(fifo));
    int fifo_index = 0;
    int page_fault = 0;
    int refs_index, frame_index;
    for (refs_index = 0; refs_index < REFERECE_SIZE; refs_index++) {
        // printf("Request %c\n", page_refs[refs_index]);
        for (frame_index = 0; frame_index < frame_size; frame_index++) {
            if (frame[frame_index] == page_refs[refs_index]) {
                break;
            }
        }
        if (frame_index == frame_size) {
            // printf("page fault!\n");
            ++page_fault;
            fifo[fifo_index] = page_refs[refs_index];
            frame[fifo_index] = page_refs[refs_index];
            fifo_index = (fifo_index + 1) % frame_size;
            // printf("current frame:");
            // for(frame_index = 0; frame_index < frame_size; frame_index++) {
            //     printf("%c ", frame[frame_index]);
            // }
            // printf("\n");
        }
        // else {
        //     printf("Memory resident.\n");
        // }

    }
    printf("[%d frames] Page fault %d with FIFO algorithm.\n", frame_size, page_fault);
}

// Optimal replacement algorithm
void optimal(const char page_refs[], const int frame_size) {
    char frame[frame_size];
    memset(frame, ' ', sizeof(frame));
    int page_fault = 0;
    int refs_index, frame_index;
    for (refs_index = 0; refs_index < REFERECE_SIZE; refs_index++) {
        // printf("Request %c\n", page_refs[refs_index]);
        for (frame_index = 0; frame_index < frame_size; frame_index++) {
            if (frame[frame_index] == page_refs[refs_index]) {
                break;
            }
        }
        if (frame_index == frame_size) {
            // printf("page fault!\n");
            ++page_fault;
            int longest_unused_frame = longest_called_again(page_refs, refs_index, frame, frame_size);
            frame[longest_unused_frame] = page_refs[refs_index];
            // printf("current frame:");
            // for(frame_index = 0; frame_index < frame_size; frame_index++) {
            //     printf("%c ", frame[frame_index]);
            // }
            // printf("\n");
        }
        // else {
        //     printf("Memory resident.\n");
        // }

    }
    printf("[%d frames] Page fault %d with Optimal algorithm.\n", frame_size, page_fault);
}

// Return longest called frame index in the past (for LRU replacement algorithm)
int longest_called(const char page_refs[], const int current_index, const char frame[], const int frame_size) {
    int unused_frame_index = -1;
    int unused_distance = REFERECE_SIZE;
    int frame_index, ref_index;
    for (frame_index = 0; frame_index < frame_size; ++frame_index) {
        for (ref_index = current_index; ref_index < REFERECE_SIZE; --ref_index) {
            if (page_refs[ref_index] == frame[frame_index]) {
                break;
            }
        }
        if (unused_distance > ref_index) {
            unused_frame_index = frame_index;
            unused_distance = ref_index;
        }
    }
    return unused_frame_index;
}

// Return longest called frame index in future (for Optimal replacement algorithm)
int longest_called_again(const char page_refs[], const int current_index, const char frame[], const int frame_size) {
    int unused_frame_index = -1;
    int unused_distance = -1;
    int frame_index, ref_index;
    for (frame_index = 0; frame_index < frame_size; ++frame_index) {
        for (ref_index = current_index; ref_index < REFERECE_SIZE; ++ref_index) {
            if (page_refs[ref_index] == frame[frame_index]) {
                break;
            }
        }
        if (unused_distance < ref_index) {
            unused_frame_index = frame_index;
            unused_distance = ref_index;
        }
    }
    return unused_frame_index;
}
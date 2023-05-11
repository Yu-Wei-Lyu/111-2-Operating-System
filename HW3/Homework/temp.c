#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_NUM 10
#define FRAME_NUM_MAX 7

void fifo(int refs[], int n, int frames);
void lru(int refs[], int n, int frames);
void optimal(int refs[], int n, int frames);

int main()
{
    int refs[100], n = 0;
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        refs[i] = rand() % PAGE_NUM;
        n++;
    }
    for (int frames = 1; frames <= FRAME_NUM_MAX; frames++) {
        printf("Frames: %d\n", frames);
        printf("FIFO:\n");
        fifo(refs, n, frames);
        printf("LRU:\n");
        lru(refs, n, frames);
        printf("Optimal:\n");
        optimal(refs, n, frames);
    }
    return 0;
}

void fifo(int refs[], int n, int frames)
{
    int fifo[frames];
    int fault = 0, idx = 0;
    for (int i = 0; i < n; i++) {
        int j = 0;
        for (; j < frames; j++)
            if (fifo[j] == refs[i])
                break;
        if (j == frames) {
            fault++;
            fifo[idx] = refs[i];
            idx = (idx + 1) % frames;
        }
    }
    printf("Page faults: %d\n", fault);
}

void lru(int refs[], int n, int frames)
{
    int lru[frames];
    int time[frames], fault = 0;
    for (int i = 0; i < frames; i++)
        time[i] = -1;
    for (int i = 0; i < n; i++) {
        int j = 0;
        for (; j < frames; j++)
            if (lru[j] == refs[i])
                break;
        if (j == frames) {
            fault++;
            int k = 0;
            for (int l = 1; l < frames; l++)
                if (time[l] < time[k])
                    k = l;
            lru[k] = refs[i];
            time[k] = i;
        } else {
            time[j] = i;
        }
    }
    printf("Page faults: %d\n", fault);
}

void optimal(int refs[], int n, int frames)
{
    int opt[frames];
    int next[PAGE_NUM];
    int fault = 0;
    for (int i = 0; i < frames; i++)
        opt[i] = -1;
    for (int i = 0; i < n; i++) {
        int j = 0;
        for (; j < frames; j++)
            if (opt[j] == refs[i])
                break;
        if (j == frames) {
            fault++;
            int k = 0;
            for (int l = 1; l < frames; l++) {
                if (next[opt[l]] > next[opt[k]])
                    k = l;
            }
            opt[k] = refs[i];
        }
        next[refs[i]] = i;
    }
    printf("Page faults: %d\n", fault);
}
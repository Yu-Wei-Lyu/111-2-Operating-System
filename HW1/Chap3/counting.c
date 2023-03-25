#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_MAX 4086

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("[Error] Use: ./counting <positive number>\n");
        return 1;
    }

    int num = atoi(argv[1]);
    
    if (num < 1) {
        printf("[Error] Number must be positive.\n");
        return 1;
    }

    pid_t pid = fork();
    
    if (pid > 0) {
        printf("[Parent] Waiting child process finish ...\n");
        wait(NULL);
        printf("[Parent] Process is done.\n");
    } else if (pid == 0) {
        printf("[Child] Process working ...\n");
        printf("[Child] Number sequence: %d", num);
        while (num != 1) {
            if (num % 2 == 0) {
                num /= 2;
            } else {
                num = 3 * num + 1;
            }
            printf(", %d", num);
        }
        printf("\n[Child] Process is done.\n");
    } else {
        perror("fork()");
        exit(-1);
    }
    
    return 0;
}

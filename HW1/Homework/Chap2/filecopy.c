#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_MAX 4086
#define FILE_IN "Source"
#define FILE_OUT "Destination"

int openFile(char value[], char type[]);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("[Error] Use: ./fileCopy.o <source file name> <destination file name>\n");
        return 1;
    }

    int fd_source;
    int fd_dest;
    char buff[BUFF_MAX] = { 0 };
    
    fd_source = openFile(argv[1], FILE_IN);
    read(fd_source, buff, BUFF_MAX);
    close(fd_source);

    fd_dest = openFile(argv[2], FILE_OUT);
    write(fd_dest, buff, strlen(buff));
    close(fd_dest);

    printf("[Success] Copy file content.\n");
    
    return 0;
}

// Open file with error check
int openFile(char value[], char type[]) {
    int fd = -1;
    if (strcmp(type, FILE_IN) == 0) {
        fd = open(value, O_RDONLY);
    } else if (strcmp(type, FILE_OUT) == 0) {
        fd = open(value, O_TRUNC | O_WRONLY);
    }
    if (fd == -1) {
        printf("[Error] %s file does not exist\n", type);
        exit(1);
    }
}
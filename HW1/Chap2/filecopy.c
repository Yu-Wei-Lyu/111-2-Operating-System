#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_MAX 4086

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("[Error] Use: ./fileCopy.o <source file name> <destination file name>\n");
        return 1;
    }

    int fd_source;
    int fd_dest;
    char buff[BUFF_MAX] = { 0 };
    
    fd_source = open(argv[1], O_RDONLY);
    if (fd_source == -1) {
        printf("[Error] Source file does not exist\n");
        return 1;
    }
    read(fd_source, buff, BUFF_MAX);
    close(fd_source);

    fd_dest = open(argv[2], O_TRUNC | O_WRONLY);
    if (fd_dest == -1) {
        printf("[Error] Destination file does not exist\n");
        return 1;
    }
    
    write(fd_dest, buff, strlen(buff));
    close(fd_dest);

    printf("[Success] Copy file content.\n");
    
    return 0;
}

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DECIMAL 10
#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <virtual_address>\n", argv[0]);
        return 1;
    }

    char *address_str = argv[1];
    char *endptr;
    unsigned long num = strtoul(address_str, &endptr, DECIMAL);
    if (*endptr != '\0' || num > UINT_MAX) {
        printf("Invalid input ( valid: 0 ~ 2^32 )\n");
        return 1;
    }

    unsigned int virtual_address = atoi(argv[1]);

    printf("The address %u contains:\n", virtual_address);
    printf("\tpage number=%u\n", virtual_address / PAGE_SIZE);
    printf("\toffset=%u\n", virtual_address % PAGE_SIZE);
    printf("\twith page size=%u\n", PAGE_SIZE);

    return 0;
}
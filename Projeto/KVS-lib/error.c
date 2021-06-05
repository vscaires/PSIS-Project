#include "KVS-lib.h"

// Improved Read with error checking
int eRead(int fd, void *buf, size_t count){
    int i = read(fd, buf, count);
    if(i < 0){
        perror("read");
        return(-1);
    }

    return i;
}

// Improved write with error checking
int eWrite(int fd, void *buf, size_t count){
    int i = write(fd, buf, count);
    if(i < 0){
        perror("write");
        return(-2);
    }

    return i;
}
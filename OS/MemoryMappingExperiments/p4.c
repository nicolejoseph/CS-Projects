#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int main(){

    char writebuf[4100];
    char buf[1];

    int fd = open("file.txt", O_RDWR| O_CREAT| O_TRUNC, 0666);
    if (fd == -1){
        fprintf(stderr, "Failed to open file\n");
        return -1;
    }
    write(fd, writebuf, sizeof(writebuf));

    char *map = mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED){
        fprintf(stderr, "The mmap system call failed with error: %s\n", strerror(errno));
        return -1;
    }


    printf("Reading from map[4116]: %d\n", map[4116]); //Verify that mmap can be read beyond EOF

    map[4100] = 'X';
    lseek(fd, 15, SEEK_END);
    write(fd, buf, 1);

    lseek(fd, 4100, SEEK_SET);
    if(read(fd, buf, 1) < 0){
        fprintf(stderr, "Read system call failed: %s\n", strerror(errno));
        return -1;
    }

    if(map[4100] != buf[0]){
        printf("The write failed\n");
        return 1;
    }

    if(close(fd) < 0){
        fprintf(stderr, "The file did not close properly: %s\n", strerror(errno));
        return -1;
    }

    printf("The write was successful\n");
    return 0;

}


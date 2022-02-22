#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char* map1;
    char buf[5];
    // open a file, O_CREAT since it doesn't already exist and cases should be self-contained
    int fd = open("file", O_RDWR| O_CREAT| O_TRUNC, 0666);
    if (fd == -1)
    {
        fprintf(stderr, "Error creating file\n");
        return -1;
    }
    write(fd, "LEMON", 5 );

    map1 = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    map1[0] = 'M'; // write to memory mapped region
    map1[2] = 'L';
    lseek (fd, 0, SEEK_SET); // set the offset of file to 0
    read(fd, buf,5); //read the 5 bytes from the file, store in buffer buf
    // write(1,buf,5); //print the 5 bytes from the buffer on the screen: "MELON"
    close (fd);

    if (buf[0] == 'M' && buf[2] == 'L')
    {
        exit(0);
    }
    else
    {
        fprintf(stderr, "MAP_SHARED test program failed.\n");
        return -1;
    }

}

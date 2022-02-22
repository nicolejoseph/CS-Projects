#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/signal.h>

void handler(int sig){

    printf("Signal %d received\n", sig);
    printf("%s\n", strsignal(sig));
    _exit(sig);
}

int main() {

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;

    sigemptyset(&sa.sa_mask);
    int i;
    for (i = 1; i < 32; i++) {
        (void) sigaction(i, &sa, NULL);
    }

    char *map = mmap(NULL, 4, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "The mmap system call failed with error: %s\n", strerror(errno));
        return -1;
    }

    map[3] = 'A';

    if (map[3] != 'A') {
        printf("Write to the mmap failed\n");
        return 255;
    }

    printf("Write to the mmap was successful\n");
    return 0;
}




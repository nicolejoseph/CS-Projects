#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/signal.h>
#include <sys/wait.h>

int numhandled = 0;

// Handler action for REAL TIME signal - should print (numchild x numsig) times
// Handler action for "traditional" non-real time signal - print less
void handler(int sig){

    numhandled++;

}

int main(int argc, char *argv[])
{

    if(argc != 4){
        fprintf(stderr, "Incorrect number of arguments\n");
        exit(-1);
    }

    int sig = atoi(argv[1]);
    int numchild = atoi(argv[2]);
    int numsig = atoi(argv[3]);
    int i, j;
    pid_t pid, ppid;
    ppid = getpid();

    struct sigaction sa;
        sa.sa_handler = handler;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        (void)sigaction(sig,&sa,NULL);

    for(i = 0; i < numchild; i++){
        switch(pid = fork()){
            case -1:            // reports error if fork failed
                perror("fork");
            case 0:             // child process, pid=0
                for(j = 0; j < numsig; j++){
                    // send signals to parent
                    kill(ppid, sig);
                }
                exit(0);
            default:            // parent process
                wait(NULL);
        }
    }

    fprintf(stderr, "%d signals handled\n", numhandled);
    return 0;
}
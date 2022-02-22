#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sched.h>

#include "tas.h"
#include "spinlock.h"
#include "sem.h"

void task(struct sem *A, struct sem *B, int moves){

    int i;

    for(i = 0; i < moves; i++){
        if(sem_try(A) == 0){
            sem_wait(A);
        }
        sem_inc(B);
    }
}

int main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(stderr, "Incorrect number of arguments\n");
        return -1;
    }

    int sem_count = atoi(argv[1]);
    int move_count = atoi(argv[2]);
    pid_t task0,task1,task2,task3,task4,task5;
    struct sem *A, *B, *C;

    A = mmap(NULL, sizeof(struct sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    B = mmap(NULL, sizeof(struct sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    C = mmap(NULL, sizeof(struct sem), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    sem_init(A,sem_count);
    sem_init(B,sem_count);
    sem_init(C,sem_count);

    printf("Initial count of sem A: %d\n", A->count);
    printf("Initial count of sem B: %d\n", B->count);
    printf("Initial count of sem C: %d\n", C->count);

    task0 = fork();

    if(task0 == 0){
        printf("VCPU 0 starting, pid %d\n", getpid());
        task(A,B,move_count);
        //printf("Child 0 (pid %d) done, signal handler was invoked %d times.\n", getpid(), );
        //printf("VCPU 0 done\n");
        exit(0);
    } else {
        task1 = fork();
        if(task1 == 0){
            printf("VCPU 1 starting, pid %d\n", getpid());
            task(A,C,move_count);
            exit(0);
        } else {
            task2 = fork();
            if(task2 == 0){
                printf("VCPU 2 starting, pid %d\n", getpid());
                task(B,A,move_count);
                exit(0);
            } else {
                task3 = fork();
                if(task3 == 0){
                    printf("VCPU 3 starting, pid %d\n", getpid());
                    task(B,C,move_count);
                    exit(0);
                } else {
                    task4 = fork();
                    if(task4 == 0){
                        printf("VCPU 4 starting, pid %d\n", getpid());
                        task(C,A,move_count);
                        exit(0);
                    } else {
                        task5 = fork();
                        if(task5 == 0){
                            printf("VCPU 5 starting, pid %d\n", getpid());
                            task(C,B,move_count);
                            exit(0);
                        }
                    }

                }
            }
        }
    }

    while(wait(NULL) > 0);
    printf("Count of sem A: %d\n", A->count);
    printf("Count of sem B: %d\n", B->count);
    printf("Count of sem C: %d\n", C->count);
    return 0;
}
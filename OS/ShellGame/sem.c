#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "tas.h"
#include "spinlock.h"
#include "sem.h"

void handler(int signum){

}

void sem_init(struct sem *s, int count){

    s->count = count;
    memset(s->waitlist, 0, N_PROC);
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    (void)sigaction(SIGUSR1,&sa,NULL);

}

int sem_try(struct sem *s){

    int returncode = 0;
    spin_lock(&(s->lock));
    if(s->count >= 1){
        s->count--;
        returncode = 1;
    }
    spin_unlock(&(s->lock));

    return returncode;
}

void sem_wait(struct sem *s){

    sigset_t *sigsuspendmask;
    sigemptyset(sigsuspendmask);
    sigaddset(sigsuspendmask, SIGUSR1);

    spin_lock(&(s->lock));
    if(s->count < 1){
        s->waitlist[s->count] = getpid();
        //sigprocmask to temporarily block SIGUSR1
        sigsuspend(sigsuspendmask);
    }
    s->count--;
    spin_unlock(&(s->lock));
}

void sem_inc(struct sem *s){

    spin_lock(&(s->lock));
    s->count++;
    if(s->waitlist[s->count] != 0){
        int i = 0;
        while(i < N_PROC){
            kill(s->waitlist[i], SIGUSR1);
            s->waitlist[i]=0;
            i++;
        }
    }
    spin_unlock(&(s->lock));

}
#define _POSIX_C_SOURCE 200812L
#include <stdio.h>  /*printf*/
#include <signal.h> /*SIGUSR , sig funcitons*/
#include <assert.h> /*assert*/
#include <semaphore.h> /*sem_t*/
#include <pthread.h> /*pthread :mutex, create, type*/
#include <stdlib.h> /*atoi*/
#include "aux_funcs.h"
#include "wd_utils.h" /*AddAndRunScheduler*/

int missed = 0;
int sending_pid = 0;
int g_watchdog_pid = 0 ;


int main(int argc, char const *argv[])
{
    sigset_t full_sigset = {0};
    argus_t argus = {0};
    pthread_mutex_t mutex = {0};
    sigset_t sigset = {0};
    int i = 0;


    pthread_mutex_init(&mutex,NULL);

    for(i = 0 ; i < argc-2 ; i++)
    {
        argus.argv[i] = (char*)argv[i];
    }

    argus.is_usr = 0;
    argus.mutex = &mutex;
    argus.frequency = atoi(argv[argc-2]);
    argus.fail_count = atoi(argv[argc-1]);
    argus.argc = argc;

    /*mask all signals and init signal handler*/
    sigfillset(&full_sigset);
    pthread_sigmask(SIG_BLOCK, &full_sigset, NULL);

    sigemptyset(&full_sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2);
    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);

    InitSignalHandlers();

    DEBUG_ONLY(printf("WATCHDOG STARTED %d , argc = %d , argus.argv = %s\n",getpid(),argc,argus.argv[0]));
    DEBUG_ONLY(printf("WATCHDOG: freq = %d , fail_count = %d\n", argus.frequency, argus.fail_count));
    AddAndRunScheduler(&argus);

    DEBUG_ONLY(printf("WATCHDOG PROCCESS FINISHED\n"));

    return 0;
}


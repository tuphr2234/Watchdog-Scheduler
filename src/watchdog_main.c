#define _POSIX_C_SOURCE 200812L
#include <pthread.h>/*pthread_t, pthread funcs*/
#include <stdio.h>/*printf*/
#include <semaphore.h>/*sem_t and semaphore funcs*/
#include <signal.h>/*sigset_t and signal funcs*/
#include <assert.h>/*assert*/
#include <sys/wait.h>/*wait*/
#include <string.h> /*memcpy*/

#include "aux_funcs.h"
#include "watchdog.h" /*MMI , DNR*/
#include "wd_utils.h" /*AddAndRunScheduler , resuscitate */

int resucicate = 1;
int g_watchdog_pid = 0;
pthread_mutex_t mutex = {0};

void *WDfunction(argus_t *argus)
{
    char *all_argv[20] = {0};
    sigset_t full_sigset = {0};
    sigset_t sigset = {0};
    argus_t argus_thread = {0};
    char freq_arg[10];
    char fail_count_arg[10];
    
    memcpy(&argus_thread,argus,sizeof(argus_thread));
/*mask all signals*/
    InitSignalHandlers();
            
    sigfillset(&full_sigset);
    pthread_sigmask(SIG_BLOCK, &full_sigset, NULL);
/*unmask  SIGUSR1 and SIGUSR2 */

    sigemptyset(&full_sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2);

    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);

    {
        int i =0;

        all_argv[0] = (argus->argv)[0];

        while(i < argus->argc)
        {
            all_argv[i] = argus->argv[i];
            i++;
        }
    
        sprintf(freq_arg , "%d", argus->frequency);
        sprintf(fail_count_arg , "%d", argus->fail_count);

        all_argv[argus->argc] = freq_arg ;
        all_argv[(argus->argc)+1] = fail_count_arg;

    }

    all_argv[0] = argus->argv[0];
    
    if(resucicate)
    {
        resuscitate(1,all_argv); /* create first watchdog */
    }
    wait(NULL); /*wait for watchdog to signal*/
    sem_post(argus->sem);
    DEBUG_ONLY(printf("WATCHDOG IS READY\n"));

    AddAndRunScheduler(&argus_thread);
    
    DEBUG_ONLY(printf("WATCHDOG THREAD FINISHED\n"));

    return NULL;
}

int MakeMeImmortal(int argc,char *argv[], int frequency, int fail_count)
{
    argus_t argus = {0};
    pthread_t wd_thread = {0};
    sigset_t sigset = {0};
    sem_t sem = {0};
    int i = 0;
    sem_init(&sem,0,0);
    pthread_mutex_init(&mutex,NULL);

    /*block SIGUSR1 and SIGUSR2 */
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGUSR1);
    sigaddset(&sigset,SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &sigset, NULL);
   
    argus.is_usr = 1;
    argus.sem = &sem;
    argus.frequency = frequency;
    argus.fail_count = fail_count;
    argus.mutex = &mutex;
    argus.argc = argc;

    while(i < argc)
    {
        argus.argv[i] = argv[i];
        i++;
    }

    DEBUG_ONLY(printf("main pid : %d\n", getpid()));

    EXIT_IF_FAIL(-1 == pthread_create(&wd_thread, NULL, (void* (*)(void *))WDfunction,&argus) , "thread creation failure", 1);
    EXIT_IF_FAIL(0 != pthread_detach(wd_thread), "-------------------thread detach failure-----------", 1);

    /*parent waits until watchdog is ready*/
    sem_wait(&sem);
    sem_destroy(&sem);

    DEBUG_ONLY(printf("END OF MMI\n"));

    return 1;
}

int DoNotResuscitate()
{
    sigset_t sigset = {0};
    int kill_stat = 0;

/*DNR*/
    resucicate = 0;
    assert(g_watchdog_pid != 0);
    kill_stat = kill(g_watchdog_pid,SIGUSR2);

    EXIT_IF_FAIL(kill_stat == -1,"kill error", -1);

/*free sync objects*/
    pthread_mutex_destroy(&mutex);

/* return to previous masking settings*/
 
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGUSR1);
    sigaddset(&sigset,SIGUSR2);
    pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);

    
    return 0;
}
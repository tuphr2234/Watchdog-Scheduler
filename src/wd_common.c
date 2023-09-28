#define _POSIX_C_SOURCE 200812L
#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <semaphore.h> /*sem_t*/
#include <assert.h> /*assert*/
#include <sys/wait.h> /*wait*/
#include <errno.h> /*errno*/

#include "aux_funcs.h"
#include "scheduler.h"
#include "watchdog.h"
#include "wd_utils.h"

#define UNUSED(x) ((void)(x))

int g_signal_missed = 0; /*signals missed counter */
int g_sending_pid = 0;
int g_res = 1; /* resuscicate indicator */
int usr_pid = 0;
extern int g_watchdog_pid;

static void SignalFunction(int nothing, siginfo_t *info, void *op )
{
	UNUSED(nothing);
	UNUSED(op);
    g_sending_pid = info->si_pid;
    g_signal_missed = 0;

    DEBUG_ONLY(printf("signal recieved from %d my pid : %d\n", g_sending_pid ,  getpid()));

    g_watchdog_pid = g_sending_pid;

}

static void DNRSignalFunction(int nothing, siginfo_t *info, void *op )
{
	UNUSED(nothing);
	UNUSED(op);
    UNUSED(info);
    g_res = 0;
    g_signal_missed = -1;
 
}

void InitSignalHandlers()
{
    static struct sigaction sigHandler = {0};
    static struct sigaction DNRsigHandler = {0};

    sigHandler.sa_sigaction = SignalFunction;
    sigHandler.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1 , &sigHandler , NULL );
    
    DNRsigHandler.sa_sigaction = DNRSignalFunction;
    DNRsigHandler.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2 , &DNRsigHandler , NULL );

}

int FinishWatchdogs(argus_t* argus)
{
    int kill_stat = 0;
    DEBUG_ONLY(printf("FinishWatchdogs%d\n", getpid()));
    
    do{
        kill_stat = kill(g_sending_pid,SIGUSR2);
    }
    while(errno != ESRCH && -1 != kill_stat);

    if(argus->is_usr)
    {
        DEBUG_ONLY(printf("BYE-BYE user-watchdog\n"));
    }    
    else
    {
        DEBUG_ONLY(printf("BYE-BYE WATCHDOG\n"));
    }
    
    return 1;
}


void resuscitate(int is_usr_proc ,  char *argv[])
{
    pid_t pid = 0;
    char *cmd = NULL;

    if(is_usr_proc)/*user proccess (watchdog thread) reinitiate watchdog proccess*/
    {
        cmd = "bin/release/watchdog";
        #ifndef NDEBUG
        cmd = "bin/debug/watchdog";
        #endif 
        pid = fork();
    }

    else/*watchdog proccess reinitiate user proccess*/
    {
        int kill_stat = 0;
        DEBUG_ONLY(printf("watchdog resuscicating...\n"));
        
        /*if user proccess exists:*/
        if(g_sending_pid != getppid())
        {
            do{
            kill_stat = kill(g_sending_pid,SIGKILL);
            }
            while(errno != ESRCH && -1 != kill_stat);
        }

        cmd = argv[0];
    }

    if(pid == 0)
    { 
        execv(cmd ,argv);

        perror("execv error");
        exit(1);
    }
    else
    {
        if(is_usr_proc)
            g_sending_pid = pid;
    }

}

int WaitForSignalTask(void *argus)
{
    char *argv[20];
    int kill_stat = 0;
    argus_t *arguss = (argus_t*)argus;
    
    int i =0;
    char freq_arg[10];
    char fail_count_arg[10];

    argv[0] = (arguss->argv)[0];

    while(i < arguss->argc)
    {
        argv[i] = arguss->argv[i];
        i++;
    }
   
    sprintf(freq_arg , "%d", arguss->frequency);
    sprintf(fail_count_arg , "%d", arguss->fail_count);

    argv[arguss->argc] = freq_arg ;
    argv[(arguss->argc)+1] = fail_count_arg;
    
    DEBUG_ONLY(printf("proccess : %d missed %d signals\n",getpid(),g_signal_missed ));

    if(!g_res)
    {
        int out = FinishWatchdogs(arguss);
        return out;
    }

    if(!((arguss)->is_usr))
    {
        g_sending_pid = getppid();
    }
    if(g_signal_missed == (arguss)->fail_count)
    {
        if(g_res)
        {
            resuscitate((arguss)->is_usr,argv);
        }
        else
        {
            int out = FinishWatchdogs(arguss);
            return out;
        }

        g_signal_missed = 0;
    }
    
    DEBUG_ONLY(printf("%d ------> %d\n",getpid(),g_sending_pid ));

    ++g_signal_missed;
    kill_stat = kill(g_sending_pid,SIGUSR1);

    EXIT_IF_FAIL(-1 == kill_stat,"kill failed",-1);
    
    return 0;
} 

void AddAndRunScheduler(argus_t *argus)
{
    scheduler_t *scheduler = SchedCreate();
    argus->scheduler = scheduler;

    SchedAdd(scheduler,WaitForSignalTask , argus , argus->frequency);
    SchedRun(scheduler);

    SchedDestroy(argus->scheduler);
}












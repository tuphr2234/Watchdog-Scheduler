#include "watchdog.h"
#include "scheduler.h"

typedef struct argus{
   
    char *argv[20]; /*user arguments*/
    int argc; /*user arguments num*/
    int frequency;
    int fail_count;
    sem_t *sem;
    int is_usr;
    pthread_mutex_t *mutex;
    char *freq_args;
    char *fail_count_args;
    scheduler_t *scheduler;

}argus_t;

/*loads argus struct with argc arguments*/
void LoadArgus(argus_t *argus, int argc ,  char *argv[]);

/*loads argv with argus arguments*/
void LoadArgv(char *argv[],argus_t *argus);

/*resuscitate watchdog*/
void resuscitate(int is_usr_proc ,  char *argv[]);

/*
scheduler function
argus us of type argus_t* 
*/
int WaitForSignalTask(void *argus);

void AddAndRunScheduler(argus_t *argus);

void InitSignalHandlers();

int FinishWatchdogs(argus_t* argus);
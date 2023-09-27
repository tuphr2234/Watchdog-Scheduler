#include "uid.h"

typedef struct task task_t;

/*
Getters
*/
Uid_t TaskGetUid(task_t *task);
op_func_t TaskGetFunc(task_t *task);
void *TaskGetData(task_t *task);
size_t TaskGetInterval(task_t *task);
time_t TaskGetExeTime(task_t *task);

/*
Setters
*/
void TaskSetData(task_t *task , void *data);
void TaskSetInterval(task_t *task , size_t interval);
void TaskSetExeTime(task_t *task , time_t time);
void TaskSetUid(task_t *task , Uid_t uid);


/*
Creates a new task with a function and argument, and an interval for 
the scheduler.
*/
task_t *TaskCreate(void *data , op_func_t op , size_t interval);

void TaskDestroy(task_t *task);


/*
Execute task
*/
int TaskExecute(task_t *task);



int TaskIsEqual_Destroy(task_t *task1 , task_t *task2);



#include <stdlib.h>  /* size_t malloc */
#include <time.h>	/* time_t  */


#include <stdio.h>	/*printf*/
#include <assert.h> /*assert*/


#include "pqueue.h"    /* all PQueue functions */
#include "scheduler.h"
#include "task.h"	   /* all Task function - assisting module */

struct scheduler
{
	p_queue_t *pqueue;
	task_t *cur_task;
	int self_destruct;
	int is_running;
};


/*******************************************************************************
Sched's queue priority fucntion.
*******************************************************************************/
static int Priority(const void* task1, const void* task2)
{
	task_t *t1 = (task_t*)task1;
	task_t *t2 = (task_t*)task2;

	return (TaskGetExeTime(t1) + TaskGetInterval(t1) - (TaskGetExeTime(t2) + TaskGetInterval(t2)));
	
}



/*******************************************************************************
Description:     	Creates an empty Sched.
Return value:    	Pointer to Sched in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'SchedDestroy()' at end of use.
*******************************************************************************/
scheduler_t *SchedCreate(void)
{
	scheduler_t *Sched = (scheduler_t*)malloc(sizeof(scheduler_t));
	
	if(NULL ==Sched)
	{
		return NULL;
	}
	
	Sched->pqueue = PQueueCreate(Priority);
	
	if(NULL == Sched->pqueue)
	{
		free(Sched);
		Sched = NULL;
		
		return NULL;
	}
	
	Sched->is_running = 0;
	Sched->self_destruct = 0;
	
	return Sched;

}


/*******************************************************************************
Description:     	Deletes a Sched pointed by 'scheduler_t' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if Sched is NULL.
*******************************************************************************/
void SchedDestroy(scheduler_t *Sched)
{
	assert(Sched);
	
	SchedClear(Sched);
	PQueueDestroy(Sched->pqueue);
	free(Sched);
	Sched = NULL;
}


/*******************************************************************************
Description:     	Add 'task' to Sched pointed by 'Sched'.
Return value:    	0 for success, 1 for fail.         
Time Complexity: 	
Notes:              Undefined behaviour if Sched is invalid pointer.
*******************************************************************************/
Uid_t SchedAdd(
    scheduler_t *Sched,
    int (*op_func)(void *params),
    void *params,
    size_t interval
)
{
	task_t *task = TaskCreate(params , op_func, interval);
	
	PQueueEnqueue(Sched->pqueue,task);

	return TaskGetUid(task);
}


/*******************************************************************************
Auxiliary function for SchedRemove
*******************************************************************************/
static int TaskIsEqual_Destroy_Func(const void *task1 , const void *task2)
{
	return TaskIsEqual_Destroy( (task_t*)task1 , (task_t*)task2 );
}


/*******************************************************************************
Description:     	remove 'task' to Sched pointed by 'Sched'.
Return value:    	0 for success, 1 for fail.         
Time Complexity: 	
Notes:              Undefined behaviour if Sched is invalid pointer.
*******************************************************************************/
int SchedRemove(scheduler_t *Sched, Uid_t uid_to_remove)
{

	task_t *task_to_remove = NULL;
	int success = 0;
	assert(Sched);

	
	/*self-destruct*/
	if( UidIsEqual( TaskGetUid(Sched->cur_task) , uid_to_remove))
	{
		Sched->self_destruct = 1;
		return 1;
	}
	
	task_to_remove = TaskCreate(NULL , NULL , 0);
	TaskSetUid(task_to_remove,uid_to_remove);
	
	success = PQueueErase(Sched->pqueue, TaskIsEqual_Destroy_Func, task_to_remove);
	
	TaskDestroy(task_to_remove);

	return success;

}


static size_t WaitForExecutionTime(task_t *task)
{
	int sleep_time = 0;
	assert(task);
	
	sleep_time = (difftime(TaskGetExeTime(task)+ TaskGetInterval(task),time(NULL)));
	
	

	sleep_time = (sleep_time > 0) * sleep_time;
	
	while(sleep_time)
	{
		sleep_time = sleep(sleep_time);
	}
	
	return 0;
}


/*******************************************************************************
Description:     	run all tasks in Sched pointed by 'Sched'.        
Time Complexity: 	
Notes:              MUST call SchedStop() before calling SchedRun().
                    Undefined behaviour if Sched is invalid pointer.
*******************************************************************************/
int SchedRun(scheduler_t *Sched)
{
	task_t *task = NULL;
	int task_output = 0;
	size_t interval = 0;
	size_t exe_time = 0;

	
	assert(Sched);
	
	Sched->is_running = 1;
	
	while(!PQueueIsEmpty(Sched->pqueue) && Sched->is_running)
	{
		
/*Peek and Dequeue*/		
		task = PQueuePeek(Sched->pqueue);
		Sched->cur_task = task;
		PQueueDequeue(Sched->pqueue);
		
/*Execute*/		
		WaitForExecutionTime(task);
		task_output = TaskExecute(task);
		
		
/*Prepare next execution*/
		
		if(Sched->self_destruct == 1)
		{
			TaskDestroy(Sched->cur_task);
			Sched->cur_task = NULL;
		}
		
		else if(0 == task_output)
		{
			interval = TaskGetInterval(task);
			exe_time = TaskGetExeTime(task);
			TaskSetExeTime(task,interval+exe_time);
			PQueueEnqueue(Sched->pqueue , task);
		}
		else
		{
			TaskDestroy(task);
			task = NULL;
		}
	}
	
	Sched->is_running = 0;
	
	return task_output;
	
}


/*******************************************************************************
Description:     	stops the run of all tasks in Sched pointed by
                    'Sched'.   
Time Complexity: 	
Notes:              Undefined behaviour if Sched is invalid pointer.
*******************************************************************************/
void SchedStop(scheduler_t *Sched)
{
	assert(Sched);
	Sched->is_running = 0;

}


/*******************************************************************************
Description:     	Returns number of elements in 'Sched'
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if Sched is invalid pointer
*******************************************************************************/
size_t SchedSize(const scheduler_t *Sched)
{
	assert(Sched);
	
	return PQueueSize(Sched->pqueue) + Sched->is_running;
}


/*******************************************************************************
Description: 		Checks if 'Sched' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if Sched is invalid pointer.
*******************************************************************************/
int SchedIsEmpty(const scheduler_t *Sched)
{
	assert(Sched);
		
	return ( PQueueIsEmpty(Sched->pqueue) && !Sched->is_running );	
		
}


/*******************************************************************************
Description: 		remove all tasks from 'Sched'.
Time complexity:  	O(n)
Note: 				Undefined behaviour if Sched is invalid pointer.
*******************************************************************************/
void SchedClear(scheduler_t *Sched)
{
	task_t *task = NULL;
	assert(Sched);
	
	while(!PQueueIsEmpty(Sched->pqueue))	
	{
		task = PQueuePeek(Sched->pqueue);
		TaskDestroy(task);
		task = NULL;
		PQueueDequeue(Sched->pqueue);
	}
	
}


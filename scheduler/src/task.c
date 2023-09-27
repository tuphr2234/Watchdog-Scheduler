#include <stdlib.h> /* malloc, size_t , free*/
#include <time.h>

#include "uid.h" /*UidCreate , Uid_t*/
#include "scheduler.h"
#include "task.h"


struct task
{
	Uid_t uid;
	op_func_t op_func;
	void *data;
	size_t interval;
	time_t exe_time;

};


/****************************************************************************/
/*Getters*/

Uid_t TaskGetUid(task_t *task)
{
	return task->uid;
}

op_func_t TaskGetFunc(task_t *task)
{
	return task->op_func;
}

void *TaskGetData(task_t *task)
{
	return task->data;
}


size_t TaskGetInterval(task_t *task)
{
	return task->interval;
}

time_t TaskGetExeTime(task_t *task)
{
	return task->exe_time;
}


/****************************************************************************/
/*Setters*/


void TaskSetData(task_t *task , void *data)
{
	 task->data = data;
}


void TaskSetInterval(task_t *task , size_t interval)
{
	 task->interval = interval;
}

void TaskSetExeTime(task_t *task , time_t time)
{
	 task->exe_time = time;
}

void TaskSetUid(task_t *task , Uid_t uid)
{
	task->uid = uid;
}


/****************************************************************************/


task_t *TaskCreate(void *data , op_func_t op , size_t interval)
{
	task_t *task = (task_t*)malloc(sizeof(task_t));
	
	if(NULL == task)
	{
		return NULL;
	}
	
	task->uid = UidCreate();
	
	if( UidIsEqual(task->uid , UID_BAD) )
	{
		free(task);
		task = NULL;
		return NULL;
	}
	
	task->op_func = op;
	task->data = data;
	
	task->exe_time = time(NULL);
	
	if(-1 == task->exe_time)
	{
		free(task);
		task = NULL;
		
		return NULL;
		
	}
	
	task->interval = interval;
	
	return task;

}

void TaskDestroy(task_t *task)
{
	free(task);
	task = NULL;
}



int TaskExecute(task_t *task)
{
	return task->op_func(task->data);
}


int TaskIsEqual_Destroy(task_t *task1 , task_t *task2)
{

	if(UidIsEqual(task1->uid , task2->uid))
	{
		TaskDestroy(task1);
		task1 = NULL; 
		return 1;
	}
	
	return 0;
}











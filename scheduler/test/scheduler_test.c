#include <stdio.h>		/*printf*/
#include <assert.h> 	/*assert*/
#include <stdlib.h> /*	malloc		*/
#include "scheduler.h"

#define SIZE 5

static void Test();
static int Stop(void *scheduler);
static int AddTask(void *scheduler);

Uid_t global_uid;

size_t counter = 0;
int ex_interval = 11;


int main()
{
	Test();

	return 0;
}


static int PrintStr(void *num)
{
	counter++;
	printf("%d - %ld \n" , *(int*)num , counter);
	
	return 0;
}

static int Stop(void *scheduler)
{
	counter++;
	SchedStop((scheduler_t *)scheduler);	
	
	printf("STOPPED - %ld\n" , counter);
	return 1;
}


static int AddTask(void *scheduler)
{
	counter++;
	printf("task_adds_task - %ld\n" , counter);
	global_uid = SchedAdd((scheduler_t *)scheduler,PrintStr,&ex_interval,ex_interval);
	
	return 1;
}

static int RemoveTask(void *scheduler)
{
	counter++;
	printf("task_remove- %ld\n" , counter);
	SchedRemove((scheduler_t *)scheduler, global_uid);
	return 1;
}

typedef struct wrapper
{
	scheduler_t *scheduler;
	Uid_t *uid;

}wrapper_t;


static wrapper_t *CreateWrapper(scheduler_t *scheduler, Uid_t *uid )
{
	wrapper_t *wrapper = (wrapper_t*)malloc(sizeof(wrapper_t));
	wrapper->scheduler = scheduler;
	wrapper->uid = uid;
	return wrapper;
}

static int SelfDestructTask(void *wrapper)
{
	counter++;
	printf("SelfDestructTask - %ld\n" , counter);
	SchedRemove(((wrapper_t *)wrapper)->scheduler, *(((wrapper_t *)wrapper)->uid));
	return 1;
}




static void Test()
{
	int interval_arr[SIZE] = {5,3,6,9,4}; /*3 4 5 6 9*/

	Uid_t uid_self_destruct;
	int i = 0;
	
	wrapper_t *wrapper = NULL;
	
	scheduler_t *scheduler = SchedCreate();
	
	for(i = 0 ; i < SIZE ; i++)
	{
		SchedAdd(  scheduler,PrintStr,interval_arr+i,interval_arr[i]);
	}
	
	assert( SIZE == SchedSize(scheduler));
	
/*Add and remove funcs*/	
	SchedAdd(  scheduler,AddTask,scheduler,1);
	SchedAdd(  scheduler,RemoveTask,scheduler,13);
	
	
/*Self-Destruct func*/
	wrapper = CreateWrapper(scheduler,  &uid_self_destruct );
	uid_self_destruct = SchedAdd(  scheduler,SelfDestructTask,wrapper,30);
	wrapper->uid = &uid_self_destruct;
	
/*Stop func*/
	SchedAdd(  scheduler,Stop,scheduler,35);
	assert(!SchedIsEmpty(scheduler));

/*  RUN  */
	SchedRun(scheduler);
		
/*IsEmpty and Clear test*/	
	assert(!SchedIsEmpty(scheduler));
	SchedClear(scheduler);
	assert(SchedIsEmpty(scheduler));
	
/*free and destroy*/	
	free(wrapper);
	SchedDestroy(scheduler);
	
}



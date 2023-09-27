/* in .c */
    
#include <stdlib.h>  /*size_t , malloc*/
#include <assert.h>  /*assert*/
 
#include "vector.h" /*all vector functions*/
#include "heap.h"

static void Swap(size_t *a, size_t *b)
{
	size_t temp = *a;
	*a = *b;
	*b = temp;
}

struct heap
{
	vector_t *vector;
    is_before_t is_before_func;
};


/*******************************************************************************
Description:     	Creates an empty heap, according to 'is_before_func'.
Return value:    	Pointer to heap in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'HeapDestroy()' at end ofuse.
*******************************************************************************/
heap_t *HeapCreate(is_before_t is_before_func)
{
	heap_t *heap = (heap_t*)malloc(sizeof(heap_t));
	if(NULL == heap)
	{
		return NULL;
	}
	heap->vector = VectorCreate(5,sizeof(void*));
	if(NULL == heap->vector)
	{
		free(heap);
		return NULL;
	}

	heap->is_before_func = is_before_func;
	
	return heap;
}
/*******************************************************************************
Description:     	Deletes a heap pointed by 'heap' from memory.
Time Complexity: 	O(1) 
Notes:           	Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
void HeapDestroy(heap_t *heap)
{
	assert(heap);
	
	VectorDestroy(heap->vector);
	free(heap);heap=NULL;
}


static void	HeapifyUp(heap_t *heap)
{
	size_t index = VectorSize(heap->vector)-1;
	size_t *first = VectorGetAccess(heap->vector,index);
	size_t *next = VectorGetAccess(heap->vector,((index+1)/2)- (index !=0));

	while(index)
	   {
	   		first = VectorGetAccess(heap->vector,index);
	   		next = VectorGetAccess(heap->vector,((index+1)/2)- (index !=0));
	   	
	   		if(heap->is_before_func(*(void**)first, *(void**)next))
	   		{
	   			Swap(first,next);
			}
			
	  	    index = ((index+1)/2)- (index !=0) ;
	   }
}


static void	HeapifyDown(heap_t *heap, size_t index)
{
	while((index*2)+2 < VectorSize(heap->vector))
	{

		size_t *first = VectorGetAccess(heap->vector,index);
		size_t *next = VectorGetAccess(heap->vector,(index*2)+1);
		size_t *next2 = VectorGetAccess(heap->vector,(index*2)+2);

		if(heap->is_before_func(*(void**)next,*(void**)next2))
		{
			Swap(first,next);
			index = (index*2)+1;
		}
		else 
		{
			Swap(first,next2);
			index = (index*2) + 2;
		}
	}	
}


/*******************************************************************************
Description:     	Add 'data' to heap pointed by 'heap'.
Return value:    	0 for success, 1 for fail.         
Time Complexity: 	Amortized O(log(n)).
Notes:              Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
int HeapPush(heap_t *heap, void *data)
{
	assert(heap);
	{
	int success = VectorPushBack(heap->vector,&data);
	
	HeapifyUp(heap);
	
	return success;
	}
}


/*******************************************************************************
Description:       Deletes the first element in 'heap'.
Time Complexity:   O(log(n))
Notes:             Undefined behaviour if 'heap' is invalid pointer.          
*******************************************************************************/
void HeapPop(heap_t *heap)
{
	assert(heap);
	
	{
	size_t index = VectorSize(heap->vector)-1;
	
	size_t *indexp = VectorGetAccess(heap->vector,index);
	size_t *firstp = VectorGetAccess(heap->vector,0);
	
	Swap(indexp , firstp);

	VectorPopBack(heap->vector);
	HeapifyDown(heap,0);
	
	}
	

}


/*******************************************************************************
Description:   		Returns data of the first element in 'heap'. 
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
void *HeapPeek(const heap_t *heap)
{
	assert(heap);
	
	return (void*)*((void**)(VectorGetAccess(heap->vector,0)));
}


/*******************************************************************************
Description: 		Checks if 'heap' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	
	return VectorSize(heap->vector) == 0;
}


/*******************************************************************************
Description:     	Returns number of elements in 'heap'
Time Complexity: 	O(1)
Notes:			 	Undefined behaviour if 'heap' is invalid pointer
*******************************************************************************/
size_t HeapSize(const heap_t *heap)
{
	assert(heap);
	
	return VectorSize(heap->vector);
}


/*******************************************************************************
Description:  	  	Erase the element with 'data' found by 'is_before_func' from
                    a heap pointed by 'heap'.
Return value 		0 for success otherwise 1.
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
int HeapRemove(heap_t *heap, is_match_func_t is_match, void *param)
{
	assert(heap);
	{
	size_t size = HeapSize(heap);
	size_t i = 0;
	size_t *indexp = NULL;
	size_t *lastp =  VectorGetAccess(heap->vector,size - 1);
	
	for(i = 0 ; i < size ; i++)
	{
		if(is_match(*(void**)VectorGetAccess(heap->vector,i),param))
		{
			indexp = VectorGetAccess(heap->vector,i);
			
			Swap(indexp ,lastp);	
			VectorPopBack(heap->vector);	
			HeapifyDown(heap,i);

			return 0;	
		}
	}
	
	return 1;	
	
	}
}



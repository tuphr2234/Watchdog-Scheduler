
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>  /* size_t */

typedef struct Vector vector_t;


/*******************************************************************************
description:  create a vector of size "elem_size" and type "element_num".
arguments:    elem_size in bytes 
return value:     pointer to vector in case of success, otherwise NULL
Time Complexity: determined by system call complexity. 
Note: should call "VectorDestroy()" at end of use
*******************************************************************************/
vector_t *VectorCreate(size_t element_num, size_t elem_size);


/*******************************************************************************
description:  delete a vector pointed by "VectorCreate" from memory.
Time Complexity: determined by system call complexity. 
Notes : undefined behaviour in case of invalid vector pointer 

*******************************************************************************/
void VectorDestroy(vector_t *VectorCreate);

/*******************************************************************************
description:  add "element" to the end in vector pointed by "vector".
Time Complexity: O(1)
Notes: undefined behaviour in case of full vector.
*******************************************************************************/
int VectorPushBack(vector_t *vector, void *element);

/*******************************************************************************
description:  deletes the last element in vector pointed by "vector".
Time Complexity: O(1)
Notes: undefined behaviour in case of empty vector.
*******************************************************************************/
void VectorPopBack(vector_t *vector);


/*******************************************************************************
description:  return the 'index' element in "vector"
Notes: undefined behaviour in case of index out of bounds.
Time Complexity: O(1)
*******************************************************************************/
void *VectorGetAccess(const vector_t *vector , size_t index);


/*******************************************************************************
description:  return number of occupied elements in "vector"
Time Complexity: O(1)
*******************************************************************************/
size_t VectorSize(const vector_t *vector);


/*******************************************************************************
description:  checks if "vector" is empty.
return value : 1 if true, 0 if false
time complexity: O(1)
*******************************************************************************/
int VectorIsEmpty(const vector_t *vector);


/*******************************************************************************
description:  returns total number of element in vector.
Time complexity: O(1)
*******************************************************************************/
size_t VectorCapacity(const vector_t *vector);



/*******************************************************************************
description:  Reserves 'new_capacity' size of elements to 'vector'
return value : 1 for success, 0 for failure
time complexity: O(new_capacity)
*******************************************************************************/
int VectorReserve(vector_t *vector , size_t new_capacity);


/*******************************************************************************
description:  shrinks vector's allocated mem to size : so v->size == v->capacity
return value : 1 for success, 0 for failure
time complexity: O(v->size)
*******************************************************************************/
int VectorShrinkToFit(vector_t *vector);



#endif    /*__VECTOR_H__*/

#include <stdio.h> /*		printf		*/
#include <stdlib.h>/*	size_t , malloc	*/
#include <string.h>/*		strcmp		*/
#include <assert.h>/*		assert		*/
#include "../include/vector.h"/*vector.h*/
#define GROWTH_FACTOR 2

struct Vector
{
	size_t size;
	size_t capacity;
	size_t elem_size;
	void *data;
};


vector_t *VectorCreate(size_t element_num, size_t element_size)
{

	vector_t *vector;
	vector = (vector_t*)malloc(sizeof(vector_t));
	
	if(NULL == vector)
	{
		return NULL;
	}
	
	vector->data = malloc((element_size*element_num));
	
	if(NULL == vector->data)
	{
		free(vector);
		return NULL;
	}
	
	vector->elem_size = element_size;
	vector->capacity = element_num;
	vector->size = 0;
	
	return vector;
}

void VectorDestroy(vector_t *vector)
{
	free(vector->data);
	vector->data = NULL;
	free(vector);
	vector = NULL;
}

void VectorPopBack(vector_t *vector)
{
	assert(vector);
	vector->size --;
}


int VectorPushBack(vector_t *vector , void *var)
{

	int success = 0;
	size_t elmSize = vector->elem_size;
	size_t vectSize = vector->size;
	char *vect_ptr = NULL;

	assert(NULL != vector);
	assert(NULL != vector->data);
	assert(NULL != var);
	
	vect_ptr = (char*)vector->data;	
	(vector->size)++;
	
	if(vector->capacity == vector->size)
	{
		success = VectorReserve( vector, (vector->capacity) * GROWTH_FACTOR );
	}
	
	vect_ptr = (char*)vector->data;
	memcpy(vect_ptr + ((vectSize)*elmSize) ,var,elmSize);
	

	return success;
}


void *VectorGetAccess(const vector_t *vector , size_t index)
{
	
	char *tempVect = NULL;
	
	assert(NULL != vector);
	assert(NULL != vector->data);
	assert(index < vector->size);
	
	tempVect = (char*)(vector->data);
	
	if(0 == vector->size)
	{
		return NULL;
	}
	
	tempVect+= index*(vector->elem_size);
	
	
	return tempVect;
}


size_t VectorSize(const vector_t *vector)
{
	assert(vector);
	return vector->size;
}


int VectorIsEmpty(const vector_t *vector)
{
	assert(vector);
	return vector->size == 0;
}


size_t VectorCapacity(const vector_t *vector)
{
	assert(vector);
	return vector->capacity;
}


int VectorReserve(vector_t *vector , size_t new_capacity)
{

	size_t element_size = vector->elem_size;
	assert(vector);
	assert(vector->data);
	
	vector->capacity = new_capacity; /*(vector->capacity) * GROWTH_FACTOR;*/
	vector->data = realloc(vector->data , new_capacity * element_size);

	
	if(vector->data == NULL)
	{
		printf("REALLOC FAILED");
		return 0;
	}
	
	return 1;
}

int VectorShrinkToFit(vector_t *vector)
{
	assert(vector);
	return VectorReserve(vector , vector->size);

}












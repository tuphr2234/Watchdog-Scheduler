
#ifndef __UID_H__
#define __UID_H__

#include <stddef.h> /* size_t */
#include <time.h>   /* time_t */
#include <unistd.h> /* pid_t  */
#include <sys/types.h> /*pid_t*/
#define IP_SIZE 16

typedef struct Uid 
{
    size_t count;
    pid_t pid;
    time_t timestamp;
    char ip[IP_SIZE];
}Uid_t;


extern const Uid_t UID_BAD;

/******************************************************************************
Description:     	Creates an UID.
Return value:    	uid_t on success, UID_BAD on fail.
Time Complexity: 	O(1).         	
******************************************************************************/
Uid_t UidCreate();

/******************************************************************************
Description:     	Compares between two UIDs.
Return value:    	1 for true, 0 for false.
Time Complexity: 	O(1).
******************************************************************************/
int UidIsEqual(Uid_t uid1, Uid_t uid2);

#endif    /*__UID_H__*/

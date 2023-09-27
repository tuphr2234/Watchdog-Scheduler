
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <time.h>   	/* time_t 			*/
#include <unistd.h> 	/* pid_t  			*/
#include <ifaddrs.h>	/* ifaddrs 			*/
#include <string.h>		/*strcmp			*/
#include <stddef.h>		/*size_t			*/
#include <sys/types.h>  /*vgetifaddrs		*/
#include <netinet/in.h> /*INET_ADDRSTRLE	*/
#include <arpa/inet.h>  /*inet_ntop			*/
#include <pthread.h>
#include <omp.h>
#include <stdlib.h>
#include <semaphore.h>


#include "uid.h"

#define IP_SIZE 16

static char* get_ipv4_address(char *addressBuffer) ;

const Uid_t UID_BAD = {0, 0, 0, ""};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Uid_t UidCreate()
{
    Uid_t u;
	static int count=0;

	pthread_mutex_lock(&mutex);
    
	u.pid = getpid();
	u.timestamp = time(NULL);
	u.count = count;
	
	get_ipv4_address(u.ip); 
	
	if(!strcmp(u.ip,"failed"))
	{
		return UID_BAD ;
	}

	count++;
	
    pthread_mutex_unlock(&mutex);

	return u;

}

static char* get_ipv4_address(char *addressBuffer) 
{
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;
	int fail = 0;
    fail = getifaddrs(&ifAddrStruct);
    
    if(fail)
    {
    	return "failed";
    }

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (!ifa->ifa_addr) 
        {
            continue;
        }
        /* check it is IP4 (AF_INET)*/
        if (ifa->ifa_addr->sa_family == AF_INET) 
        { 
           
            /* is a valid IP4 Address*/
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
           
            if (0 != strcmp(ifa->ifa_name, "lo")) 
            {
            	freeifaddrs(ifAddrStruct);
                return addressBuffer;
            } 
        } 
    }
    
    return addressBuffer;
}

int UidIsEqual(Uid_t Uid1, Uid_t Uid2)
{
	return Uid1.pid == Uid2.pid && Uid1.count == Uid2.count && 
	Uid1.timestamp == Uid2.timestamp && !strcmp(Uid1.ip , Uid2.ip);
}

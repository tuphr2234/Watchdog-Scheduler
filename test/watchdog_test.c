#include <stdio.h> /*printf*/
#include <unistd.h>/*sleep*/
#include <dlfcn.h> /*shared object*/

#include "watchdog.h"


int main(int argc, char const *argv[])
{
   size_t i = 0;
   size_t max = 30;

    MakeMeImmortal( argc,( char **)argv,  2,  6);
    
    printf("start loop\n");
    while(i < max )
    {
        ++i;
        printf("usr program : %ld\n", i);

        sleep(1);
    }
    printf("end loop\n");

    printf("Exit?\n");
    DoNotResuscitate();

    printf("-------------------DoNotResuscitate was called----------------------\n");
    i = 0;

    printf("usr program after DNR : %ld\n", i);


    return 0;
}

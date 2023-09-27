
/*
 call this function to make the process immortal
    @param argc: number of arguments passed to the main function
    @param argv: arguments passed to the main function
    @param frequency: frequency of the watchdog counter
    @param fail_count: number of times the watchdog can fail before it stops resuscitating the process
*/
int MakeMeImmortal(int argc,char *argv[], int frequency, int fail_count);

/*
call this function to stop the watchdog from resuscitating the process
*/
int DoNotResuscitate();

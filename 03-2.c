#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t p = fork();
    if (p == 0)
	printf("I'm child process with P = %d\nMy parent's P = %d\n", (int)getpid(), (int)getppid());
    else if (p == -1)
	printf("Something went wrong, can't create child process\n");
    else
    {
	printf("I'm parent with P = %d\nMy child's PID = %d\n", (int)getpid(), (int)pid);
	sleep(1);
    }
    return 0;
}

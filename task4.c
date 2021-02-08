#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main () {
    pid_t pid, ppid;
    pid = fork();
    if (pid == 0) {
        printf("child with ppid: %d\n", getppid());
        execl("./3", "arg1", "arg2", "arg3", "arg4", (char *)0);
    } else if (pid == -1) {
        printf("cheers, you have an exception");
        return 1;
    } else {
        prinf("parent with pid: %d\n", getpid());
    }
    return 0;
}
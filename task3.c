#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[], char *envp[]) {
    printf("number of arguments = %d:\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    printf("environment property:\n");
    for (int i = 0; envp[i] != 0; ++i) {
        printf("envp[%d] = %s\n", i, envp[i]);
    }
    return 0;
}
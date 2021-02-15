#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int res;
    int fd1[2];
    int fd2[2];
    size_t size1;
    size_t size2;
    char resstr1[26];
    char resstr2[27];
    if (pipe(fd1) < 0) {
        printf("First pipe could not be opened\n");
        exit(-1);
    }
    if (pipe(fd2) < 0) {
        printf("Second pipe could not be opened\n");
        exit(-1);
    }
    res = fork();
    if (res < 0) {
        printf("Can not fork child\n");
        exit(-1);
    }
    else if (res > 0) 
    {
        if (close(fd1[0]) < 0) {
            printf("Can not close reading first pipe in Parent\n"); 
            exit(-1);
        }
        size1 = write(fd1[1], "Parent process", 27);
        if (size1 != 27) {
            printf("Can not write symbols to first pipe\n");
            exit(-1);
        }
        if (close(fd1[1]) < 0) {
            printf("Can not close writing first pipe in Parent\n");
            exit(-1);
        }
        printf("Parent exit\n");
        if (close(fd2[1]) < 0) {
            printf("Can not close writing second pipe in Child\n");
            exit(-1);
        }
        size2 = read(fd2[0], resstr1, 26);
        if (size2 < 0) {
            printf("Can not read string from second pipe\n");
            exit(-1);
        }
        printf("Parent exit, result string: %s\n", resstr1);
        if (close(fd2[0]) < 0) {
            printf("Can not close reading second pipe in Child\n"); 
            exit(-1);
        }
    }
    else 
    {
        if (close(fd1[1]) < 0) {
            printf("Can not close writing first pipe in Child\n"); 
            exit(-1);
        }
        size1 = read(fd1[0], resstr2, 27);
        if (size1 < 0) {
            printf("Can not read string from first pipe\n");
            exit(-1);
        }
        printf("Child exit, result string: %s\n", resstr2);
        if (close(fd1[0]) < 0) {
            printf("Can not close reading first pipe in Child\n");
            exit(-1);
        }
        if (close(fd2[0]) < 0) {
            printf("Can not close reading second pipe in Child\n"); 
            exit(-1);
        }
        size2 = write(fd2[1], "Child process!", 26);
        if (size2 != 26) {
            printf("Can not write symbols to second pipe\n");
            exit(-1);
        }
        if (close(fd2[1]) < 0) {
            printf("Can not close writing second pipe in Child\n");
            exit(-1);
        }
        printf("Child exit\n");
    }
    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main()
{
    /*
    * The semaphore is initialized with 1 at start.
    * Parent: D(0, 1) -> in for loop:
    *       write -> A(0, 1) -> Z -> D(0, 1) -> read
    * Child: in for loop:
    *       D(0, 2) -> read -> write -> A(0, 1)
    */
    int fd[2];
    int res;
    int count;
    int semid;
    key_t key;              
    size_t size;
    struct sembuf buf;
    char path[] = "Task3.c";
    char resstr[15];
    scanf("%d", &count);
    if (pipe(fd) < 0) {
        printf("Can not open pipe\n");
        exit(-1);
    }
    if ((key = ftok(path, 0)) < 0) {
        printf("Unable to generate key\n");
        exit(-2);
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Impossible to create semaphore\n");
        exit(-3);
    }
    buf.sem_num = 0;
    buf.sem_flg = 0;
    buf.sem_op = 1;
    if (semop(semid, &buf, 1) < 0) {
        printf("Cannot set initial semaphore value to 1\n");
        exit(-6);
    }
    res = fork();
    if (res < 0) {
        printf("Can not fork the child\n");
        exit(-5);
    }
    else if (res > 0) {
        buf.sem_num = 0;
        buf.sem_flg = 0;
        buf.sem_op = -1;
        if (semop(semid, &buf, 1) < 0) {
            printf("Can not decrease semaphore value by 1\n");
            exit(-6);
        }
        for (int i = 0; i < count; ++i) {
            size = write(fd[1], "Hello, child!", 14);
            if (size != 14) {
                printf("Parent: Can not write all characters to the pipe\n");
                exit(-1);
            }
            buf.sem_num = 0;
            buf.sem_flg = 0;
            buf.sem_op = 2;
            if (semop(semid, &buf, 1) < 0) {
                printf("Unable to enter the critical section correctly in program A\n");
                exit(-6);
            }
            buf.sem_num = 0;
            buf.sem_flg = 0;
            buf.sem_op = 0;
            if (semop(semid, &buf, 1) < 0) {
                printf("Can not enter the critical section correctly in program A\n");
                exit(-6);
            }
            buf.sem_num = 0;
            buf.sem_flg = 0;
            buf.sem_op = -1;
            if (semop(semid, &buf, 1) < 0) {
                printf("Can not enter the critical section correctly in program A\n");
                exit(-6);
            }
            size = read(fd[0], resstr, 15);
            if (size < 0) {
                printf("Unable to read line from pipe\n");
                exit(-1);
            }
            printf("Parent: read the line: %s\n", resstr);
        }
        printf("The parent finished his job\n");
    }
    else {
        for (int i = 0; i < count; ++i) {
            buf.sem_num = 0;
            buf.sem_flg = 0;
            buf.sem_op = -2;
            if (semop(semid, &buf, 1) < 0) {
                printf("Can not enter the critical section correctly\n");
                exit(-6);
            }
            size = read(fd[0], resstr, 14);
            if (size < 0) {
                printf("Child: Unable to read line from pipe\n");
                exit(-1);
            }
            printf("Child: read the line: %s\n", resstr);
            size = write(fd[1], "Hello, parent!", 15);
            if (size != 15) {
                printf("Child: impossible to write all the characters into the trumpet\n");
                exit(-1);
            }
            buf.sem_num = 0;
            buf.sem_flg = 0;
            buf.sem_op = 1;
            if (semop(semid, &buf, 1) < 0) {
                printf("Can not enter the critical section correctly\n");
                exit(-6);
            }
        }
        if (close(fd[0]) < 0) {
            printf("Child: impossible to close the reading side of the pipe\n");
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("Child: Unable to close the writing side of the pipe\n");
            exit(-1);
        }
    }
    return 0;
}

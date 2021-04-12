#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(int argc, char* argv[], char* envp[])
{
    int semid;
    char path[] = "09-1a.c";
    key_t key;
    struct sembuf buf;
    if ((key = ftok(path, 0)) < 0) {
        printf("Can not generate key\n");
        exit(-1);
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can not create semaphore set\n");
        exit(-1);
    }
    buf.sem_num = 0;
    buf.sem_flg = 0;
    buf.sem_op = 1;
    if (semop(semid, &buf, 1) < 0) {
        printf("Can not add 1 to semaphore\n");
        exit(-1);
    }
    printf("Condition is present\n");
    return 0;
}
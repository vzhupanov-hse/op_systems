#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

int main()
{
    int* arr;
    long i;
    int semid;
    int shmid;
    int n = 1;
    char path[] = "Task2_a.c";
    key_t key;
    struct sembuf mybuf;
    if ((key = ftok(path, 0)) < 0) {
        printf("Can not generate key\n");
        exit(-1);
    }
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Can not create shared memory\n");
            exit(-1);
        }
        else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Can not find shared memory\n");
                exit(-2);
            }
            n = 0;
        }
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Semaphore does not exist\n");
            exit(-3);
        }
        else {
            if ((semid = semget(key, 1, 0)) < 0) {
                printf("Unable to get semaphore by key\n");
                exit(-4);
            }
        }
    }
    else {
        mybuf.sem_num = 0;
        mybuf.sem_flg = 0;
        mybuf.sem_op = 1;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can not set original value of this semaphore to 1\n");
            exit(-5);
        }
    }
    if ((int*)(-1) == (arr = (int*)shmat(shmid, NULL, 0))) {
        printf("Unable to connect shared memory in program A\n");
        exit(-6);
    }
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    mybuf.sem_op = -1;
    if (semop(semid, &mybuf, 1) < 0) {
        printf("Unable to enter the critical section correctly in program A\n");
        exit(-7);
    }
    if (n) {
        arr[0] = 1;
        arr[1] = 0;
        arr[2] = 1;
    }
    else {
        arr[0] += 1;
        for (i = 0; i < 2000000000L; i++);
        arr[2] += 1;
    }
    printf
    ("Program A was created %d times, program B - %d times, total - %d times\n",
        arr[0], arr[1], arr[2]);
    mybuf.sem_num = 0;
    mybuf.sem_flg = 0;
    mybuf.sem_op = 1;
    if (semop(semid, &mybuf, 1) < 0) {
        printf("Unable to exit correctly from critical section in program A\n");
        exit(-8);
    }
    if (shmdt(arr) < 0) {
        printf("Unable to detach shared memory in program A\n");
        exit(-9);
    }
    return 0;
}

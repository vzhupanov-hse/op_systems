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
    int shmid;
    int semid;
    int n = 1;
    key_t key;
    struct sembuf buf;
    char path[] = "Task2_a.c";
    if ((key = ftok(path, 0)) < 0) {
        printf("Can not generate key\n");
        exit(-1);
    }
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Unable to create shared memory\n");
            exit(-2);
        }
        else {
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Can not find shared memory\n");
                exit(-3);
            }
            n = 0;
        }
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Semaphore does not exist.\n");
            exit(-4);
        }
        else {
            if ((semid = semget(key, 1, 0)) < 0) {
                printf("Unable to get semaphore by key\n");
                exit(-5);
            }
        }
    }
    else {
        buf.sem_num = 0;
        buf.sem_flg = 0;
        buf.sem_op = 1;
        if (semop(semid, &buf, 1) < 0) {
            printf("Can't initialize this semaphore to 1\n");
            exit(-6);
        }
    }
    if ((int*)(-1) == (arr = (int*)shmat(shmid, NULL, 0))) {
        printf("Unable to connect shared memory in program B\n");
        exit(-7);
    }
    buf.sem_num = 0;
    buf.sem_flg = 0;
    buf.sem_op = -1;
    if (semop(semid, &buf, 1) < 0) {
        printf("Can not enter the critical section correctly in program B\n");
        exit(-8);
    }
    if (n) {
        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 1;
    }
    else {
        arr[1] += 1;
        for (i = 0; i < 2000000000L; i++);
        arr[2] += 1;
    }
    printf
    ("Program A was created %d times, program B - %d times, total - %d times\n",
        arr[0], arr[1], arr[2]);
    buf.sem_num = 0;
    buf.sem_flg = 0;
    buf.sem_op = 1;
    if (semop(semid, &buf, 1) < 0) {
        printf("Unable to exit the critical section correctly in program B\n");
        exit(-8);
    }
    if (shmdt(arr) < 0) {
        printf("Unable to detach shared memory in program B\n");
        exit(-9);
    }
    return 0;
}
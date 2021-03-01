#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    int shmid;
    key_t key;
    char* txt;
    int* size_ptr;
    int size = 0;
    char path[] = "Task1-2.c";
    if ((key = ftok(path, 0)) < 0) {
        printf("Can not generate key\n");
        exit(-1);
    }
    if ((shmid = shmget(key, sizeof(int) + size * sizeof(char), 0666 | IPC_CREAT)) < 0) {
        printf("Can not create shared memory\n");
        exit(-1);
    }
    if ((size_ptr = (int*)shmat(shmid, NULL, 0)) == (int*)(-1)) {
        printf("Can not attach shared memory\n");
        exit(-1);
    }
    size = *size_ptr;
    txt = (char*)(size_ptr + 1);
    for (int i = 0; i < size; i++) {
        putchar(txt[i]);
    }
    if (shmdt(size_ptr) < 0) {
        printf("Can not detach shared memory\n");
        exit(-1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        printf("Can not delete shared memory\n");
        exit(-1);
    }
    return 0;
}
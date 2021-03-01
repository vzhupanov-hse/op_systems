#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int shmid;
    key_t key;
    FILE* file;
    char* text;
    int* size_ptr;
    int size = 0;
    char path[] = "Task1-2.c";
    file = fopen(path, "r");
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);
    if ((key = ftok(path, 0)) < 0) {
        printf("Can not generate code\n");
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
    *size_ptr = size;
    text = (char*)(size_ptr + 1);
    for (int i = 0; i < size; i++) {
        text[i] = fgetc(file);
    }
    fclose(file);
    if (shmdt(size_ptr) < 0) {
        printf("Can not detach shared memory\n");
        exit(-1);
    }
    return 0;
}
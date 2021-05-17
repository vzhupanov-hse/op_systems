#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <sys/types.h>

int main() {
    int msqid;
    char pathname[] = "Task3_b.c";
    key_t key;
    int len, max;
    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float message;
        } info;
    } clientbuf;
    struct sermsgbuf {
        long mtype;
        struct {
            float message;
        } info;
    } serverbuf;
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can not generate key\n");
        exit(-1);
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can not get msqid\n");
        exit(-1);
    }
    while (true) {
        max = sizeof(clientbuf.info);
        if (len = msgrcv(msqid, (struct clientmsgbuf*)&clientbuf, max, 1, 0) < 0) {
            printf("Can not receive message from queue\n");
            exit(-1);
        }
        printf("Got number %f from client with pid = %d, processing...\n", clientbuf.info.message, clientbuf.info.pid);
        serverbuf.mtype = clientbuf.info.pid;
        float number = clientbuf.info.message;
        serverbuf.info.message = number * number;
        len = sizeof(serverbuf.info);
        if (msgsnd(msqid, (struct sermsgbuf*)&serverbuf, len, 0) < 0) {
            printf("Can not send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
        }
        printf("Result is returned to client with pid = %d\n", clientbuf.info.pid);
    }
    return 0;
}
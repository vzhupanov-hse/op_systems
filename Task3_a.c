#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
    struct servermsgbuf {
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
    clientbuf.mtype = 1;
    clientbuf.info.pid = getpid();
    printf("Enter float number: ");
    float number;
    scanf("%f", &number);
    len = sizeof(clientbuf.info);
    clientbuf.info.message = number;
    printf("Number %f was sent by client with pid = %d\n", clientbuf.info.message, clientbuf.info.pid);
    if (msgsnd(msqid, (struct clientmsgbuf*)&clientbuf, len, 0) < 0) {
        printf("Can not send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    printf("Waiting for a response from a server\n");
    max = sizeof(serverbuf.info);
    if (len = msgrcv(msqid, &serverbuf, max, getpid(), 0) < 0) {
        printf("Can not receive message from queue\n");
        exit(-1);
    }
    printf("Response = %f\n", serverbuf.info.message);
    return 0;
}
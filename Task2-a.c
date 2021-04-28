#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main(void)
{
    int msqid;      // IPC descriptor for the message queue
    char path[] = "Task2-a.c";
    key_t  key;     // IPC key
    int len, maxlen; 
    struct mymsgbuf // Custom structure for the message
    {
        long mtype;
        struct {
            int  my_int;
            float my_float;
        } info;
    } mybuf;
    if ((key = ftok(path, 0)) < 0) {
        printf("Can not generate key\n");
        exit(-1);
    }
    //
    // Trying to get access by key to the message queue, if it exists,
    // or create it, with read & write access for all users.
    //
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can not get msqid\n");
        exit(-2);
    }
    for (int i = 0; i < 5; i++) {
        //
        // Set the message type to either an int, or a float
        // (based on i - if it's even or not).
        //
        mybuf.mtype = 1;
        mybuf.info.my_int = 100;
        //
        // Send the message. If there is an error,
        // report it and delete the message queue from the system.
        //
        len = sizeof(mybuf.info);
        if (msgsnd(msqid, (struct mymsgbuf*)&mybuf, len, 0) < 0) {
            printf("Can not send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-2);
        }
    }
    maxlen = sizeof(mybuf.info);
    for (int i = 0; i < 5; i++) {
        if ((len = msgrcv(msqid, (struct mymsgbuf*)&mybuf, maxlen, 2, 0)) < 0) {
            printf("Can not receive message from queue\n");
            exit(-3);
        }
        printf("Program A: type = %ld, received an float: %f\n", mybuf.mtype, mybuf.info.my_float);
    }
    return 0;
}
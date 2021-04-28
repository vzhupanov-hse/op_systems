#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char path[] = "Task1-a.c";
    key_t  key;
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
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can not get msqid\n");
        exit(-2);
    }
    while (1) {
        //
        // In an infinite loop, accept messages of any type in FIFO order
        // with a maximum length of 81 characters
        // until a message of type LAST_MESSAGE is received.
        //
        maxlen = sizeof(mybuf.info);
        if ((len = msgrcv(msqid, (struct mymsgbuf*)&mybuf, maxlen, 0, 0)) < 0) {
            printf("Can not receive message from queue\n");
            exit(-3);
        }
        //
        // If the received message is of type LAST_MESSAGE,
        // then terminate and remove the message queue from the system.
        // Otherwise, print the text of the received message.
        //
        if (mybuf.mtype == LAST_MESSAGE) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(0);
        }
        // Print a specific message based on the info provided.
        if (mybuf.mtype == 1) {
            printf("Message type = %ld, received an int: %d\n", mybuf.mtype, mybuf.info.my_int);
        }
        else if (mybuf.mtype == 2) {
            printf("Message type = %ld, received a float: %f\n", mybuf.mtype, mybuf.info.my_float);
        }
        else {
            printf("Error, unexpected message type: %ld, terminating.\n", mybuf.mtype);
        }
    }
    return 0;
}
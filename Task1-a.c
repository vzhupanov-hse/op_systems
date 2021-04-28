#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;      // IPC descriptor for the message queue
    char path[] = "Task1-a.c"; // The file name used to generate the key.
    key_t  key;     // IPC key
    int i, len;      // Cycle counter and the length of the informative part of the message
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
        exit(-1);
    }
    for (i = 0; i < 6; i++) {
        //
        // Set the message type to either an int, or a float
        // (based on i - if it's even or not).
        //
        if (i % 2 == 0) {
            mybuf.mtype = 1;
            mybuf.info.my_int = 100;
        }
        else {
            mybuf.mtype = 2;
            mybuf.info.my_float = 55.55;
        }
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
    mybuf.mtype = LAST_MESSAGE;
    len = 0;
    if (msgsnd(msqid, (struct mymsgbuf*)&mybuf, len, 0) < 0) {
        printf("Can not send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }
    return 0;
}
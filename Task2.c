#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;

void* newthread(void* dummy)
{
    pthread_t newthid;
    newthid = pthread_self();
    x = x + 1;
    printf("Thread %u. Result: %d\n", newthid, x);
    return NULL;
}

int main()
{
    pthread_t firstthid, secondthid, thirdthid;
    int res;
    res = pthread_create(&firstthid, (pthread_attr_t*)NULL, newthread, NULL);
    if (res != 0) 
    {
        printf("Error on thread create: %d\n", res);
        exit(-1);
    }
    printf("Second thread created. Thid: %u\n", firstthid);
    res = pthread_create(&secondthid, (pthread_attr_t*)NULL, newthread, NULL);
    if (res != 0) 
    {
        printf("Error on thread create: %d\n", res);
        exit(-1);
    }
    printf("Third thread created. Thid: %u\n", secondthid);
    thirdthid = pthread_self();
    x = x + 1;
    printf("Thread %u. Result: %d\n", thirdthid, x);
    pthread_join(firstthid, (void**)NULL);
    pthread_join(secondthid, (void**)NULL);
    return 0;
}
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    int fd;
    int result;
    size_t size;
    char str_res[14] = "Hello, world!";
    char path[] = "text.fifo";

    (void)umask(0);
    if ((fd = open(path, O_WRONLY)) < 0) {
        printf("Can not open fifo for writting\n");
        exit(-1);
    }
    size = write(fd, str_res, 14);
    if (size != 14) {
        printf("Can not write string to fifo\n");
        exit(-1);
    }
    printf("Result: %s\n", str_res);
    if (close(fd) < 0) {
        printf("Can not close fifo\n"); exit(-1);
    }
    return 0;
}